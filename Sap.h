//
// Created by qiaorui on 20/05/15.
//
#ifndef A_SAP_H
#define A_SAP_H
#include <vector>
using namespace std;

typedef vector< vector<int> > Matrix;
#define INF 1<<29;

class Sap {
private:
    int n;
    int m;
    int totalNode;
    int maxFlow;
    int s;
    int t;
    vector<int> h;
    int flowPath;
    int *parent;
    int  **capacidad;
    Matrix neighbour;

public:

    Sap();

    ~Sap();

    void setValue(int n, int m, int s, int t);

    void coutCapacity();

    void setEdge(int s, int t, int c);

    void setEdge(int **matrix);

    void solve();

    int getMaxFlow();



};


#endif //A_SAP_H
