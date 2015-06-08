//
// Created by qiaorui on 7/06/15.
//

#ifndef A_PREFLOW_H
#define A_PREFLOW_H
#include <vector>
#include <queue>
using namespace std;

typedef vector< vector<int> > Matrix;
#define INF 1<<29

class Preflow {
private:
    int n;
    int m;
    int totalNode;
    int maxFlow;
    int s;
    int t;
    int *parent;
    int **capacidad;
    int **flow;
    int *h;
    int *e;
    Matrix neighbour;

public:
    Preflow(int n, int m, int s, int t);

    ~Preflow();

    void push(int u, int v);

    bool relable(int u);

    void solve();

    void initPreflow();

    void coutCapacity();

    void coutAssign();

    bool isCorrect(int minS[], int maxS[], char **matrix, char key);

    void addEdge(int s, int t, int c);



    int setEdge(char **matrix, int s[], char key);


    int getMaxFlow();

    bool filled();



};


#endif //A_PREFLOW_H
