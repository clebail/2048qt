#include <QtDebug>
#include "CGenetic.h"

bool lessThan(CGamer *g1, CGamer *g2) {
    return g1->getScore() > g2->getScore();
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

    while(!fini) {
        nbAlive = nbGame;

        for(i=0;i<nbGame;i++) {
            games[i]->nouveau(false);
            gamers.at(i)->start();
        }

        while(nbAlive != 0) {
            usleep(100);

            for(i=0;i<nbGame;i++) {
                if(gamers.at(i)->isAlive()) {
                    gamers.at(i)->joue();

                    if(!gamers.at(i)->isAlive()) {
                        nbAlive--;
                    }
                }
            }
        }

        qSort(gamers.begin(), gamers.end(), lessThan);

        qDebug() << "Max score" << gamers.at(0)->getScore();

        fini = gamers.at(0)->isGagne();
        if(!fini) {
            croise();
        }
    }
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
