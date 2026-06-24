//-----------------------------------------------------------------------------
#ifndef __CGAME_H__
#define __CGAME_H__
//-----------------------------------------------------------------------------
#include "common.h"
#include "CDeplacement.h"
//-----------------------------------------------------------------------------
class CGame {
public:
    enum EResultat { erNone, erGagne, erPerdu, erFin, erMove };

    CGame();
    CGame(const CGame &other) = default;
    CGame& operator=(const CGame &other) = default;

    bool ajout(bool anim = false);
    void nouveau(bool anim = false);
    EResultat haut(bool anim = false);
    EResultat droite(bool anim = false);
    EResultat bas(bool anim = false);
    EResultat gauche(bool anim = false);
    int getScore() const;
    const TCases& getCases() const;
    void resetAnimFlags();
    bool perdu() const;
    int getSomme() const;
    int getValeur(int idx) const;
    bool isGagne() const;

private:
    int score;
    TCases grille;
    bool gagne;

    EResultat joue(CDeplacement *dep, bool anim);
};
//-----------------------------------------------------------------------------
#endif // __CGAME_H__
//-----------------------------------------------------------------------------
