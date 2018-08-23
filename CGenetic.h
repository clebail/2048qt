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
private:
    CWGame **games;
    int nbGame;
    QList<CGamer *> gamers;

    void croise(void);
    void croiseGamers(int i1, int i2, int ir);
    int genValue(void);
};

#endif // CGENETIC_H
