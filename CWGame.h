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
    CWGame::EResultat joue(CDeplacement *dep);
private slots:
    void onTimer(void);
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);
public:
    explicit CWGame(QWidget *parent = 0, bool initTimer = false);
    ~CWGame(void);
    bool ajout(void);
    void nouveau(void);
    CWGame::EResultat haut(void);
    CWGame::EResultat droite(void);
    CWGame::EResultat bas(void);
    CWGame::EResultat gauche(void);
    int getScore(void);
    const TCases& getCases(void) const;
    bool perdu(void);
};
//-----------------------------------------------------------------------------
#endif // __CWGAME_H__
//-----------------------------------------------------------------------------
