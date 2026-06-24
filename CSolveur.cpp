//-----------------------------------------------------------------------------
#include <QtDebug>
#include "CSolveur.h"
//-----------------------------------------------------------------------------
CSolveur::CSolveur(const CGame &game, QObject *parent)
    : QThread(parent), game(game), arreter(false), gameUpdated(false) {
}
//-----------------------------------------------------------------------------
void CSolveur::stop() {
    arreter = true;
    attente.wakeAll();
}
//-----------------------------------------------------------------------------
void CSolveur::updateGame(const CGame &game) {
    mutex.lock();
    this->game = game;
    gameUpdated = true;
    attente.wakeOne();
    mutex.unlock();
}
//-----------------------------------------------------------------------------
void CSolveur::run() {
    while(!arreter && !game.perdu() && !game.isGagne()) {
        CDeplacement::EDirection dir = solve();

        if(arreter) break;

        emit coupChoisi(dir);

        mutex.lock();
        while(!gameUpdated && !arreter)
            attente.wait(&mutex);
        gameUpdated = false;
        mutex.unlock();
    }
}
//-----------------------------------------------------------------------------
CDeplacement::EDirection CSolveur::solve() {
    CDeplacement::EDirection d = (CDeplacement::EDirection)(rand() % 4);
    qDebug() << d;
    return d;
}
//-----------------------------------------------------------------------------
