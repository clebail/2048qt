#ifndef CGAMER_H
#define CGAMER_H

#include "CWGame.h"
#include "CPerceptron.h"

class CGamer {
public:
    CGamer(CWGame *game);
    ~CGamer(void);
    void init(void);
    void joue(void);
    bool isGagne(void) const;
    bool isAlive(void) const;
    int getScore(void);
    int get2048Score(void);
    void start(int value);
    void from(CGamer *g1, CGamer *g2);
private:
    CWGame *game;
    CPerceptron *perceptron;
    CDeplacement **deps;
    bool gagne;
    bool alive;
    int score;
    int nbCoup;

    void analyse(CDeplacement *dep, int& max, int& nbFusion, int& nbVide, int &nbFusionPossible);
};

#endif // CGAMER_H
