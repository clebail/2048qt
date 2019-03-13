#ifndef CGAMER_H
#define CGAMER_H

#include <QDataStream>
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
    void start(void);
    void from(CGamer *g1, CGamer *g2);
    int diff(CGamer *other) const;
    void setShareScore(int score);
    int getShareScore(void) const;
private:
    CWGame *game;
    CPerceptron *perceptron;
    CDeplacement **deps;
    bool gagne;
    bool alive;
    int score;
    int nbCoup;
    int shareScore;

    friend QDataStream& operator<<(QDataStream& out, const CGamer * &gamer);
    friend QDataStream& operator>>(QDataStream& in, CGamer& gamer);
};

QDataStream& operator<<(QDataStream& out, const CGamer* &gamer);
QDataStream& operator>>(QDataStream& in, CGamer& gamer);

#endif // CGAMER_H
