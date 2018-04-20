//-----------------------------------------------------------------------------
#include <QtGlobal>
#include <QtDebug>
#include "CDeplacement.h"
//-----------------------------------------------------------------------------
bool CDeplacement::deplacement(SCase *grille, int &score) {
    bool move;
    int nbMove = 0;

    do {
        move = false;

        init();
        do {
            int voisin = getVoisin(idx);

            if(grille[idx].valeur != 0 && !grille[idx].fusion) {
                if(grille[voisin].valeur == grille[idx].valeur && !grille[voisin].fusion) {
                    grille[voisin].valeur *= 2;
                    grille[voisin].fusion = true;
                    grille[idx].valeur = 0;

                    score = qMax(grille[voisin].valeur, score);

                    move = true;
                    nbMove++;
                } else if(grille[voisin].valeur == 0) {
                    grille[voisin].valeur = grille[idx].valeur;
                    grille[idx].valeur = 0;

                    move = true;
                    nbMove++;
                }
            }
        }while(next());
    }while(move);

    return nbMove != 0;
}
//-----------------------------------------------------------------------------
int CDeplacementHaut::getVoisin(int idx) {
    return idx - COTE;
}
//-----------------------------------------------------------------------------
bool CDeplacementHaut::next(void) {
    if(idx + 1 < CASE) {
        idx++;
        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
void CDeplacementHaut::init(void) {
    idx = COTE;
}
//-----------------------------------------------------------------------------
int CDeplacementDroite::getVoisin(int idx) {
    return idx + 1;
}
//-----------------------------------------------------------------------------
bool CDeplacementDroite::next(void) {
    if(idx - 1 >= 0) {
        idx--;
        if(idx % COTE == COTE - 1) {
            idx--;
        }

        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
void CDeplacementDroite::init(void) {
    idx = CASE - 2;
}
//-----------------------------------------------------------------------------
int CDeplacementBas::getVoisin(int idx) {
    return idx + COTE;
}
//-----------------------------------------------------------------------------
bool CDeplacementBas::next(void) {
    if(idx - 1 >= 0) {
        idx--;

        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
void CDeplacementBas::init(void) {
    idx = CASE - COTE - 1;
}
//-----------------------------------------------------------------------------
int CDeplacementGauche::getVoisin(int idx) {
    return idx - 1;
}
//-----------------------------------------------------------------------------
bool CDeplacementGauche::next(void) {
    if(idx + 1 < CASE) {
        idx++;
        if(idx % COTE == 0) {
            idx++;
        }

        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
void CDeplacementGauche::init(void) {
    idx = 1;
}
//-----------------------------------------------------------------------------
