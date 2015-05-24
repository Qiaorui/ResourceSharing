//
// Created by qiaorui on 20/05/15.
//

#include <iostream>
#include "Sap.h"


Sap::Sap() {

}

Sap::~Sap() {
    for (int i = 0; i < totalNode; ++i) {
        delete[] capacidad[i];
    }
    delete[] capacidad;
    delete[] parent;
}

void Sap::setEdge(int s, int t, int c) {
    capacidad[s][t] = c;
}

void setEdge(int **matrix) {

}

void Sap::solve() {

}

int Sap::getMaxFlow() {
    return maxFlow;
}

void Sap::setValue(int n, int m, int s, int t) {
    maxFlow = 0;
    this->n = n;
    this->m = m;
    this->s = s;
    this->t = t;
    totalNode = n+m+2;
    capacidad = new int*[totalNode];
    for (int i = 0; i < totalNode; ++i) {
        capacidad[i] = new int[totalNode];
    }
    neighbour.resize(totalNode);
    parent = new int[totalNode];
}



void Sap::coutCapacity() {

    for (int i = 0; i < totalNode ; ++i) {
        for (int j = 0; j < totalNode; ++j) {
            cout << capacidad[i][j] << "  " ;
        }
        cout << endl;
    }

}
