#include "CGenetic.h"

bool lessThan(CGamer *g1, CGamer *g2) {
    return g1->getScore() < g2->getScore();
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
            games[i]->nouveau();
            gamers.at(i)->start();
        }

        while(nbAlive != 0) {
            usleep(100);

            for(i=0;i<nbGame;i++) {
                if(gamers.at(i)->isAlive()) {
                    gamers.at(i)->joue(games[i]->getCases());

                    if(!gamers.at(i)->isAlive()) {
                        nbAlive--;
                    }
                }
            }
        }

        qSort(gamers.begin(), gamers.end(), lessThan);

        fini = gamers.at(0)->isGagne();
        if(!fini) {
            croise();
        }
    }
}

void CGenetic::croise(void) {

}
