//-----------------------------------------------------------------------------
#include <QtGlobal>
#include <QtDebug>
#include "CDeplacement.h"
//-----------------------------------------------------------------------------
bool CDeplacement::deplacement(int *grille, int &score) {
    bool move;
    int nbMove = 0;

    do {
        move = false;

        init();
        do {
            int i = getIdx();
            int voisin = getVoisin(i);

            if(grille[i] != 0) {
                if(grille[voisin] == grille[i]) {
                    grille[voisin] *= 2;
                    grille[i] = 0;

                    score = qMax(grille[voisin], score);

                    move = true;
                    nbMove++;
                } else if(grille[voisin] == 0) {
                    grille[voisin] = grille[i];
                    grille[i] = 0;

                    move = true;
                    nbMove++;
                }
            }
        }while(next());
    }while(move);

    return nbMove != 0;
}
//-----------------------------------------------------------------------------
int CDeplacement::getIdx(void) {
    return idx;
}
//-----------------------------------------------------------------------------
int CDeplacementHaut::getVoisin(int idx) {
    return idx - COTE;
}
//-----------------------------------------------------------------------------
bool CDeplacementHaut::next(void) {
    if(this->idx + 1 < CASE) {
        this->idx++;
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
    if(this->idx - 1 >= 0) {
        this->idx--;
        if((this->idx) % COTE == COTE -1) {
            this->idx--;
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
    if(this->idx - 1 >= 0) {
        this->idx--;

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
    if(this->idx +1 < CASE) {
        this->idx++;
        if((this->idx) % COTE == 0) {
            this->idx++;
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
