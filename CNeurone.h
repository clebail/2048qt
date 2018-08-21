#ifndef CNEURONE_H
#define CNEURONE_H

#include "common.h"

class CNeurone {
public:
    void init(void);
    int eval(const TCases& cases) const;
private:
    double genes[CASE+1];
};

#endif // CNEURONE_H
