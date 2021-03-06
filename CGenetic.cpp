#include <QtDebug>
#include <QTime>
#include "CGenetic.h"

bool lessThan(CGamer *g1, CGamer *g2) {
    return g1->getShareScore() > g2->getShareScore();
}

CGenetic::CGenetic(CWGame **games, int nbGame) {
    int i;

    this->games = games;
    this->nbGame = nbGame;
    for(i=0;i<nbGame;i++) {
        CGamer *gamer = new CGamer(games[i]);
        gamer->init();

        gamers.append(gamer);
    }

    inPause = false;
}

CGenetic::~CGenetic(void) {
    int i;

    for(i=0;i<nbGame;i++) {
        delete gamers.at(i);
    }
}

void CGenetic::run(void) {
    int i;
    int nbAlive = nbGame;
    bool fini = false;
    int nb2048 = 0;
    QTime time;

    time.start();
    while(!fini) {
        nbAlive = nbGame;

        for(i=0;i<nbGame;i++) {
            gamers.at(i)->start();
        }

        while(nbAlive != 0) {
            if(!inPause) {


                for(i=0;i<nbGame;i++) {
                    usleep(1000);
                    if(gamers.at(i)->isAlive()) {
                        gamers.at(i)->joue();

                        if(!gamers.at(i)->isAlive()) {
                            nbAlive--;
                        }
                    }
                }
                //qDebug() << "";
            } else {
                usleep(100);
            }
        }

        partagePopulation();
        qSort(gamers.begin(), gamers.end(), lessThan);

        if(gamers.at(0)->isGagne()) {
            nb2048++;
        }else {
            nb2048 = 0;
        }

        qDebug() << (nb2048 > 0 ? QString(10, QChar('*')) : "") << "Max score" << gamers.at(0)->getScore() << gamers.at(0)->get2048Score();

        fini = (nb2048 == 10);

        if(!fini) {
            croise();
        }
    }

    qDebug() << time.toString("hh:mm:ss");
    qDebug() << gamers.at(0);
}

void CGenetic::togglePause(void) {
    inPause = !inPause;
}

void CGenetic::croise(void) {
    int i, ir, max;

    i = 1;
    ir = nbGame - 1;
    max = nbGame / 2;

    while(i < max) {
        croiseGamers(i-1, i, ir);
        croiseGamers(i, i-1, ir-1);

        i+=2;
        ir-=2;
    }
}

void CGenetic::croiseGamers(int i1, int i2, int ir) {
    gamers.at(ir)->from(gamers.at(i1),  gamers.at(i2));
}

void CGenetic::partagePopulation(void) {
    int i, j;
    double *mult = new double[nbGame];

    for(i=0;i<nbGame;i++) {
        mult[i] = 1.0;
    }

    for(i=0;i<nbGame;i++) {
        for(j=i+1;j<nbGame;j++) {
            int dist = gamers.at(i)->diff(gamers.at(j));

            if(dist < 10) {
                double m = (1.0 - (static_cast<double>(dist)) / 1000.0);
                mult[i] += m;
                mult[j] += m;
            }
        }

        gamers.at(i)->setShareScore(static_cast<int>(gamers.at(i)->getScore() / mult[i]));
    }

    delete[] mult;
}

