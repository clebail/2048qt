//-----------------------------------------------------------------------------
#ifndef __CWGAME_H__
#define __CWGAME_H__
//-----------------------------------------------------------------------------
#include <QWidget>
#include <QTimer>
#include "CGame.h"
//-----------------------------------------------------------------------------
class CWGame : public QWidget {
    Q_OBJECT
public:
    explicit CWGame(QWidget *parent = 0);
    ~CWGame(void);

    bool ajout(bool anim = true);
    void nouveau(bool anim = true);
    CGame::EResultat haut(bool anim = true);
    CGame::EResultat droite(bool anim = true);
    CGame::EResultat bas(bool anim = true);
    CGame::EResultat gauche(bool anim = true);
    int getScore(void);
    const TCases& getCases(void) const;
    bool perdu(void);
    int getSomme(void);
    int getValeur(int idx) const;
    CGame& getGame(void);

private:
    CGame game;
    bool forceFont;
    QFont font;
    QTimer *timer;
    int step;

    void calculFont(int valeur, int tailleMax);

private slots:
    void onTimer(void);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);
};
//-----------------------------------------------------------------------------
#endif // __CWGAME_H__
//-----------------------------------------------------------------------------
