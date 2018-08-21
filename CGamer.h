#ifndef CGAMER_H
#define CGAMER_H

#include "CWGame.h"
#include "CNeurone.h"

#define NB_NEURONE          4

class CGamer {
public:
    CGamer(CWGame *game);
    void init(void);
    void joue(const TCases& cases);
    bool isGagne(void) const;
    bool isAlive(void) const;
    int getScore(void);
    void start(void);
private:
    CWGame *game;
    CNeurone neurones[NB_NEURONE];
    bool gagne;
    bool alive;
};

#endif // CGAMER_H
