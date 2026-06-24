//-----------------------------------------------------------------------------
#include "CBitBoard.h"
#include "CGame.h"
#include <algorithm>
//-----------------------------------------------------------------------------
namespace {
    uint16_t rowGauche[65536];
    uint16_t rowDroite[65536];
    int16_t rowMonoGauche[65536];
    int16_t rowMonoDroite[65536];
    int16_t rowLissage[65536];
    int16_t rowFusions[65536];
    bool initialise = false;

    // Simule un déplacement vers la gauche d'une ligne de 4 cases encodée sur
    // 16 bits (4 nibbles, case 0 = nibble de poids faible) : on compacte les
    // valeurs non nulles vers la gauche, puis on fusionne une seule fois les
    // paires égales adjacentes (de gauche à droite, sans effet de cascade),
    // exactement comme les règles standard du 2048.
    uint16_t simuleGauche(uint16_t ligne) {
        int cases[COTE];
        for(int i = 0; i < COTE; i++) cases[i] = (ligne >> (4 * i)) & 0xF;

        int compact[COTE] = {0, 0, 0, 0};
        int pos = 0;
        for(int i = 0; i < COTE; i++) if(cases[i] != 0) compact[pos++] = cases[i];

        int fusionne[COTE] = {0, 0, 0, 0};
        pos = 0;
        for(int i = 0; i < COTE; i++) {
            if(compact[i] == 0) continue;
            if(i + 1 < COTE && compact[i] == compact[i + 1]) {
                fusionne[pos++] = compact[i] + 1;
                i++;
            } else {
                fusionne[pos++] = compact[i];
            }
        }

        uint16_t resultat = 0;
        for(int i = 0; i < COTE; i++) resultat |= ((uint16_t)fusionne[i]) << (4 * i);
        return resultat;
    }

    // Les cases contiennent déjà l'exposant log2 de la valeur (0 = vide), donc
    // pas besoin de log2()/pow() ici : une simple soustraction d'exposants
    // donne directement l'écart en log2 entre deux cases voisines.
    int calcMonoGauche(const int cases[COTE]) {
        int total = 0;
        for(int i = 0; i < COTE - 1; i++) {
            int a = cases[i], b = cases[i + 1];
            if(a != 0 && b != 0 && a > b) total += a - b;
        }
        return total;
    }

    int calcMonoDroite(const int cases[COTE]) {
        int total = 0;
        for(int i = 0; i < COTE - 1; i++) {
            int a = cases[i], b = cases[i + 1];
            if(a != 0 && b != 0 && b > a) total += b - a;
        }
        return total;
    }

    int calcLissage(const int cases[COTE]) {
        int total = 0;
        for(int i = 0; i < COTE - 1; i++) {
            int a = cases[i], b = cases[i + 1];
            if(a != 0 && b != 0) total += (a > b) ? (a - b) : (b - a);
        }
        return total;
    }

    int calcFusions(const int cases[COTE]) {
        int total = 0;
        for(int i = 0; i < COTE - 1; i++) {
            if(cases[i] != 0 && cases[i] == cases[i + 1]) total++;
        }
        return total;
    }

    uint16_t inverseNibbles(uint16_t x) {
        return (uint16_t)(((x & 0xF) << 12) | ((x & 0xF0) << 4) | ((x & 0xF00) >> 4) | ((x & 0xF000) >> 12));
    }

    // Transposition en O(1) (2 étapes d'échange de blocs) au lieu de la boucle
    // sur 16 nibbles : appelée à chaque déplacement vertical et deux fois par
    // analyse(), donc très chaude. Étape 1 échange les blocs de 3 nibbles situés
    // de part et d'autre de la diagonale (distance 12 bits), étape 2 fait de
    // même avec des blocs de 6 nibbles (distance 24 bits) — technique standard
    // de transposition de matrice 4x4 par blocs, reprise des IA de référence
    // (nneonneo). Validée par comparaison exhaustive avec l'ancienne version.
    uint64_t transpose(uint64_t plateau) {
        uint64_t a1 = plateau & 0xF0F00F0FF0F00F0FULL;
        uint64_t a2 = plateau & 0x0000F0F00000F0F0ULL;
        uint64_t a3 = plateau & 0x0F0F00000F0F0000ULL;
        uint64_t a  = a1 | (a2 << 12) | (a3 >> 12);
        uint64_t b1 = a & 0xFF00FF0000FF00FFULL;
        uint64_t b2 = a & 0x00FF00FF00000000ULL;
        uint64_t b3 = a & 0x00000000FF00FF00ULL;
        return b1 | (b2 >> 24) | (b3 << 24);
    }

    inline int nibble(uint64_t plateau, int idx) {
        return (int)((plateau >> (4 * idx)) & 0xF);
    }
}
//-----------------------------------------------------------------------------
namespace CBitBoard {

void init() {
    if(initialise) return;

    for(uint32_t ligne = 0; ligne < 65536; ligne++) {
        rowGauche[ligne] = simuleGauche((uint16_t)ligne);
        rowDroite[ligne] = inverseNibbles(simuleGauche(inverseNibbles((uint16_t)ligne)));

        int cases[COTE];
        for(int i = 0; i < COTE; i++) cases[i] = (ligne >> (4 * i)) & 0xF;

        rowMonoGauche[ligne] = (int16_t)calcMonoGauche(cases);
        rowMonoDroite[ligne] = (int16_t)calcMonoDroite(cases);
        rowLissage[ligne] = (int16_t)calcLissage(cases);
        rowFusions[ligne] = (int16_t)calcFusions(cases);
    }

    initialise = true;
}
//-----------------------------------------------------------------------------
uint64_t depuisGame(const CGame &g) {
    uint64_t plateau = 0;

    for(int i = 0; i < CASE; i++) {
        int valeur = g.getValeur(i);
        int exposant = 0;
        while(valeur > 1) { exposant++; valeur >>= 1; }
        plateau |= ((uint64_t)exposant) << (4 * i);
    }

    return plateau;
}
//-----------------------------------------------------------------------------
bool deplace(uint64_t plateau, CDeplacement::EDirection dir, uint64_t &resultat) {
    uint64_t travail = plateau;
    bool horizontal = (dir == CDeplacement::edGauche || dir == CDeplacement::edDroite);

    if(!horizontal) travail = transpose(travail);

    const uint16_t *table = (dir == CDeplacement::edGauche || dir == CDeplacement::edHaut) ? rowGauche : rowDroite;

    uint64_t sortie = 0;
    for(int r = 0; r < COTE; r++) {
        uint16_t ligne = (uint16_t)((travail >> (16 * r)) & 0xFFFF);
        sortie |= ((uint64_t)table[ligne]) << (16 * r);
    }

    if(!horizontal) sortie = transpose(sortie);

    resultat = sortie;
    return sortie != plateau;
}
//-----------------------------------------------------------------------------
bool peutDeplacer(uint64_t plateau, CDeplacement::EDirection dir) {
    uint64_t resultat;
    return deplace(plateau, dir, resultat);
}
//-----------------------------------------------------------------------------
int getCasesVide(uint64_t plateau, int *vides) {
    int nb = 0;
    for(int i = 0; i < CASE; i++) {
        if(nibble(plateau, i) == 0) vides[nb++] = i;
    }
    return nb;
}
//-----------------------------------------------------------------------------
int getMaxValeur(uint64_t plateau) {
    int maxExp = 0;
    for(int i = 0; i < CASE; i++) {
        int e = nibble(plateau, i);
        if(e > maxExp) maxExp = e;
    }
    return maxExp == 0 ? 0 : (1 << maxExp);
}
//-----------------------------------------------------------------------------
uint64_t ajoute(uint64_t plateau, int idx, int valeur) {
    int exposant = (valeur == 4) ? 2 : 1;
    return plateau | (((uint64_t)exposant) << (4 * idx));
}
//-----------------------------------------------------------------------------
void analyse(uint64_t plateau, int &mono, int &lissage, int &fusions) {
    int gaucheTotal = 0, droiteTotal = 0, hautTotal = 0, basTotal = 0;
    int lissageTotal = 0, fusionsTotal = 0;

    for(int r = 0; r < COTE; r++) {
        uint16_t ligne = (uint16_t)((plateau >> (16 * r)) & 0xFFFF);
        gaucheTotal += rowMonoGauche[ligne];
        droiteTotal += rowMonoDroite[ligne];
        lissageTotal += rowLissage[ligne];
        fusionsTotal += rowFusions[ligne];
    }

    uint64_t transpose_ = transpose(plateau);
    for(int r = 0; r < COTE; r++) {
        uint16_t ligne = (uint16_t)((transpose_ >> (16 * r)) & 0xFFFF);
        hautTotal += rowMonoGauche[ligne];
        basTotal += rowMonoDroite[ligne];
        lissageTotal += rowLissage[ligne];
        fusionsTotal += rowFusions[ligne];
    }

    mono = std::max(gaucheTotal, droiteTotal) + std::max(hautTotal, basTotal);
    lissage = lissageTotal;
    fusions = fusionsTotal;
}
//-----------------------------------------------------------------------------
bool perdu(uint64_t plateau) {
    for(int i = 0; i < CASE; i++) {
        int v = nibble(plateau, i);
        if(v == 0) return false;

        int row = i / COTE, col = i % COTE;
        if(row < COTE - 1 && nibble(plateau, i + COTE) == v) return false;
        if(col < COTE - 1 && nibble(plateau, i + 1) == v) return false;
    }
    return true;
}

}
//-----------------------------------------------------------------------------
