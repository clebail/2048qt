//-----------------------------------------------------------------------------
#include <QKeyEvent>
#include <QtDebug>
#include <QMessageBox>
#include "CMainWindow.h"
#include "ui_CMainWindow.h"
//-----------------------------------------------------------------------------
CMainWindow::CMainWindow(QWidget *parent) : QMainWindow(parent) {
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
bool CMainWindow::eventFilter(QObject *obj, QEvent *event) {
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        bool bonneTouche = false;
        CWGame::EResultat resultat;

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
            case CWGame::erGagne:
                QMessageBox::information(this, "Gagné", "Félicitation, vous avez gangé !", QMessageBox::Ok);
                break;
            case CWGame::erPerdu:
                QMessageBox::information(this, "Perdu", "Vous avez perdu, re-tenter votre chance !", QMessageBox::Ok);
                wGame->nouveau();
                lblScore->setText("Score : "+QString::number(wGame->getScore()));
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