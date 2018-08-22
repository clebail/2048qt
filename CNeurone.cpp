#include <QtGlobal>
#include "CNeurone.h"

void CNeurone::init(void) {
    int i;

    for(i=0;i<=CASE;i++) {
        initGene(i);
    }
}

int CNeurone::eval(const TCases& cases) const {
    int i;
    double s = genes[0];

    for(i=1;i<=CASE;i++) {
        s += (double)cases[i-1].valeur * genes[i];
    }

    return (int)qMax(0.0, s);
}

void CNeurone::from(const CNeurone& n1, const CNeurone& n2, int seuil) {
    int i;
    CNeurone *src = (CNeurone *)&n1;

    for(i=0;i<=CASE;i++) {
        if(i == seuil) {
            src = (CNeurone *)&n2;
        }

        genes[i] = src->genes[i];
    }
}

void CNeurone::mute(int idx) {
    if(idx >= 0 && idx <= CASE) {
        initGene(idx);
    }
}

void CNeurone::initGene(int idx) {
    genes[idx] = ((double)((rand() % 50) - 25)) + ((double)((rand() % 1000)) / 1000.0);
}
