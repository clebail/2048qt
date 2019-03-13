#ifndef CPERCEPTRON_H
#define CPERCEPTRON_H

#include <QList>
#include <QDataStream>
#include "CNeurone.h"

class CPerceptron {
public:
    CPerceptron(const QList<int>& nbInputs, int nbNeurone, bool useBiais);
    ~CPerceptron(void);
    void init(void);
    QList<double> eval(const QList<QList<double> >& inputs);
    void addLayer(int nbNeurone);
    void from(CPerceptron * p1, CPerceptron * p2);
    int diff(CPerceptron * other) const;
private:
    QList<QList<CNeurone *> > layers;
    bool useBiais;

    friend QDataStream& operator<<(QDataStream& out, const CPerceptron& percepton);
    friend QDataStream& operator>>(QDataStream& in, CPerceptron& percepton);
};

QDataStream& operator<<(QDataStream& out, const CPerceptron& percepton);
QDataStream& operator>>(QDataStream& in, CPerceptron& percepton);

#endif // CPERCEPTRON_H
