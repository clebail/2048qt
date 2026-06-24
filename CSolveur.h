//-----------------------------------------------------------------------------
#ifndef __CSOLVEUR_H__
#define __CSOLVEUR_H__
//-----------------------------------------------------------------------------
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>
#include <atomic>
#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>
#include "CGame.h"
#include "CBitBoard.h"
//-----------------------------------------------------------------------------
class CSolveur : public QThread {
    Q_OBJECT
public:
    explicit CSolveur(const CGame &game, QObject *parent = nullptr);
    void stop();
    void updateGame(const CGame &game);

signals:
    void coupChoisi(CDeplacement::EDirection direction);

protected:
    void run() override;

private:
    CGame game;
    std::atomic<bool> arreter;
    QMutex mutex;
    QWaitCondition attente;
    bool gameUpdated;
    QElapsedTimer minuteur;
    int profondeurMax;

    // Table de transposition à tableau plat (adressage direct, écrasement en
    // cas de collision, pas de chaînage) : remplace l'ancien
    // unordered_map<uint64_t,int>, dont chaque insertion alloue un nœud sur le
    // tas et dont le vidage entre deux appels à solve() itère et libère chaque
    // nœud. Ici le stockage est un tableau contigu de taille fixe, l'entrée
    // est retrouvée par un hash multiplicatif (Fibonacci hashing, dispersion
    // correcte quels que soient les bits du plateau), et le vidage est un
    // simple memset. Un faux miss (collision non détectée) ne fait au pire
    // que recalculer un sous-arbre, jamais renvoyer une mauvaise valeur (le
    // plateau complet est revérifié à la lecture).
    struct TEntree {
        uint64_t plateau;
        int valeur;
        int occupe;
    };
    class TTableCache {
    public:
        TTableCache() : table(TAILLE) {}
        void vider() { std::fill(table.begin(), table.end(), TEntree{0, 0, 0}); }
        bool get(uint64_t plateau, int &valeur) const {
            const TEntree &e = table[index(plateau)];
            if(e.occupe && e.plateau == plateau) { valeur = e.valeur; return true; }
            return false;
        }
        void set(uint64_t plateau, int valeur) {
            table[index(plateau)] = TEntree{plateau, valeur, 1};
        }
    private:
        static constexpr size_t TAILLE = size_t(1) << CACHE_BITS;
        static size_t index(uint64_t plateau) {
            return (size_t)((plateau * 0x9E3779B97F4A7C15ULL) >> (64 - CACHE_BITS));
        }
        std::vector<TEntree> table;
    };
    using TCache = std::vector<TTableCache>;
    // Table de transposition : beaucoup de séquences de coups différentes
    // mènent au même plateau, donc on mémorise le résultat par (plateau,
    // profondeur restante) pour ne jamais recalculer deux fois le même
    // sous-arbre. Indexé par profondeur restante (0..MAX_DEPTH), vidé à
    // chaque nouvel appel à solve() (les valeurs ne dépendent que du plateau
    // et de la profondeur restante, donc réutilisables entre les paliers
    // successifs de la recherche itérative d'un même coup). Un cache par
    // direction de premier coup (au lieu d'un seul partagé) : les 4 branches
    // de tête sont recherchées en parallèle dans solve(), et un cache partagé
    // entre threads aurait demandé un verrou sur chaque accès, bien trop
    // fréquent pour rester rentable.
    std::array<TCache, 4> cacheMax;
    std::array<TCache, 4> cacheChance;

    CDeplacement::EDirection solve();
    bool limiteTempsAtteinte() const;
    int expectIMax(uint64_t plateau, int depth, TCache &cacheMax, TCache &cacheChance);
    int chanceNode(uint64_t plateau, int depth, TCache &cacheMax, TCache &cacheChance);
    int evaluate(uint64_t plateau);
};
//-----------------------------------------------------------------------------
#endif // __CSOLVEUR_H__
//-----------------------------------------------------------------------------
