//
// Created by qiaorui on 27/05/15.
//

#ifndef A_ISAP_H
#define A_ISAP_H
#include "Sap.h"

class Isap : public Sap {


public:
    Isap(int n, int m, int s, int t) : Sap(n, m, s, t) { }

private:
    virtual int bfs() override;

    virtual void solve() override;
};



#endif //A_ISAP_H
