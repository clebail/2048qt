//-----------------------------------------------------------------------------
#include <QtGlobal>
#include <QtDebug>
#include "CDeplacement.h"
//-----------------------------------------------------------------------------
int CDeplacement::getNbFusionnPossibleLigne(SCase *grille) {
    int x, y;
    int result = 0;

    for(x=0;x<COTE;x++) {
        int cur = grille[x].valeur;
        for(y=1;y<COTE;y++) {
            int idx = y * COTE + x;
            if(grille[idx].valeur != 0) {
                if(grille[idx].valeur == cur) {
                    result++;
                    cur = 0;
                } else {
                    cur = grille[idx].valeur;
                }
            }
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
int CDeplacement::getNbFusionnPossibleColonne(SCase *grille) {
    int x, y;
    int result = 0;

    for(y=0;y<COTE;y++) {
        int cur = grille[y * COTE].valeur;
        for(x=1;x<COTE;x++) {
            int idx = y * COTE + x;
            if(grille[idx].valeur != 0) {
                if(grille[idx].valeur == cur) {
                    result++;
                    cur = 0;
                } else {
                    cur = grille[idx].valeur;
                }
            }
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
bool CDeplacement::deplacement(SCase *grille, int &score, bool anim) {
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
                    if(anim) {
                        grille[voisin].fusion = true;
                    }
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
bool CDeplacement::canGo(const TCases &cases) {
    init();
    do {
        int voisin = getVoisin(idx);

        if(cases[idx].valeur != 0 && !cases[idx].fusion) {
            if(cases[voisin].valeur == cases[idx].valeur && !cases[voisin].fusion) {
                return true;
            } else if(cases[voisin].valeur == 0) {
                return true;
            }
        }
    }while(next());

    return false;
}
//-----------------------------------------------------------------------------
int CDeplacement::getNbFusionPossible(SCase *grille) {
    pf nbFusionPossible = getPf();

    return (this->*nbFusionPossible)(grille);
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
CDeplacement::pf CDeplacementHaut::getPf(void) {
    return &CDeplacementHaut::getNbFusionnPossibleColonne;
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
CDeplacement::pf CDeplacementDroite::getPf(void) {
    return &CDeplacementDroite::getNbFusionnPossibleLigne;
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
CDeplacement::pf CDeplacementBas::getPf(void) {
    return &CDeplacementBas::getNbFusionnPossibleColonne;
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
CDeplacement::pf CDeplacementGauche::getPf(void) {
    return &CDeplacementGauche::getNbFusionnPossibleLigne;
}
//-----------------------------------------------------------------------------

