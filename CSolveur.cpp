//-----------------------------------------------------------------------------
#include <QtDebug>
#include <cmath>
#include "CSolveur.h"
//-----------------------------------------------------------------------------
CSolveur::CSolveur(const CGame &game, QObject *parent)
    : QThread(parent), game(game), arreter(false), gameUpdated(false) {
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
CDeplacement::EDirection CSolveur::solve() {
    CDeplacement::EDirection d = CDeplacement::edNone;
    int best = -1;

    for(int i = (int)CDeplacement::edHaut; i <= (int)CDeplacement::edGauche; i++) {
        CDeplacement::EDirection dtest = (CDeplacement::EDirection)i;
        if(game.canMove(dtest)) {
            CGame other(game);
            other.move(dtest);
            int score = chanceNode(other, 0);
            if(score > best) {
                best = score;
                d = dtest;
            }
        }
    }

    return d;
}
//-----------------------------------------------------------------------------
int CSolveur::expectIMax(const CGame &g, int depth) {
    if(g.perdu()) return 0;
    if(depth >= MAX_DEPTH) return evaluate(g);

    int best = 0;

    for(int i = (int)CDeplacement::edHaut; i <= (int)CDeplacement::edGauche; i++) {
        CDeplacement::EDirection dtest = (CDeplacement::EDirection)i;
        if(g.canMove(dtest)) {
            CGame other(g);
            other.move(dtest);
            int score = chanceNode(other, depth + 1);
            if(score > best) best = score;
        }
    }

    return best;
}
//-----------------------------------------------------------------------------
int CSolveur::chanceNode(const CGame &g, int depth) {
    int vides[CASE];
    int nbVide = g.getCasesVide(vides);
    if(nbVide == 0) return 0;

    int nbTest = qMin(nbVide, MAX_VIDE);
    double chance = 0.0;

    for(int i = 0; i < nbTest; i++) {
        int v2 = 2, v4 = 4;
        CGame g2(g), g4(g);
        g2.ajout(false, &vides[i], &v2);
        g4.ajout(false, &vides[i], &v4);
        chance += expectIMax(g2, depth) * 0.9;
        chance += expectIMax(g4, depth) * 0.1;
    }

    return (int)(chance / nbTest);
}
//-----------------------------------------------------------------------------
int CSolveur::evaluate(const CGame &g) {
    int vides[CASE];
    int nbVide = g.getCasesVide(vides);

    double mono = 0.0;

    for(int row = 0; row < COTE; row++) {
        double inc = 0.0, dec = 0.0;
        for(int col = 0; col < COTE - 1; col++) {
            int a = g.getValeur(row * COTE + col);
            int b = g.getValeur(row * COTE + col + 1);
            if(a == 0 || b == 0) continue;
            if(a > b) dec += log2(a) - log2(b);
            else if(b > a) inc += log2(b) - log2(a);
        }
        mono += qMax(inc, dec);
    }

    for(int col = 0; col < COTE; col++) {
        double inc = 0.0, dec = 0.0;
        for(int row = 0; row < COTE - 1; row++) {
            int a = g.getValeur(row * COTE + col);
            int b = g.getValeur((row + 1) * COTE + col);
            if(a == 0 || b == 0) continue;
            if(a > b) dec += log2(a) - log2(b);
            else if(b > a) inc += log2(b) - log2(a);
        }
        mono += qMax(inc, dec);
    }

    return g.getScore() + nbVide * POID_VIDE + (int)(mono * POID_MONO);
}
//-----------------------------------------------------------------------------
