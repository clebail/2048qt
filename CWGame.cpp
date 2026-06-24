//-----------------------------------------------------------------------------
#include <QPainter>
#include <QtDebug>
#include <tgmath.h>
#include "CWGame.h"
//-----------------------------------------------------------------------------
#define TRAIT       6
#define COULEURS    12
#define MS          1000
#define IMG         24
#define ANIM        8
#define DELAI       (MS/IMG)
//-----------------------------------------------------------------------------
static QColor couleurs[COULEURS] = {
    QColor(0xA0, 0xA0, 0xA0), //0
    QColor(0xFF, 0xCD, 0xD2), //2
    QColor(0xF8, 0xBB, 0xD0), //4
    QColor(0xE1, 0xBE, 0xE7), //8
    QColor(0xD1, 0xC4, 0xE9), //16
    QColor(0xC5, 0xCA, 0xE9), //32
    QColor(0xBB, 0xDE, 0xFB), //64
    QColor(0xB2, 0xDF, 0xDB), //128
    QColor(0xC8, 0xE6, 0xC9), //256
    QColor(0xDC, 0xED, 0xC8), //512
    QColor(0xFF, 0xF9, 0xCA), //1024
    QColor(0xD7, 0xCC, 0xC8), //2048
};
//-----------------------------------------------------------------------------
void CWGame::calculFont(int valeur, int tailleMax) {
    static int oldValeur = 0;
    if(valeur != oldValeur || forceFont) {
        int fontSize = 100;
        QString texte = QString::number(valeur);
        int largeur = 0, hauteur = 0;

        do {
            font = QFont("Arial", fontSize);
            QFontMetrics fm(font);
            largeur = fm.horizontalAdvance(texte) + 2 * TRAIT;
            hauteur = fm.height() + 2 * TRAIT;

            fontSize--;
        }while((largeur > tailleMax || hauteur > tailleMax) && fontSize > 1);

        if(fontSize < 1) {
            fontSize = 1;
        }

        oldValeur = valeur;
        forceFont = false;
    }
}
//-----------------------------------------------------------------------------
void CWGame::onTimer(void) {
    step++;
    repaint();
}
//-----------------------------------------------------------------------------
void CWGame::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    int taille = qMin(width(), height()) - TRAIT * COTE;
    int tailleCase = taille / COTE;
    int xD = (width() - taille) / 2;
    int yD = (height() - taille) / 2;
    QPen pen(Qt::darkGray);
    int i;
    const TCases& grille = game.getCases();

    calculFont(game.getScore(), tailleCase);

    pen.setWidth(TRAIT);
    painter.setPen(pen);
    painter.setBrush(QBrush(couleurs[0]));
    painter.drawRect(xD, yD, taille, taille);

    for(i = 0; i < CASE; i++) {
        int x = (i % COTE) * tailleCase + xD;
        int y = (i / COTE) * tailleCase + yD;
        int idCouleur = 0;
        QRect rect(x, y, tailleCase, tailleCase);

        if(grille[i].valeur != 0) {
            idCouleur = ((int)log2(grille[i].valeur)) % COULEURS;
        }

        if(grille[i].nouveau) {
            double scale = (double)step / (double)ANIM;
            int newTaille = tailleCase * scale;
            int translate = (tailleCase - newTaille) / 2;

            rect.setX(x + translate);
            rect.setY(y + translate);
            rect.setWidth(newTaille);
            rect.setHeight(newTaille);
        }

        painter.setPen(pen);
        painter.setBrush(QBrush(couleurs[idCouleur]));
        painter.drawRect(rect);

        if(grille[i].valeur != 0 && !grille[i].nouveau) {
            QFont f(this->font);
            if(grille[i].fusion) {
                f.setPointSize(this->font.pointSize() / (ANIM - step));
            }
            painter.setFont(f);
            painter.setPen(QPen(Qt::black));
            painter.drawText(rect, Qt::AlignCenter, QString::number(grille[i].valeur));
        }

    }

    if(step == ANIM - 1) {
        game.resetAnimFlags();
    }
}
//-----------------------------------------------------------------------------
void CWGame::resizeEvent(QResizeEvent *) {
    forceFont = true;
}
//-----------------------------------------------------------------------------
CWGame::CWGame(QWidget *parent) : QWidget(parent) {
    forceFont = false;
    step = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));

    game.nouveau(true);

    timer->setInterval(DELAI);
    timer->start();
}
//-----------------------------------------------------------------------------
CWGame::~CWGame(void) {
    delete timer;
}
//-----------------------------------------------------------------------------
bool CWGame::ajout(bool anim) {
    bool added = game.ajout(anim);
    if(added) step = 0;
    return added;
}
//-----------------------------------------------------------------------------
void CWGame::nouveau(bool anim) {
    game.nouveau(anim);
    step = 0;
}
//-----------------------------------------------------------------------------
CGame::EResultat CWGame::haut(bool anim) {
    step = 0;
    return game.haut(anim);
}
//-----------------------------------------------------------------------------
CGame::EResultat CWGame::droite(bool anim) {
    step = 0;
    return game.droite(anim);
}
//-----------------------------------------------------------------------------
CGame::EResultat CWGame::bas(bool anim) {
    step = 0;
    return game.bas(anim);
}
//-----------------------------------------------------------------------------
CGame::EResultat CWGame::gauche(bool anim) {
    step = 0;
    return game.gauche(anim);
}
//-----------------------------------------------------------------------------
int CWGame::getScore(void) {
    return game.getScore();
}
//-----------------------------------------------------------------------------
const TCases& CWGame::getCases(void) const {
    return game.getCases();
}
//-----------------------------------------------------------------------------
bool CWGame::perdu(void) {
    return game.perdu();
}
//-----------------------------------------------------------------------------
int CWGame::getSomme(void) {
    return game.getSomme();
}
//-----------------------------------------------------------------------------
int CWGame::getValeur(int idx) const {
    return game.getValeur(idx);
}
//-----------------------------------------------------------------------------
CGame& CWGame::getGame(void) {
    return game;
}
//-----------------------------------------------------------------------------
