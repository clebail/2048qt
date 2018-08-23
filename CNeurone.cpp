#include <QtGlobal>
#include "CNeurone.h"

CNeurone::CNeurone(int nbGene) {
    this->nbGene = nbGene;

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
    double s = genes[0];

    for(i=1;i<nbGene;i++) {
        s += inputs.at(i-1) * genes[i];
    }

    return qMax(0.0, s);
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
}

void CNeurone::mute(int idx) {
    if(idx >= 0 && idx < nbGene) {
        initGene(idx);
    }
}

int CNeurone::getNbGene(void) const {
    return nbGene;
}

void CNeurone::initGene(int idx) {
    genes[idx] = ((double)((rand() % 21) - 10)) + ((double)((rand() % 1000)) / 1000.0);
}
