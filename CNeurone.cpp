#include <QtGlobal>
#include "CNeurone.h"

void CNeurone::init(void) {
    int i;

    for(i=0;i<=CASE;i++) {
        genes[i] = ((double)((rand() % 50) - 25)) + ((double)((rand() % 1000)) / 1000.0);
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
