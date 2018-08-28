#include <QtDebug>
#include "CResolutionDialog.h"

CResolutionDialog::CResolutionDialog(QWidget *parent) : QDialog(parent) {
    int i;
    int x, y;
    QGridLayout *layout;
    setupUi(this);

    layout = static_cast<QGridLayout *>(grid->layout());

    for(y=i=0;y<NB_LIGNE;y++) {
        for(x=0;x<NB_COLONNE;x++, i++) {
            games[i] = new CWGame(this);

            layout->addWidget(games[i], y, x);
        }
    }

    genetic = 0;
}

CResolutionDialog::~CResolutionDialog(void) {
    int i;

    for(i=0;i<NB_GAME;i++) {
        delete games[i];
    }
}

void CResolutionDialog::on_pbGo_clicked(bool) {
    genetic = new CGenetic(games, NB_GAME);

    genetic->start();
}

void CResolutionDialog::on_pbPause_clicked(bool) {
    if(genetic != 0) {
        genetic->togglePause();
    }
}
