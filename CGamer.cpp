#include "CGamer.h"

CGamer::CGamer(CWGame *game) {
    this->game = game;

    alive= true;
    gagne = false;
}

void CGamer::init(void) {
    int i;

    for(i=0;i<NB_NEURONE;i++) {
        neurones[i].init();
    }
}

void CGamer::joue(const TCases& cases) {
    int values[NB_NEURONE];
    int max = 0;
    int idxMax = -1;
    int i;
    CWGame::EResultat resultat = CWGame::erNone;

    for(i=0;i<NB_NEURONE;i++) {
        values[i] = neurones[i].eval(cases);
        if(idxMax == -1 || values[i] > max) {
            max = values[i];
            idxMax = i;
        }
    }

    switch(idxMax) {
    case 0:
        resultat = game->haut();
        break;
    case 1:
        resultat = game->droite();
        break;
    case 2:
        resultat = game->bas();
        break;
    case 3:
        resultat = game->gauche();
        break;
    }

    gagne = game->getScore() >= 2048;
    alive = (!game->perdu() && resultat == CWGame::erMove);
}

bool CGamer::isGagne(void) const {
    return gagne;
}

bool CGamer::isAlive(void) const {
    return alive;
}

int CGamer::getScore(void) {
    return game->getScore();
}

void CGamer::start(void) {
    alive = true;
}
