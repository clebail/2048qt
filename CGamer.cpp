#include "CGamer.h"

CGamer::CGamer(CWGame *game) {
    this->game = game;

    alive= true;
    gagne = false;

    deps = new CDeplacement*[NB_NEURONE];
    deps[0] = new CDeplacementHaut();
    deps[1] = new CDeplacementDroite();
    deps[2] = new CDeplacementBas();
    deps[3] = new CDeplacementGauche();
}

CGamer::~CGamer(void) {
    int i;
    for(i=0;i<NB_NEURONE;i++) {
        delete deps[i];
    }

    delete deps;
}

void CGamer::init(void) {
    int i;

    for(i=0;i<NB_NEURONE;i++) {
        neurones[i].init();
    }
}

void CGamer::joue(void) {
    int values[NB_NEURONE];
    int max = 0;
    int idxMax = -1;
    int i;
    int inputs[NB_GENE-1];

    inputs[0] = game->getColonneMax(0);
    inputs[1] = game->getColonneMax(1);
    inputs[2] = game->getColonneMax(2);
    inputs[3] = game->getColonneMax(3);
    inputs[4] = game->getLigneMax(0);
    inputs[5] = game->getLigneMax(1);
    inputs[6] = game->getLigneMax(2);
    inputs[7] = game->getLigneMax(3);
    inputs[8] = game->getColonneMaxApres(0, 1);
    inputs[9] = game->getColonneMaxApres(1, 1);
    inputs[10] = game->getColonneMaxApres(2, 1);
    inputs[11] = game->getColonneMaxApres(3, 1);
    inputs[12] = game->getColonneMaxApres(0, -1);
    inputs[13] = game->getColonneMaxApres(1, -1);
    inputs[14] = game->getColonneMaxApres(2, -1);
    inputs[15] = game->getColonneMaxApres(3, -1);
    inputs[16] = game->getLigneMaxApres(0, 1);
    inputs[17] = game->getLigneMaxApres(1, 1);
    inputs[18] = game->getLigneMaxApres(2, 1);
    inputs[19] = game->getLigneMaxApres(3, 1);
    inputs[20] = game->getLigneMaxApres(0, -1);
    inputs[21] = game->getLigneMaxApres(1, -1);
    inputs[22] = game->getLigneMaxApres(2, -1);
    inputs[23] = game->getLigneMaxApres(3, -1);

    for(i=0;i<NB_NEURONE;i++) {
        values[i] = neurones[i].eval(inputs);
        if(deps[i]->canGo(game->getCases())) {
            if(idxMax == -1 || values[i] > max) {
                max = values[i];
                idxMax = i;
            }
        }
    }

    switch(idxMax) {
    case 0:
        game->haut(false);
        break;
    case 1:
        game->droite(false);
        break;
    case 2:
        game->bas(false);
        break;
    case 3:
        game->gauche(false);
        break;
    case -1:
        alive = false;
        score = 0;
        return;
    }

    nbCoup++;
    gagne = game->getScore() >= 2048;
    alive = !gagne && !game->perdu();
}

bool CGamer::isGagne(void) const {
    return gagne;
}

bool CGamer::isAlive(void) const {
    return alive;
}

int CGamer::getScore(void) {
    if(score == -1) {
        score = game->getSomme();
    }
    return score;
}

void CGamer::start(void) {
    alive = true;
    score = -1;
    nbCoup = 0;
}

void CGamer::from(CGamer *g1, CGamer *g2) {
    int i;
    for(i=0;i<NB_NEURONE;i++) {
        int seuil = rand() % NB_GENE;

        neurones[i].from(g1->neurones[i], g2->neurones[i], seuil);
        neurones[i].mute(rand() % NB_GENE);
    }
}
