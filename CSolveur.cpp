//-----------------------------------------------------------------------------
#include <QtDebug>
#include <QElapsedTimer>
#include <cmath>
#include <algorithm>
#include <random>
#include <future>
#include "CSolveur.h"
//-----------------------------------------------------------------------------
namespace {
    // rand() n'est pas garanti thread-safe : les 4 directions de tête étant
    // désormais recherchées en parallèle (voir solve()), chaque thread a besoin
    // de son propre générateur pour le tirage des cases vides dans chanceNode().
    thread_local std::mt19937 rng{std::random_device{}()};
}
//-----------------------------------------------------------------------------
CSolveur::CSolveur(const CGame &game, QObject *parent)
    : QThread(parent), game(game), arreter(false), gameUpdated(false) {
    for(auto &c : cacheMax) c.resize(MAX_DEPTH + 1);
    for(auto &c : cacheChance) c.resize(MAX_DEPTH + 1);
    CBitBoard::init();
}
//-----------------------------------------------------------------------------
void CSolveur::stop() {
    arreter = true;
    attente.wakeAll();
}
//-----------------------------------------------------------------------------
void CSolveur::updateGame(const CGame &game) {
    mutex.lock();
    this->game = game;
    gameUpdated = true;
    attente.wakeOne();
    mutex.unlock();
}
//-----------------------------------------------------------------------------
void CSolveur::run() {
    while(!arreter && !game.perdu() && !game.isGagne()) {
        CDeplacement::EDirection dir = solve();

        if(arreter || dir == CDeplacement::edNone) break;

        emit coupChoisi(dir);

        mutex.lock();
        while(!gameUpdated && !arreter)
            attente.wait(&mutex);
        gameUpdated = false;
        mutex.unlock();
    }
}
//-----------------------------------------------------------------------------
bool CSolveur::limiteTempsAtteinte() const {
    return arreter || minuteur.hasExpired(TEMPS_MAX_MS);
}
//-----------------------------------------------------------------------------
CDeplacement::EDirection CSolveur::solve() {
    minuteur.start();

    CDeplacement::EDirection meilleur = CDeplacement::edNone;

    // Recherche à profondeur itérative : on cherche à 1 coup, puis 2, 3...
    // tant qu'il reste du temps. On ne garde le résultat d'une profondeur que
    // si elle a pu être calculée entièrement avant la limite ; sinon elle est
    // jetée pour ne pas biaiser le choix avec une comparaison incomplète
    // entre directions. Ça borne le temps par coup tout en cherchant aussi
    // loin que la complexité du plateau (nb de cases vides) le permet.
    uint64_t plateau = CBitBoard::depuisGame(game);

    for(auto &c : cacheMax) for(auto &m : c) m.vider();
    for(auto &c : cacheChance) for(auto &m : c) m.vider();

    for(profondeurMax = 1; profondeurMax <= MAX_DEPTH && !arreter; profondeurMax++) {
        // Les 4 directions de tête sont indépendantes (chacune avec son propre
        // cache, voir CSolveur.h) : on les recherche en parallèle sur des
        // threads séparés plutôt que séquentiellement, pour exploiter les
        // coeurs disponibles au lieu d'un seul.
        std::vector<CDeplacement::EDirection> directions;
        std::vector<std::future<int>> futures;

        for(int i = (int)CDeplacement::edHaut; i <= (int)CDeplacement::edGauche; i++) {
            CDeplacement::EDirection dtest = (CDeplacement::EDirection)i;
            uint64_t resultat;
            if(CBitBoard::deplace(plateau, dtest, resultat)) {
                int idx = i - (int)CDeplacement::edHaut;
                directions.push_back(dtest);
                futures.push_back(std::async(std::launch::async, [this, resultat, idx]() {
                    return chanceNode(resultat, 0, cacheMax[idx], cacheChance[idx]);
                }));
            }
        }

        CDeplacement::EDirection d = CDeplacement::edNone;
        int best = -1;

        for(size_t k = 0; k < futures.size(); k++) {
            int score = futures[k].get();
            if(score > best) {
                best = score;
                d = directions[k];
            }
        }

        bool tronque = limiteTempsAtteinte();

        if(!tronque && d != CDeplacement::edNone) meilleur = d;

        if(tronque) break;
    }

    qDebug() << "solve() en" << minuteur.elapsed() << "ms (profondeur atteinte" << profondeurMax << ")";

    return meilleur;
}
//-----------------------------------------------------------------------------
int CSolveur::expectIMax(uint64_t plateau, int depth, TCache &cacheMax, TCache &cacheChance) {
    if(CBitBoard::perdu(plateau)) return 0;
    if(depth >= profondeurMax || limiteTempsAtteinte()) return evaluate(plateau);

    int restant = profondeurMax - depth;
    auto &table = cacheMax[restant];
    int valeurCache;
    if(table.get(plateau, valeurCache)) return valeurCache;

    int best = 0;

    for(int i = (int)CDeplacement::edHaut; i <= (int)CDeplacement::edGauche; i++) {
        CDeplacement::EDirection dtest = (CDeplacement::EDirection)i;
        uint64_t resultat;
        if(CBitBoard::deplace(plateau, dtest, resultat)) {
            int score = chanceNode(resultat, depth + 1, cacheMax, cacheChance);
            if(score > best) best = score;
        }
    }

    table.set(plateau, best);
    return best;
}
//-----------------------------------------------------------------------------
int CSolveur::chanceNode(uint64_t plateau, int depth, TCache &cacheMax, TCache &cacheChance) {
    int vides[CASE];
    int nbVide = CBitBoard::getCasesVide(plateau, vides);
    if(nbVide == 0) return 0;
    if(limiteTempsAtteinte()) return evaluate(plateau);

    int restant = profondeurMax - depth;
    auto &table = cacheChance[restant];
    int valeurCache;
    if(table.get(plateau, valeurCache)) return valeurCache;

    int nbTest = qMin(nbVide, MAX_VIDE);
    double chance = 0.0;

    // Tirage aléatoire (Fisher-Yates tronqué) plutôt que les nbTest premières
    // cases dans l'ordre du plateau : sinon, dès que nbVide > MAX_VIDE, seules
    // les cases en haut à gauche seraient jamais échantillonnées, biaisant
    // systématiquement l'espérance calculée.
    for(int i = 0; i < nbTest; i++) {
        int j = i + rng() % (nbVide - i);
        std::swap(vides[i], vides[j]);
    }

    for(int i = 0; i < nbTest; i++) {
        uint64_t p2 = CBitBoard::ajoute(plateau, vides[i], 2);
        uint64_t p4 = CBitBoard::ajoute(plateau, vides[i], 4);
        chance += expectIMax(p2, depth, cacheMax, cacheChance) * 0.9;
        chance += expectIMax(p4, depth, cacheMax, cacheChance) * 0.1;
    }

    int resultat = (int)(chance / nbTest);
    table.set(plateau, resultat);
    return resultat;
}
//-----------------------------------------------------------------------------
int CSolveur::evaluate(uint64_t plateau) {
    // Heuristique alignée sur les IA de référence (nneonneo/macroxue) : cases
    // vides (marge de survie), monotonie globale (direction cohérente sur
    // toute la grille), lissage (pénalise les écarts entre cases voisines,
    // qui compliquent les fusions futures) et nombre de fusions immédiatement
    // possibles. La seule recherche en profondeur ne suffit pas à produire un
    // rangement correct, ces critères le complètent.
    int vides[CASE];
    int nbVide = CBitBoard::getCasesVide(plateau, vides);

    int mono, lissage, fusions;
    CBitBoard::analyse(plateau, mono, lissage, fusions);

    return CBitBoard::getMaxValeur(plateau) + nbVide * POID_VIDE
         + mono * POID_MONO - lissage * POID_LISSE + fusions * POID_FUSION;
}
//-----------------------------------------------------------------------------
