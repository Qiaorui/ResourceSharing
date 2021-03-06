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
protected:
    int n;
    int m;
    int totalNode;
    int maxFlow;
    int s;
    int t;
    int *parent;
    int **capacidad;
    int **flow;
    Matrix neighbour;

public:

    Sap(int n, int m, int s, int t);

    ~Sap();

    void coutCapacity();

    void coutAssign();

    bool isCorrect(int minS[], int maxS[], char **matrix, char key);

    void addEdge(int s, int t, int c);



    int setEdge(char **matrix, int s[], char key);



    int getMaxFlow();

    bool filled();
    virtual int bfs();
    virtual void solve();
};


#endif //A_SAP_H
