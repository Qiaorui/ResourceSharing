//
// Created by qiaorui on 7/06/15.
//

#include <iostream>
#include <string.h>
#include "Preflow.h"

Preflow::Preflow(int n, int m, int s, int t) {
    this->n = n;
    this->m = m;
    this->s = s;
    this->t = t;
    totalNode = n+m+2;
    capacidad = new int*[totalNode];
    for (int i = 0; i < totalNode; ++i) {
        capacidad[i] = new int[totalNode];
    }

    for (int i = 0; i < totalNode; ++i) {
        for (int j = 0; j < totalNode; ++j) {
            capacidad[i][j] = 0;
        }
    }

    flow = new int*[totalNode];
    for (int i = 0; i < totalNode; ++i) {
        flow[i] = new int[totalNode];
    }

    for (int i = 0; i < totalNode; ++i) {
        for (int j = 0; j < totalNode; ++j) {
            flow[i][j] = 0;
        }
    }

    h = new int[totalNode];
    e = new int[totalNode];
    for (int i = 0; i < totalNode; ++i) {
        h[i] = 0;
        e[i] = 0;
    }
    neighbour.resize(totalNode);
    parent = new int[totalNode];
}

Preflow::~Preflow() {
    for (int i = 0; i < totalNode; ++i) {
        delete[] capacidad[i];
    }
    delete[] capacidad;
    for (int i = 0; i < totalNode; ++i) {
        delete[] flow[i];
    }
    delete[] flow;
    delete[] parent;
    delete[] h;
    delete[] e;
}




inline void Preflow::initPreflow() {

    int c;
    //cout << "init Preflow" << endl;
    for (int i = 0; i < n; ++i) {
        c = capacidad[0][i+1] - flow[0][i+1];
        //cout << "c=" << c << "     c:"<< capacidad[0][i+1] << "    f:" << flow[0][i+1] << endl;
        flow[0][i+1] += c;
        flow[i+1][0] -= c;
        e[0] -=c;
        e[i+1]+=c;

        //cout << "flow [0]["<<i+1<<"]=" << flow[0][i+1] << "   flow[i+1][0]="<< flow[i+1][0] << endl;
        //cout << "e[" << i+1 << "] =" << e[i+1] << endl;
    }
}



inline void Preflow::push(int u, int v) {
    int f = min(e[u], capacidad[u][v] - flow[u][v]);

    //cout << "push: " << u << " to " << v << "   flow:" << f << endl;
    flow[u][v] += f;
    flow[v][u] -= f;
    e[u] -= f;
    e[v] += f;

}

inline bool Preflow::relable(int u) {
    int height = INF;
    for (int i = 0; i < neighbour[u].size(); ++i) {
        int v = neighbour[u][i];
        if (h[v] < height && capacidad[u][v]-flow[u][v]>0) {
            height = h[v];
        }
    }
    if (height == INF) {
        //cout << "relable: " << u << "-> h[" << u << "] = " << h[u] << " cant relable " <<endl;
        return false;
    }
    h[u] = height + 1;
    //cout << "relable: " << u << "-> h[" << u << "] = " << h[u] << endl;
    return true;

}


void Preflow::solve() {
    queue<int> activeNode;
    bool inQueue[totalNode];
    memset(inQueue,false,sizeof(inQueue));
    int u,v;
    bool canPush;
    initPreflow();
    for (int i = 1; i < n+1; ++i) {
        if (e[i]> 0) {
            activeNode.push(i);
            inQueue[i] = true;
        }

    }
    while (!activeNode.empty()) {
        u = activeNode.front();
        //cout << u << " -> " << endl;
        canPush = false;

        for (int i = 0; i < neighbour[u].size() && e[u]>0; ++i) {
            v = neighbour[u][i];
            //cout << v << " r:" << capacidad[u][v]-flow[u][v] << "  h[" << v << "]: " << h[v] << "  e[" << u << "]:" << e[u] << endl;
            if (capacidad[u][v]>flow[u][v] && h[u]==h[v]+1 && e[u]>0){
                push(u,v);
                canPush = true;
                if (e[v]>0 && v != s && v != t && !inQueue[v]) {
                    activeNode.push(v);
                    inQueue[v] = true;
                }
                if (e[u] == 0) {
                    activeNode.pop();
                    inQueue[u] = false;
                }
            }
        }
        if (!canPush) {
            if (!relable(u)){
                break;
            }
        }
    }

    maxFlow = e[t];


}

void Preflow::addEdge(int s, int t, int c) {
    capacidad[s][t] = c;
}

int Preflow::setEdge(char **matrix, int s[], char key) {
    for (int i = 0; i < totalNode ; ++i) {
        e[i] = 0;
    }
    h[this->s] = totalNode;
    h[t]=0;
    int reserved = 0;
    for (int j = 0; j < m; ++j) {
        capacidad[n+j+1][t] = 1;
        h[n+j+1] = 1;
        flow[n+j+1][t] = 0;
        flow[t][n+j+1] = 0;
        neighbour[n+j+1].push_back(t);
        neighbour[t].push_back(n+j+1);
    }
    for (int i = 0; i < n; ++i) {
        h[i+1] = 2;
        capacidad[0][i+1] = s[i];
        capacidad[i+1][0] = -s[i]; //preflow!!!!!!
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
                    //capacidad[0][i+1]--;
                    h[n+j+1] = INF;
                    ++flow[0][i+1];
                    --flow[i+1][0];
                    flow[i+1][n+j+1] = 1;
                    flow[n+j+1][i+1] = -1;
                    flow[n+j+1][t] = 1;
                    flow[t][n+j+1] = -1;
                    e[t]++;
                    //--e[i+1];
                    //neighbour[i+1].push_back(n+j+1);
                    //neighbour[n+j+1].push_back(i+1);
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




int Preflow::getMaxFlow() {
    //cout << "maxH:" << h[t] << endl;
    return maxFlow;
}


void Preflow::coutAssign() {
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

void Preflow::coutCapacity() {

    for (int i = 0; i < totalNode ; ++i) {
        for (int j = 0; j < totalNode; ++j) {
            cout << capacidad[i][j] << "  " ;
        }
        cout << endl;
    }

}


bool Preflow::isCorrect(int minS[], int maxS[], char **matrix, char key) {
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

bool Preflow::filled() {
    for (int i = 0; i < n; ++i) {
        if (capacidad[0][i+1] - flow[0][i+1] > 0) {
            return false;
        }
    }
    return true;
}












