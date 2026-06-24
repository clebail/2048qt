//-----------------------------------------------------------------------------
#ifndef __CSOLVEUR_H__
#define __CSOLVEUR_H__
//-----------------------------------------------------------------------------
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <atomic>
#include "CGame.h"
//-----------------------------------------------------------------------------
class CSolveur : public QThread {
    Q_OBJECT
public:
    explicit CSolveur(const CGame &game, QObject *parent = nullptr);
    void stop();
    void updateGame(const CGame &game);

signals:
    void coupChoisi(CDeplacement::EDirection direction);

protected:
    void run() override;

private:
    CGame game;
    std::atomic<bool> arreter;
    QMutex mutex;
    QWaitCondition attente;
    bool gameUpdated;

    CDeplacement::EDirection solve();
    int expectIMax(const CGame &game, int depth = 0);
    int chanceNode(const CGame &game, int depth);
    int evaluate(const CGame &game);
};
//-----------------------------------------------------------------------------
#endif // __CSOLVEUR_H__
//-----------------------------------------------------------------------------
