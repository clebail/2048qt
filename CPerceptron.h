#ifndef CPERCEPTRON_H
#define CPERCEPTRON_H

#include <QList>
#include "CNeurone.h"

class CPerceptron {
public:
    CPerceptron(const QList<int>& nbInputs, int nbNeurone, bool useBiais);
    ~CPerceptron(void);
    void init(void);
    QList<double> eval(const QList<QList<double> >& inputs);
    void addLayer(int nbNeurone, bool useBiais);
    void from(CPerceptron * p1, CPerceptron * p2);
private:
    QList<QList<CNeurone *> > layers;
    bool useBiais;
};

#endif // CPERCEPTRON_H
