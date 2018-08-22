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

void CGamer::joue(const TCases& cases) {
    int values[NB_NEURONE];
    int max = 0;
    int idxMax = -1;
    int i;
    CWGame::EResultat resultat = CWGame::erNone;

    for(i=0;i<NB_NEURONE;i++) {
        values[i] = neurones[i].eval(cases);
        if(deps[i]->canGo(cases)) {
            if(idxMax == -1 || values[i] > max) {
                max = values[i];
                idxMax = i;
            }
        }
    }

    switch(idxMax) {
    case 0:
        resultat = game->haut(false);
        break;
    case 1:
        resultat = game->droite(false);
        break;
    case 2:
        resultat = game->bas(false);
        break;
    case 3:
        resultat = game->gauche(false);
        break;
    case -1:
        alive = false;
        score = 0;
        return;
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
    if(score == -1) {
        score = game->getScore();
    }
    return score;
}

void CGamer::start(void) {
    alive = true;
    score = -1;
}

void CGamer::from(CGamer *g1, CGamer *g2) {
    int i;
    for(i=0;i<NB_NEURONE;i++) {
        int seuil = rand() % (CASE + 1);

        neurones[i].from(g1->neurones[i], g2->neurones[i], seuil);
        neurones[i].mute(rand() % (CASE + 1));
    }

    score = 0;
}
