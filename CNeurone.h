#ifndef CNEURONE_H
#define CNEURONE_H

#include <QList>
#include <QDataStream>
#include "common.h"

class CNeurone {
public:
    CNeurone(int nbGene, bool useBiais);
    ~CNeurone(void);
    void init(void);
    double eval(const QList<double>& inputs) const;
    void from(CNeurone * n1, CNeurone * n2, int seuil);
    void mute(int idx);
    int getNbGene(void) const;
    int diff(CNeurone *other) const;
private:
    int nbGene;
    double *genes;
    bool useBiais;
    int biais;

    void initGene(int idx);
    int initBiais(void);

    friend QDataStream& operator<<(QDataStream& out, const CNeurone& neurone);
    friend QDataStream& operator>>(QDataStream& in, CNeurone& neurone);
};

QDataStream& operator<<(QDataStream& out, const CNeurone& neurone);
QDataStream& operator>>(QDataStream& in, CNeurone& neurone);

#endif // CNEURONE_H
