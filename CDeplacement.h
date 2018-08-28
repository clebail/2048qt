//-----------------------------------------------------------------------------
#ifndef __CDEPLACEMENT_H__
#define __CDEPLACEMENT_H__
//-----------------------------------------------------------------------------
#include "common.h"
//-----------------------------------------------------------------------------
class CDeplacement {
protected:
    typedef int (CDeplacement::* pf)(SCase *grille);

    int idx;

    virtual int getVoisin(int idx) = 0;
    virtual bool next(void) = 0;
    virtual void init(void) = 0;
    virtual CDeplacement::pf getPf(void) = 0;

    int getNbFusionnPossibleLigne(SCase *grille);
    int getNbFusionnPossibleColonne(SCase *grille);
public:
    bool deplacement(SCase *grille, int &score, bool anim = true);
    bool canGo(const TCases& cases);
    virtual ~CDeplacement(void) {}
    int getNbFusionPossible(SCase *grille);
};
//-----------------------------------------------------------------------------
class CDeplacementHaut : public CDeplacement {
protected:
    virtual int getVoisin(int idx);
    virtual bool next(void);
    virtual void init(void);
    virtual CDeplacement::pf getPf(void);
};
//-----------------------------------------------------------------------------
class CDeplacementDroite : public CDeplacement {
protected:
    virtual int getVoisin(int idx);
    virtual bool next(void);
    virtual void init(void);
    virtual CDeplacement::pf getPf(void);
};
//-----------------------------------------------------------------------------
class CDeplacementBas : public CDeplacement {
protected:
    virtual int getVoisin(int idx);
    virtual bool next(void);
    virtual void init(void);
    virtual CDeplacement::pf getPf(void);
};
//-----------------------------------------------------------------------------
class CDeplacementGauche : public CDeplacement {
protected:
    virtual int getVoisin(int idx);
    virtual bool next(void);
    virtual void init(void);
    virtual CDeplacement::pf getPf(void);
};
//-----------------------------------------------------------------------------
#endif // __CDEPLACEMENT_H__
//-----------------------------------------------------------------------------
