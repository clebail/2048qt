//-----------------------------------------------------------------------------
#ifndef __CWGAME_H__
#define __CWGAME_H__
//-----------------------------------------------------------------------------
#include <QWidget>
#include "common.h"
#include "CDeplacement.h"
//-----------------------------------------------------------------------------
class CWGame : public QWidget {
    Q_OBJECT
public:
    typedef enum { erNone, erGagne, erPerdu } EResultat;
private:
    int score;
    int grille[CASE];
    QFont font;

    void calculFont(int valeur, int tailleMax);
    CWGame::EResultat joue(CDeplacement *dep);
    bool perdu(void);
protected:
    virtual void paintEvent(QPaintEvent *);
public:
    explicit CWGame(QWidget *parent = 0);
    bool ajout(void);
    void nouveau(void);
    CWGame::EResultat haut(void);
    CWGame::EResultat droite(void);
    CWGame::EResultat bas(void);
    CWGame::EResultat gauche(void);
    int getScore(void);
};
//-----------------------------------------------------------------------------
#endif // __CWGAME_H__
//-----------------------------------------------------------------------------
