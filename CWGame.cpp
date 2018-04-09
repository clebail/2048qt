//-----------------------------------------------------------------------------
#include <QPainter>
#include <QtDebug>
#include <tgmath.h>
#include "CWGame.h"
//-----------------------------------------------------------------------------
#define TRAIT   6
//-----------------------------------------------------------------------------
static QColor couleurs[] = {
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
   int fontSize = 100;
    QString texte = QString::number(valeur);
    int taille = 0;

    do {
        font = QFont("Arial", fontSize);
        QFontMetrics fm(font);
        taille = fm.width(texte) + 2 * TRAIT;

        fontSize--;
    }while(taille > tailleMax && fontSize > 1);
}
//-----------------------------------------------------------------------------
CWGame::EResultat CWGame::joue(CDeplacement *dep) {
    if(dep->deplacement(grille, score)) {
        if(!ajout()) {
            return CWGame::erPerdu;
        }
        repaint();
        if(score == 2048) {
            return CWGame::erGagne;
        }

        return CWGame::erNone;
    }

    return perdu() ? CWGame::erPerdu : CWGame::erNone;
}
//-----------------------------------------------------------------------------
bool CWGame::perdu(void) {
    int i;

    for(i=0;i<CASE;i++) {
        if(grille[i] == 0) {
            return false;
        }
    }

    return true;
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

    calculFont(qMax(10, score), tailleCase);

    pen.setWidth(TRAIT);
    painter.setPen(pen);
    painter.setBrush(QBrush(couleurs[0]));
    painter.drawRect(xD, yD, taille, taille);

    painter.setFont(font);

    for(i=0;i<CASE;i++) {
        int x = (i % COTE) * tailleCase + xD;
        int y = (i / COTE) * tailleCase + yD;
        int idCouleur = 0;
        QRect rect(x, y, tailleCase, tailleCase);

        if(grille[i] != 0) {
            idCouleur = log2(grille[i]);
        }

        painter.setPen(pen);
        painter.setBrush(QBrush(couleurs[idCouleur]));
        painter.drawRect(rect);

        if(grille[i] != 0) {
            painter.setPen(QPen(Qt::black));
            painter.drawText(rect, Qt::AlignCenter, QString::number(grille[i]));
        }
    }
}
//-----------------------------------------------------------------------------
CWGame::CWGame(QWidget *parent) : QWidget(parent) {
    srand(time(NULL));
    nouveau();
}
//-----------------------------------------------------------------------------
bool CWGame::ajout(void) {
    int nbVide = 0;
    int vides[CASE];
    int i, j;

    for(i=j=0;i<CASE;i++) {
        if(grille[i] == 0) {
            nbVide++;
            vides[j++] = i;
        }
    }

    if(nbVide != 0) {
        grille[vides[rand() % nbVide]] = 2;

        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
void CWGame::nouveau(void) {
    score = 2;
    memset(grille, 0, CASE * sizeof(int));

    ajout();
    ajout();

    repaint();
}
//-----------------------------------------------------------------------------
CWGame::EResultat CWGame::haut(void) {
    CDeplacementHaut dep;

    return joue(&dep);
}
//-----------------------------------------------------------------------------
CWGame::EResultat CWGame::droite(void) {
    CDeplacementDroite dep;

    return joue(&dep);
}
//-----------------------------------------------------------------------------
CWGame::EResultat CWGame::bas(void) {
    CDeplacementBas dep;

    return joue(&dep);
}
//-----------------------------------------------------------------------------
CWGame::EResultat CWGame::gauche(void) {
    CDeplacementGauche dep;

    return joue(&dep);
}
//-----------------------------------------------------------------------------
int CWGame::getScore(void) {
    return score;
}
//-----------------------------------------------------------------------------
