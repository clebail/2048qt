#include <QtGlobal>
#include "CNeurone.h"

void CNeurone::init(void) {
    int i;

    for(i=0;i<NB_GENE;i++) {
        initGene(i);
    }
}

int CNeurone::eval(int *inputs) const {
    int i;
    double s = genes[0];

    for(i=1;i<NB_GENE;i++) {
        s += (double)inputs[i-1] * genes[i];
    }

    return (int)qMax(0.0, s);
}

void CNeurone::from(const CNeurone& n1, const CNeurone& n2, int seuil) {
    int i;
    CNeurone *src = (CNeurone *)&n1;

    for(i=0;i<NB_GENE;i++) {
        if(i == seuil) {
            src = (CNeurone *)&n2;
        }

        genes[i] = src->genes[i];
    }
}

void CNeurone::mute(int idx) {
    if(idx >= 0 && idx < NB_GENE) {
        initGene(idx);
    }
}

void CNeurone::initGene(int idx) {
    genes[idx] = ((double)((rand() % 50) - 25)) + ((double)((rand() % 1000)) / 1000.0);
}
