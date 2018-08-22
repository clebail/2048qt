#ifndef CNEURONE_H
#define CNEURONE_H

#include "common.h"

class CNeurone {
public:
    void init(void);
    int eval(const TCases& cases) const;
    void from(const CNeurone& n1, const CNeurone& n2, int seuil);
    void mute(int idx);
private:
    double genes[CASE+1];

    void initGene(int idx);
};

#endif // CNEURONE_H
