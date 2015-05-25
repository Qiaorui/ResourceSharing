//
// Created by qiaorui on 20/05/15.
//

#include <iostream>
#include <queue>
#include "Sap.h"


Sap::Sap() {

}

Sap::~Sap() {
    for (int i = 0; i < totalNode; ++i) {
        delete[] capacidad[i];
    }
    delete[] capacidad;
    for (int i = 0; i < totalNode; ++i) {
        delete[] flow[i];
    }
    delete[] flow;
    delete[] parent;
}

void Sap::addEdge(int s, int t, int c) {
    capacidad[s][t]++;
}

int Sap::setEdge(char **matrix, int s[], char key) {
    maxFlow = 0;
    int reserved = 0;
    for (int j = 0; j < m; ++j) {
        capacidad[n+j+1][t] = 1;
        flow[n+j+1][t] = 0;
        flow[t][n+j+1] = 0;
        neighbour[n+j+1].push_back(t);
        neighbour[t].push_back(n+j+1);
    }
    for (int i = 0; i < n; ++i) {
        capacidad[0][i+1] = s[i];
        flow[0][i+1] = 0;
        flow[i+1][0] = 0;
        neighbour[0].push_back(i+1);
        neighbour[i+1].push_back(0);
        for (int j = 0; j < m; ++j) {
            if (key == 'C') {
                if (matrix[i][j] > '0') {
                    capacidad[i+1][n+j+1] = 1;
                    flow[i+1][n+j+1] = 0;
                    flow[n+j+1][i+1] = 0;
                    neighbour[i+1].push_back(n+j+1);
                    neighbour[n+j+1].push_back(i+1);
                }
            } else if (matrix[i][j] > '1') {
                if (matrix[i][j] == '3' && key == 'A') {
                    capacidad[i+1][n+j+1] = 1;
                    flow[0][i+1] = 1;
                    flow[i+1][0] = -1;
                    flow[i+1][n+j+1] = 1;
                    flow[n+j+1][i+1] = -1;
                    flow[n+j+1][t] = 1;
                    flow[t][n+j+1] = -1;
                    ++reserved;
                } else {
                    capacidad[i+1][n+j+1] = 1;
                    flow[i+1][n+j+1] = 0;
                    flow[n+j+1][i+1] = 0;
                    neighbour[i+1].push_back(n+j+1);
                    neighbour[n+j+1].push_back(i+1);
                }
            }
        }
    }

    return reserved;

}


int Sap::bfs() {




    for (int i = 0; i < totalNode; ++i) {
        parent[i] = -1;
    }
    parent[s] = -2;
    int M[totalNode];
    M[s] = INF;
    queue<int> q;
    q.push(s);
    int u;
    while (!q.empty()) {
        u = q.front();
        //cout << "u->" << u << endl;
        q.pop();
        for (int i = 0; i < neighbour[u].size(); ++i) {
            int v = neighbour[u][i];
            if (h[u]+1 > h[v]) h[v] = h[u]+1;
            //cout << "v->" << v <<  "  R->" << capacidad[u][v] - flow[u][v] << "    P->" << parent[v] <<endl;
            if (capacidad[u][v] - flow[u][v] > 0 && parent[v] == -1) {
                parent[v] = u;
                M[v] = min(M[u],capacidad[u][v] - flow[u][v]);
                //cout << "M[v]" << M[v] << endl;
                if (v != t) {
                    q.push(v);
                } else {
                    //cout << "ok" << endl;
                    return M[t];
                }
            }
        }
    }
    return 0;
}

void Sap::solve() {
    int f,u,v;
    h.assign(totalNode,0);
    while(true) {
        f = bfs();
        //cout << "f->" << f << endl;
        if (f == 0) {
            break;
        }
        maxFlow += f;
        v = t;
        while(v != s) {
            u = parent[v];
            flow[u][v] += f;
            flow[v][u] -= f;
            v = u;
        }
    }
}

int Sap::getMaxFlow() {
    //cout << "maxH:" << h[t] << endl;
    return maxFlow;
}

void Sap::setValue(int n, int m, int s, int t) {
    this->n = n;
    this->m = m;
    this->s = s;
    this->t = t;
    totalNode = n+m+2;
    capacidad = new int*[totalNode];
    for (int i = 0; i < totalNode; ++i) {
        capacidad[i] = new int[totalNode];
    }
/*
    for (int i = 0; i < totalNode; ++i) {
        for (int j = 0; j < totalNode; ++j) {
            capacidad[i][j] = 0;
        }
    }
*/
    flow = new int*[totalNode];
    for (int i = 0; i < totalNode; ++i) {
        flow[i] = new int[totalNode];
    }

    for (int i = 0; i < totalNode; ++i) {
        for (int j = 0; j < totalNode; ++j) {
            flow[i][j] = 0;
        }
    }

    neighbour.resize(totalNode);
    parent = new int[totalNode];
}


void Sap::coutAssign() {
    bool find = false;
    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n; ++i) {
            if (flow[i+1][n+j+1] > 0) {
                cout << i << " ";
                break;
            }
        }
    }
    cout << endl;
}

void Sap::coutCapacity() {

    for (int i = 0; i < totalNode ; ++i) {
        for (int j = 0; j < totalNode; ++j) {
            cout << capacidad[i][j] << "  " ;
        }
        cout << endl;
    }

}


bool Sap::isCorrect(int minS[], int maxS[], char **matrix, char key) {
    //checking if A get all his preference
    if (key == 'A') {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (matrix[i][j] == '3' && flow[i+1][n+j+1] != 1) {
                    cout << "Fatal: matrix[" << i << "][" << j << "] = 3 and flow["<< i+1 << "][" << n+j+1 << "] =" << flow[i+1][n+j+1] << endl;
                    return false;
                }
            }
        }
    }


    //checking if each user has minS and maxS correct

    for (int i = 0; i < n; ++i) {
        if (flow[0][i+1] > maxS[i] || flow[0][i+1] < minS[i]) {
            cout << "Fatal: flow[0][" << i+1 << "] =" <<  flow[0][i+1] << "    when min="<< minS[i] << "   max=" << maxS[i] << endl;
            return false;
        }
    }


    return true;
}
