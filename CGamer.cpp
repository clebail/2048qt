#include <QtDebug>
#include "CGamer.h"

#define NB_INPUT                4
#define NB_INPUT_NEURONE        4
#define NB_OUTPUT_NEURONE       4

CGamer::CGamer(CWGame *game) {
    QList<int> layer1Inputs;
    int i;

    this->game = game;

    alive= true;
    gagne = false;

    deps = new CDeplacement*[NB_OUTPUT_NEURONE];
    deps[0] = new CDeplacementHaut();
    deps[1] = new CDeplacementDroite();
    deps[2] = new CDeplacementBas();
    deps[3] = new CDeplacementGauche();

    for(i=0;i<NB_INPUT_NEURONE;i++) {
        layer1Inputs << NB_INPUT;
    }

    perceptron = new CPerceptron(layer1Inputs, NB_INPUT_NEURONE, false);

    /*perceptron->addLayer(6, false);
    perceptron->addLayer(8, false);
    perceptron->addLayer(6, false);
    perceptron->addLayer(NB_OUTPUT_NEURONE, false);*/
}

CGamer::~CGamer(void) {
    int i;
    for(i=0;i<NB_OUTPUT_NEURONE;i++) {
        delete deps[i];
    }

    delete deps;
    delete perceptron;
}

void CGamer::init(void) {
    perceptron->init();
}

void CGamer::joue(void) {
    int max = 0;
    int nbFusion;
    int nbVide;
    int nbFusionPossible;
    int idxMax = -1;
    int i;
    QList<QList<double> > inputs;
    QList<double> in[NB_INPUT_NEURONE];
    QList<double> values;

    for(i=0;i<NB_INPUT_NEURONE;i++) {
        analyse(deps[i], max, nbFusion, nbVide, nbFusionPossible);

        in[i] << (double)max;
        in[i] << (double)nbFusion;
        in[i] << (double)nbVide;
        in[i] << (double)nbFusionPossible;

        inputs << in[i];
    }

    values = perceptron->eval(inputs);
	
    //qDebug() << in[0] << in[1] << in[2] << in[3] << values;

    max = 0;
    for(i=0;i<NB_OUTPUT_NEURONE;i++) {
        if(deps[i]->canGo(game->getCases())) {
            if(idxMax == -1 || values.at(i) > max) {
                max = values.at(i);
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
        //score = 0;
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
        score = game->getScore() * 10 + game->getSomme();
    }
    return score;
}

int CGamer::get2048Score(void) {
    return game->getScore();
}

void CGamer::start(int value) {
    alive = true;
    score = -1;
    nbCoup = 0;
    game->nouveau(false);
}

void CGamer::from(CGamer *g1, CGamer *g2) {
    perceptron->from(g1->perceptron, g2->perceptron);
}

void CGamer::analyse(CDeplacement *dep, int& max, int& nbFusion, int& nbVide, int &nbFusionPossible) {
    TCases grille;
    int i;

    memcpy(grille, game->getCases(), CASE * sizeof(SCase));

    max = 0;
    dep->deplacement(grille, max, true);
    nbFusionPossible = dep->getNbFusionPossible(grille);

    nbFusion = 0;
    nbVide = 0;

    for(i=0;i<CASE;i++) {
        if(grille[i].fusion) {
            nbFusion++;
        } else if(grille[i].valeur == 0) {
            nbVide++;
        }
    }
}
