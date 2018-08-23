//-----------------------------------------------------------------------------
#include <QPainter>
#include <QtDebug>
#include <QGraphicsBlurEffect>
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
            largeur = fm.width(texte) + 2 * TRAIT;
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
CWGame::EResultat CWGame::joue(CDeplacement *dep, bool anim) {
    bool isMove = dep->deplacement(grille, score, anim);

    if(isMove) {
        if(!ajout(anim)) {
            return gagne ? CWGame::erFin : CWGame::erPerdu;
        }
        if(score == 2048) {
            gagne = true;
            return CWGame::erGagne;
        }
    }

    return (perdu() ? (gagne ? CWGame::erFin : CWGame::erPerdu) : isMove ? CWGame::erMove : CWGame::erNone);
}
//-----------------------------------------------------------------------------
void CWGame::decodeValue(int& x, int& y, int& valeur, int value) {
    valeur = value & 0x0000000F;
    x = (value & 0x000000C0) >> 6;
    y = (value & 0x00000030) >> 4;
}
//-----------------------------------------------------------------------------
int CWGame::encodeValue(int x, int y, int value) {
    return ((x & 0x00000003) << 6) | ((y & 0x00000003) << 4) | (value & 0x0000000F);
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

    calculFont(score, tailleCase);

    pen.setWidth(TRAIT);
    painter.setPen(pen);
    painter.setBrush(QBrush(couleurs[0]));
    painter.drawRect(xD, yD, taille, taille);

    for(i=0;i<CASE;i++) {
        int x = (i % COTE) * tailleCase + xD;
        int y = (i / COTE) * tailleCase + yD;
        int idCouleur = 0;
        QRect rect(x, y, tailleCase, tailleCase);

        if(grille[i].valeur != 0) {
            idCouleur = ((int)log2(grille[i].valeur)) % COULEURS;
        }

        if(grille[i].nouveau) {
            double scale = (double)step/(double)ANIM;
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
            QFont font(this->font);
            if(grille[i].fusion) {
                font.setPointSize(this->font.pointSize() / (ANIM - step));
            }
            painter.setFont(font);
            painter.setPen(QPen(Qt::black));
            painter.drawText(rect, Qt::AlignCenter, QString::number(grille[i].valeur));
        }

        if(step == ANIM - 1) {
            grille[i].nouveau = false;
            grille[i].fusion = false;
        }
    }
}
//-----------------------------------------------------------------------------
void CWGame::resizeEvent(QResizeEvent *) {
    forceFont = true;
}
//-----------------------------------------------------------------------------
CWGame::CWGame(QWidget *parent, bool initTimer) : QWidget(parent) {
    forceFont = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    step = 0;

    if(initTimer) {
        srand(time(NULL));
    }
    nouveau();

    timer->setInterval(DELAI);
    timer->start();
}
//-----------------------------------------------------------------------------
CWGame::~CWGame(void) {
    delete timer;
}
//-----------------------------------------------------------------------------
bool CWGame::ajout(bool anim) {
    int nbVide = 0;
    int vides[CASE];
    int i, j;

    for(i=j=0;i<CASE;i++) {
        if(grille[i].valeur == 0) {
            nbVide++;
            vides[j++] = i;
        }
    }

    if(nbVide != 0) {
        int idx = vides[rand() % nbVide];
        grille[idx].valeur = 2 * (rand() % 2 + 1);
        if(anim) {
            grille[idx].nouveau = true;
        }
        score = qMax(score, grille[idx].valeur);
        step = 0;

        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
void CWGame::nouveau(bool anim, int *value) {
    score = 0;
    gagne = false;
    memset(grille, 0, CASE * sizeof(SCase));

    if(value == 0) {
        ajout(anim);
        ajout(anim);
    } else {
        int x, y, valeur;

        decodeValue(x, y, valeur, *value & 0x000000FF);
        grille[y*COTE+x].valeur = valeur;

        decodeValue(x, y, valeur, (*value & 0x0000FF00) >> 8);
        grille[y*COTE+x].valeur = valeur;
    }
}
//-----------------------------------------------------------------------------
CWGame::EResultat CWGame::haut(bool anim) {
    CDeplacementHaut dep;

    return joue(&dep, anim);
}
//-----------------------------------------------------------------------------
CWGame::EResultat CWGame::droite(bool anim) {
    CDeplacementDroite dep;

    return joue(&dep, anim);
}
//-----------------------------------------------------------------------------
CWGame::EResultat CWGame::bas(bool anim) {
    CDeplacementBas dep;

    return joue(&dep, anim);
}
//-----------------------------------------------------------------------------
CWGame::EResultat CWGame::gauche(bool anim) {
    CDeplacementGauche dep;

    return joue(&dep, anim);
}
//-----------------------------------------------------------------------------
int CWGame::getScore(void) {
    return score;
}
//-----------------------------------------------------------------------------
const TCases& CWGame::getCases(void) const {
    return grille;
}
//-----------------------------------------------------------------------------
bool CWGame::perdu(void) {
    int i;

    for(i=0;i<CASE;i++) {
        if(grille[i].valeur == 0) {
            return false;
        }
    }

    for(i=0;i<CASE;i++) {
        if(i / COTE != COTE - 1) {
            if(grille[i].valeur == grille[i + COTE].valeur) {
                return false;
            }
        }
        if(i % COTE != COTE - 1) {
            if(grille[i].valeur == grille[i + 1].valeur) {
                return false;
            }
        }
    }

    return true;
}
//-----------------------------------------------------------------------------
int CWGame::getColonneMax(int colIdx) {
    int i, c;
    int m = 0;

    for(i=0,c=colIdx;i<COTE;i++,c+=COTE) {
        m = qMax(m, grille[c].valeur);
    }

    return m;
}
//-----------------------------------------------------------------------------
int CWGame::getLigneMax(int ligIdx) {
    int i, l;
    int m = 0;

    for(i=0,l=ligIdx*COTE;i<COTE;i++,l++) {
        m = qMax(m, grille[l].valeur);
    }

    return m;
}
//-----------------------------------------------------------------------------
int CWGame::getColonneMaxApres(int colIdx, int sens) {
    int i, c;
    int m = 0;
    int step = COTE * sens;

    c = (COTE * (COTE - 1) * (-0.5 * sens + 0.5)) + colIdx;
    for(i=0;i<COTE-1;i++,c+=step) {
        if(grille[c].valeur != 0 && grille[c+step].valeur == grille[c].valeur) {
            m = qMax(m, grille[c].valeur * 2);
        }
    }

    return m;
}
//-----------------------------------------------------------------------------
int CWGame::getLigneMaxApres(int ligIdx, int sens) {
    int i, l;
    int m = 0;
    int step = sens;

    l = COTE * ligIdx + (-0.5 * sens + 0.5) * (COTE - 1);
    for(i=0;i<COTE-1;i++,l+=step) {
        if(grille[l].valeur != 0 && grille[l+step].valeur == grille[l].valeur) {
            m = qMax(m, grille[l].valeur * 2);
        }
    }

    return m;
}
//-----------------------------------------------------------------------------
int CWGame::getSomme(void) {
    int i;
    int s = 0;

    for(i=0;i<CASE;i++) {
        s += grille[i].valeur;
    }

    return s;
}
//-----------------------------------------------------------------------------
int CWGame::genValue(void) {
    int vides[CASE];
    int i, j, k;
    TCases grille;
    int value = 0;

    memset(grille, 0, CASE * sizeof(SCase));

    for(k=0;k<2;k++) {
        int idx;
        for(i=j=0;i<CASE;i++) {
            if(grille[i].valeur == 0) {
                vides[j++] = i;
            }
        }

        idx = vides[rand() % (CASE - k)];
        grille[idx].valeur = 2 * (rand() % 2 + 1);

        value <<= 8;
        value |= encodeValue(idx % COTE, idx / COTE, grille[idx].valeur);
    }

    return value;
}
//-----------------------------------------------------------------------------
