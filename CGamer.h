#ifndef CGAMER_H
#define CGAMER_H

#include "CWGame.h"
#include "CNeurone.h"

#define NB_NEURONE          4

class CGamer {
public:
    CGamer(CWGame *game);
    ~CGamer(void);
    void init(void);
    void joue(const TCases& cases);
    bool isGagne(void) const;
    bool isAlive(void) const;
    int getScore(void);
    void start(void);
    void from(CGamer *g1, CGamer *g2);
private:
    CWGame *game;
    CNeurone neurones[NB_NEURONE];
    CDeplacement **deps;
    bool gagne;
    bool alive;
    int score;
};

#endif // CGAMER_H
