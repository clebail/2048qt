#ifndef CGENETIC_H
#define CGENETIC_H

#include <QThread>
#include <QList>
#include "CWGame.h"
#include "CGamer.h"

class CGenetic : public QThread {
public:
    CGenetic(CWGame **games, int nbGame);
    ~CGenetic(void);
    void run(void);
    void togglePause(void);
private:
    CWGame **games;
    int nbGame;
    QList<CGamer *> gamers;
    bool inPause;

    void croise(void);
    void croiseGamers(int i1, int i2, int ir);
    int genValue(void);
    void partagePopulation(void);
};

#endif // CGENETIC_H
