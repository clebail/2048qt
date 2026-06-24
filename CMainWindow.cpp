//-----------------------------------------------------------------------------
#include <QKeyEvent>
#include <QtDebug>
#include <QMessageBox>
#include "CMainWindow.h"
//-----------------------------------------------------------------------------
CMainWindow::CMainWindow(QWidget *parent) : QMainWindow(parent), solveur(nullptr) {
    qRegisterMetaType<CDeplacement::EDirection>();
    srand(time(NULL));

    setupUi(this);

    installEventFilter(this);
    lblScore->setText("Score : "+QString::number(wGame->getScore()));
}
//-----------------------------------------------------------------------------
void CMainWindow::on_actQuitter_triggered() {
    QApplication::quit();
}
//-----------------------------------------------------------------------------
void CMainWindow::on_actNouveauJeu_triggered() {
    if(QMessageBox::question(this, "Confirmation", "Etes vous sûre de vouloir commencer une nouvelle partie ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        wGame->nouveau();
        lblScore->setText("Score : "+QString::number(wGame->getScore()));
    }
}
//-----------------------------------------------------------------------------
void CMainWindow::on_pbIA_clicked() {
    if(solveur) {
        solveur->stop();
    } else {
        solveur = new CSolveur(wGame->getGame());
        connect(solveur, &CSolveur::coupChoisi, this, &CMainWindow::onCoupChoisi);
        connect(solveur, &QThread::finished, this, &CMainWindow::onSolveurFini);
        pbIA->setText("Stop");
        solveur->start();
    }
}
//-----------------------------------------------------------------------------
void CMainWindow::onCoupChoisi(CDeplacement::EDirection direction) {
    CGame::EResultat resultat;

    switch(direction) {
    case CDeplacement::edHaut:    resultat = wGame->haut(false);   break;
    case CDeplacement::edDroite:  resultat = wGame->droite(false); break;
    case CDeplacement::edBas:     resultat = wGame->bas(false);    break;
    case CDeplacement::edGauche:  resultat = wGame->gauche(false); break;
    default: return;
    }

    lblScore->setText("Score : "+QString::number(wGame->getScore()));

    if(resultat == CGame::erPerdu || resultat == CGame::erFin || resultat == CGame::erGagne) {
        solveur->stop();
        return;
    }

    solveur->updateGame(wGame->getGame());
}
//-----------------------------------------------------------------------------
void CMainWindow::onSolveurFini() {
    solveur->deleteLater();
    solveur = nullptr;
    pbIA->setText("IA");
}
//-----------------------------------------------------------------------------
bool CMainWindow::eventFilter(QObject *obj, QEvent *event) {
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        bool bonneTouche = false;
        CGame::EResultat resultat;

        switch(keyEvent->key()) {
            case Qt::Key_Up:
                resultat = wGame->haut();
                bonneTouche = true;
                break;
            case Qt::Key_Right:
                resultat = wGame->droite();
                bonneTouche = true;
                break;
            case Qt::Key_Down:
                resultat = wGame->bas();
                bonneTouche = true;
                break;
            case Qt::Key_Left:
                resultat = wGame->gauche();
                bonneTouche = true;
                break;
            default:
                break;
        }

        if(bonneTouche) {
            lblScore->setText("Score : "+QString::number(wGame->getScore()));

            switch(resultat) {
            case CGame::erGagne:
                QMessageBox::information(this, "Gagné", "Félicitation, vous avez gangé !", QMessageBox::Ok);
                break;
            case CGame::erPerdu:
                QMessageBox::information(this, "Perdu", "Vous avez perdu, re-tenter votre chance !", QMessageBox::Ok);
                wGame->nouveau();
                lblScore->setText("Score : "+QString::number(wGame->getScore()));
                break;
            case CGame::erFin:
                QMessageBox::information(this, "Fin de partie", "Votre partie est terminé !", QMessageBox::Ok);
                break;
            default:
                break;
            }
            return true;
        }
    }

    return QObject::eventFilter(obj, event);
}
//-----------------------------------------------------------------------------
