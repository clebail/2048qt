#ifndef CNEURONE_H
#define CNEURONE_H

#include "common.h"

#define NB_GENE         25

class CNeurone {
public:
    void init(void);
    int eval(int * inputs) const;
    void from(const CNeurone& n1, const CNeurone& n2, int seuil);
    void mute(int idx);
private:
    double genes[NB_GENE];

    void initGene(int idx);
};

#endif // CNEURONE_H
