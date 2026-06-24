//-----------------------------------------------------------------------------
#ifndef __CDEPLACEMENT_H__
#define __CDEPLACEMENT_H__
//-----------------------------------------------------------------------------
#include <QMetaType>
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
    enum EDirection { edHaut, edDroite, edBas, edGauche };

    bool deplacement(SCase *grille, int &score, bool anim = true);
    bool canGo(const TCases& cases);
    virtual ~CDeplacement(void) {}
    int getNbFusionPossible(SCase *grille);
    virtual EDirection getDirection() = 0;
};
//-----------------------------------------------------------------------------
class CDeplacementHaut : public CDeplacement {
protected:
    virtual int getVoisin(int idx);
    virtual bool next(void);
    virtual void init(void);
    virtual CDeplacement::pf getPf(void);
public:
    EDirection getDirection() { return edHaut; }
};
//-----------------------------------------------------------------------------
class CDeplacementDroite : public CDeplacement {
protected:
    virtual int getVoisin(int idx);
    virtual bool next(void);
    virtual void init(void);
    virtual CDeplacement::pf getPf(void);
public:
    EDirection getDirection() { return edDroite; }
};
//-----------------------------------------------------------------------------
class CDeplacementBas : public CDeplacement {
protected:
    virtual int getVoisin(int idx);
    virtual bool next(void);
    virtual void init(void);
    virtual CDeplacement::pf getPf(void);
public:
    EDirection getDirection() { return edBas; }
};
//-----------------------------------------------------------------------------
class CDeplacementGauche : public CDeplacement {
protected:
    virtual int getVoisin(int idx);
    virtual bool next(void);
    virtual void init(void);
    virtual CDeplacement::pf getPf(void);
public:
    EDirection getDirection() { return edGauche; }
};
//-----------------------------------------------------------------------------
Q_DECLARE_METATYPE(CDeplacement::EDirection)
//-----------------------------------------------------------------------------
#endif // __CDEPLACEMENT_H__
//-----------------------------------------------------------------------------
