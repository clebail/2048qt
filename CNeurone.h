#ifndef CNEURONE_H
#define CNEURONE_H

#include <QList>
#include "common.h"

class CNeurone {
public:
    CNeurone(int nbGene);
    ~CNeurone(void);
    void init(void);
    double eval(const QList<double>& inputs) const;
    void from(CNeurone * n1, CNeurone * n2, int seuil);
    void mute(int idx);
    int getNbGene(void) const;
private:
    int nbGene;
    double *genes;

    void initGene(int idx);
};

#endif // CNEURONE_H
