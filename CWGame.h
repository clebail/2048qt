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
    typedef enum { erNone, erGagne, erPerdu } EResultat;
private:
    int score;
    int grille[CASE];
    int idxNouveau;
    bool forceFont;
    QFont font;
    QTimer *timer;
    int step, cpt;

    void calculFont(int valeur, int tailleMax);
    CWGame::EResultat joue(CDeplacement *dep);
    bool perdu(void);
private slots:
    void onTimer(void);
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);
public:
    explicit CWGame(QWidget *parent = 0);
    ~CWGame(void);
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
