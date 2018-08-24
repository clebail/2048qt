//-----------------------------------------------------------------------------
#ifndef __CWGAME_H__
#define __CWGAME_H__
//-----------------------------------------------------------------------------
#include <QWidget>
#include <QTimer>
#include "common.h"
#include "CDeplacement.h"
//-----------------------------------------------------------------------------
class CWGame : public QWidget {
    Q_OBJECT
public:
    typedef enum { erNone, erGagne, erPerdu, erFin, erMove } EResultat;
private:
    int score;
    TCases grille;
    int fusion[CASE];
    bool forceFont;
    QFont font;
    QTimer *timer;
    int step;
    bool gagne;

    void calculFont(int valeur, int tailleMax);
    CWGame::EResultat joue(CDeplacement *dep, bool anim);
    void decodeValue(int& x, int& y, int& valeur, int value);
    int encodeValue(int x, int y, int value);
    int getColonneMax(int colIdx);
    int getLigneMax(int ligIdx);
    void getColonneResultatApres(int colIdx, int sens, int& max, int& nbFusion, int& nbVide);
    void getLigneResultatApres(int ligIdx, int sens, int& max, int& nbFusion, int& nbVide);
private slots:
    void onTimer(void);
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);
public:
    explicit CWGame(QWidget *parent = 0, bool initTimer = false);
    ~CWGame(void);
    bool ajout(bool anim=true);
    void nouveau(bool anim=true, int *value=0);
    CWGame::EResultat haut(bool anim=true);
    CWGame::EResultat droite(bool anim=true);
    CWGame::EResultat bas(bool anim=true);
    CWGame::EResultat gauche(bool anim=true);
    int getScore(void);
    const TCases& getCases(void) const;
    bool perdu(void);
    int getSomme(void);
    int genValue(void);
    int getColMax(void);
    int getLigMax(void);
    void getColResultatApres(int sens, int& max, int& nbFusion, int& nbVide);
    void getLigResultatApres(int sens, int& max, int& nbFusion, int& nbVide);
};
//-----------------------------------------------------------------------------
#endif // __CWGAME_H__
//-----------------------------------------------------------------------------
