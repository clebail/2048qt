#include "CResolutionDialog.h"

CResolutionDialog::CResolutionDialog(QWidget *parent) : QDialog(parent) {
    setupUi(this);

    games[0] = wg0;
    games[1] = wg1;
    games[2] = wg2;
    games[3] = wg3;
    games[4] = wg4;
    games[5] = wg5;
    games[6] = wg6;
    games[7] = wg7;
    games[8] = wg8;
    games[9] = wg9;
    games[10] = wg10;
    games[11] = wg11;
    games[12] = wg12;
    games[13] = wg13;
    games[14] = wg14;
    games[15] = wg15;
}

void CResolutionDialog::on_pbGo_clicked(bool) {
    genetic = new CGenetic(games, NB_GAME);

    genetic->start();
}
