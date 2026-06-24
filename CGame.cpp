//-----------------------------------------------------------------------------
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include "CGame.h"
//-----------------------------------------------------------------------------
CGame::CGame() {
    score = 0;
    gagne = false;
    memset(grille, 0, CASE * sizeof(SCase));
}
//-----------------------------------------------------------------------------
bool CGame::ajout(bool anim, int *idx, int *value) {
    int nbVide = 0;
    int vides[CASE];

    nbVide = getCasesVide(vides);

    if(nbVide != 0) {
        int idx2Use;
        int value2Use;

        if(idx == nullptr) {
            idx2Use = vides[rand() % nbVide];
        } else {
            idx2Use = *idx;
        }

        if(value == nullptr) {
            value2Use = (rand() % 10 == 0) ? 4 : 2;
        } else {
            value2Use = *value;
        }

        grille[idx2Use].valeur = value2Use;

        if(anim) {
            grille[idx2Use].nouveau = true;
        }
        score = std::max(score, grille[idx2Use].valeur);

        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
void CGame::nouveau(bool anim) {
    score = 0;
    gagne = false;
    memset(grille, 0, CASE * sizeof(SCase));

    ajout(anim);
    ajout(anim);
}
//-----------------------------------------------------------------------------
CGame::EResultat CGame::joue(CDeplacement *dep, bool anim) {
    bool isMove = dep->deplacement(grille, score, anim);

    if(isMove) {
        if(!ajout(anim)) {
            return gagne ? CGame::erFin : CGame::erPerdu;
        }
        /*if(score == 2048) {
            gagne = true;
            return CGame::erGagne;
        }*/
    }

    return (perdu() ? (gagne ? CGame::erFin : CGame::erPerdu) : isMove ? CGame::erMove : CGame::erNone);
}
//-----------------------------------------------------------------------------
CGame::EResultat CGame::haut(bool anim) {
    CDeplacementHaut dep;
    return joue(&dep, anim);
}
//-----------------------------------------------------------------------------
CGame::EResultat CGame::droite(bool anim) {
    CDeplacementDroite dep;
    return joue(&dep, anim);
}
//-----------------------------------------------------------------------------
CGame::EResultat CGame::bas(bool anim) {
    CDeplacementBas dep;
    return joue(&dep, anim);
}
//-----------------------------------------------------------------------------
CGame::EResultat CGame::gauche(bool anim) {
    CDeplacementGauche dep;
    return joue(&dep, anim);
}
//-----------------------------------------------------------------------------
int CGame::getScore() const {
    return score;
}
//-----------------------------------------------------------------------------
const TCases& CGame::getCases() const {
    return grille;
}
//-----------------------------------------------------------------------------
bool CGame::perdu() const {
    int i;

    for(i = 0; i < CASE; i++) {
        if(grille[i].valeur == 0) {
            return false;
        }
    }

    for(i = 0; i < CASE; i++) {
        if(i / COTE != COTE - 1) {
            if(grille[i].valeur == grille[i + COTE].valeur) {
                return false;
            }
        }
        if(i % COTE != COTE - 1) {
            if(grille[i].valeur == grille[i + 1].valeur) {
                return false;
            }
        }
    }

    return true;
}
//-----------------------------------------------------------------------------
int CGame::getSomme() const {
    int i, s = 0;
    for(i = 0; i < CASE; i++) {
        s += grille[i].valeur;
    }
    return s;
}
//-----------------------------------------------------------------------------
void CGame::resetAnimFlags() {
    int i;
    for(i = 0; i < CASE; i++) {
        grille[i].nouveau = false;
        grille[i].fusion = false;
    }
}
//-----------------------------------------------------------------------------
int CGame::getValeur(int idx) const {
    return grille[idx].valeur;
}
//-----------------------------------------------------------------------------
bool CGame::isGagne() const {
    return gagne;
}
//-----------------------------------------------------------------------------
bool CGame::move(CDeplacement::EDirection dir) {
    switch(dir) {
        case CDeplacement::edHaut:   { CDeplacementHaut d;   return d.deplacement(grille, score, false); }
        case CDeplacement::edDroite: { CDeplacementDroite d; return d.deplacement(grille, score, false); }
        case CDeplacement::edBas:    { CDeplacementBas d;    return d.deplacement(grille, score, false); }
        case CDeplacement::edGauche: { CDeplacementGauche d; return d.deplacement(grille, score, false); }
        default: break;
    }
    return false;
}
//-----------------------------------------------------------------------------
bool CGame::canMove(CDeplacement::EDirection dir) const {
    switch(dir) {
        case CDeplacement::edHaut:   { CDeplacementHaut d;   return d.canGo(grille); }
        case CDeplacement::edDroite: { CDeplacementDroite d; return d.canGo(grille); }
        case CDeplacement::edBas:    { CDeplacementBas d;    return d.canGo(grille); }
        case CDeplacement::edGauche: { CDeplacementGauche d; return d.canGo(grille); }
        default: break;
    }
    return false;
}
//-----------------------------------------------------------------------------
int CGame::getCasesVide(int *vides) const {
    int i, j, nbVide = 0;

    memset(vides, 0, CASE * sizeof(int));

    for(i = j = 0; i < CASE; i++) {
        if(grille[i].valeur == 0) {
            nbVide++;
            vides[j++] = i;
        }
    }

    return nbVide;
}
//-----------------------------------------------------------------------------
