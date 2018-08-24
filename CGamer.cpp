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

    perceptron = new CPerceptron(layer1Inputs, NB_INPUT_NEURONE);

    perceptron->addLayer(6);
    perceptron->addLayer(NB_OUTPUT_NEURONE);
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
    int idxMax = -1;
    int i;
    QList<QList<double> > inputs;
    QList<double> in[NB_INPUT_NEURONE];
    QList<double> values;

    analyse(deps[0], max, nbFusion, nbVide);
    in[0] << (double)game->getScore();
    in[0] << (double)max;
    in[0] << (double)nbFusion;
    in[0] << (double)nbVide;

    analyse(deps[1], max, nbFusion, nbVide);
    in[1] << (double)game->getScore();
    in[1] << (double)max;
    in[1] << (double)nbFusion;
    in[1] << (double)nbVide;

    analyse(deps[2], max, nbFusion, nbVide);
    in[2] << (double)game->getScore();
    in[2] << (double)max;
    in[2] << (double)nbFusion;
    in[2] << (double)nbVide;

    analyse(deps[3], max, nbFusion, nbVide);
    in[3] << (double)game->getScore();
    in[3] << (double)max;
    in[3] << (double)nbFusion;
    in[3] << (double)nbVide;

    //qDebug() << in[0] << in[1] << in[2] << in[3];


    for(i=0;i<NB_INPUT_NEURONE;i++) {
        inputs << in[i];
    }

    values = perceptron->eval(inputs);
	
	//qDebug() << in[0] << in[1] << in[2] << in[3] << values;

    max = 0;
    for(i=0;i<NB_OUTPUT_NEURONE;i++) {
        if(deps[i]->canGo(game->getCases()) && values.at(i) != 0.0) {
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
    game->nouveau(false, &value);
}

void CGamer::from(CGamer *g1, CGamer *g2) {
    perceptron->from(g1->perceptron, g2->perceptron);
}

void CGamer::analyse(CDeplacement *dep, int& max, int& nbFusion, int& nbVide) {
    TCases grille;
    int i;

    memcpy(grille, game->getCases(), CASE * sizeof(SCase));

    dep->deplacement(grille, max, true);

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
