#include <QtGlobal>
#include <math.h>
#include "CNeurone.h"

CNeurone::CNeurone(int nbGene, bool useBiais) {
    this->nbGene = nbGene;
    this->useBiais = useBiais;
    biais =initBiais();

    genes = new double[nbGene];
}

CNeurone::~CNeurone(void) {
    delete[] genes;
}

void CNeurone::init(void) {
    int i;

    for(i=0;i<nbGene;i++) {
        initGene(i);
    }
}

double CNeurone::eval(const QList<double> &inputs) const {
    int i;
    double s = biais;

    for(i=0;i<nbGene;i++) {
        s += genes[i] * inputs.at(i);
    }

    //return qMax(0.0, s);
    return 1.0 / (1.0 + exp(-s));
}

void CNeurone::from(CNeurone *n1, CNeurone *n2, int seuil) {
    int i;
    CNeurone *src = n1;

    for(i=0;i<nbGene;i++) {
        if(i == seuil) {
            src = n2;
        }

        genes[i] = src->genes[i];
    }

    biais = rand() % 2 ? n1->biais : n2->biais;
}

void CNeurone::mute(int idx) {
    if(rand() % 2) {
        biais = initBiais();
    }

    if(idx >= 0 && idx < nbGene) {
        initGene(idx);
    }
}

int CNeurone::getNbGene(void) const {
    return nbGene;
}

void CNeurone::initGene(int idx) {
    genes[idx] = ((double)((rand() % 101) - 50)) + ((double)((rand() % 1000)) / 1000.0) * (rand() % 2 ? 1 : -1);
}

int CNeurone::initBiais(void) {
    return useBiais ? rand() % 201 - 100 : 0;
}

