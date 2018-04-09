//-----------------------------------------------------------------------------
#ifndef __CDEPLACEMENT_H__
#define __CDEPLACEMENT_H__
//-----------------------------------------------------------------------------
#include "common.h"
//-----------------------------------------------------------------------------
class CDeplacement {
protected:
    int idx;

    virtual int getVoisin(int idx) = 0;
    virtual bool next(void) = 0;
    virtual void init(void) = 0;
public:
    bool deplacement(int *grille, int &score);
    int getIdx(void);
    virtual ~CDeplacement(void) {}
};
//-----------------------------------------------------------------------------
class CDeplacementHaut : public CDeplacement {
protected:
    virtual int getVoisin(int idx);
    virtual bool next(void);
    virtual void init(void);
};
//-----------------------------------------------------------------------------
class CDeplacementDroite : public CDeplacement {
protected:
    virtual int getVoisin(int idx);
    virtual bool next(void);
    virtual void init(void);
};
//-----------------------------------------------------------------------------
class CDeplacementBas : public CDeplacement {
protected:
    virtual int getVoisin(int idx);
    virtual bool next(void);
    virtual void init(void);
};
//-----------------------------------------------------------------------------
class CDeplacementGauche : public CDeplacement {
protected:
    virtual int getVoisin(int idx);
    virtual bool next(void);
    virtual void init(void);
};
//-----------------------------------------------------------------------------
#endif // __CDEPLACEMENT_H__
//-----------------------------------------------------------------------------
