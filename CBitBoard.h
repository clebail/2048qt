//-----------------------------------------------------------------------------
#ifndef __CBITBOARD_H__
#define __CBITBOARD_H__
//-----------------------------------------------------------------------------
#include <cstdint>
#include "common.h"
#include "CDeplacement.h"
//-----------------------------------------------------------------------------
class CGame;
//-----------------------------------------------------------------------------
// Représentation compacte du plateau pour la recherche du solveur : les 16
// cases sont encodées sur 4 bits chacune (l'exposant de la valeur, 0 = case
// vide, 1 = 2, 2 = 4, ...) dans un seul uint64_t. Les déplacements horizontaux
// sont résolus via des tables précalculées (une par ligne possible sur 16
// bits), les déplacements verticaux réutilisent ces tables après transposition
// du plateau. Ça évite les copies de structures et les appels virtuels de
// CGame/CDeplacement, bien trop coûteux pour une recherche à grande profondeur.
namespace CBitBoard {
    void init();

    uint64_t depuisGame(const CGame &g);

    // Renvoie false si le déplacement ne change pas le plateau (coup illégal)
    bool deplace(uint64_t plateau, CDeplacement::EDirection dir, uint64_t &resultat);
    bool peutDeplacer(uint64_t plateau, CDeplacement::EDirection dir);

    int getCasesVide(uint64_t plateau, int *vides);
    int getMaxValeur(uint64_t plateau);
    uint64_t ajoute(uint64_t plateau, int idx, int valeur);
    bool perdu(uint64_t plateau);

    // Monotonie (direction cohérente sur toute la grille, pas ligne par
    // ligne), lissage (pénalité d'écart entre cases voisines) et nombre de
    // fusions immédiatement possibles. Les cases stockent déjà l'exposant
    // log2 de la valeur, donc ces calculs sont purement entiers.
    void analyse(uint64_t plateau, int &mono, int &lissage, int &fusions);
}
//-----------------------------------------------------------------------------
#endif // __CBITBOARD_H__
//-----------------------------------------------------------------------------
