#include <iostream>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Sap.h"
#include "Preflow.h"

using namespace std;

//
// Created by qiaorui on 20/05/15.
//


//list all files under directory
inline vector<string> listFile(char *dirname){
    vector<string> a;
    DIR *pDIR;
    struct dirent *entry;
    char* current_dir= get_current_dir_name();
    strcat(current_dir,"/");
    char* result = new char[strlen(current_dir)+strlen(dirname)+1];
    strcpy(result, current_dir);
    strcat(result, dirname);
    if( pDIR=opendir(result) ){
        while(entry = readdir(pDIR)){
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
                a.push_back(entry->d_name);
        }
        closedir(pDIR);
    }
    return a;
}


inline void buildSapModel(string source) {

    //open file
    char bestCase = 'A';
    int n;
    int m;
    const char *file = source.c_str();
    ifstream fin(file,ios::in|ios::binary);
    if(!fin.is_open()) {
        cout<<"Could not open the file "<<file<<endl;
        cout<<"Program terminating.\n";
        exit(EXIT_FAILURE);
    }
    fin >> n >> m;
    char **matrix;

    matrix = new char*[n]; //matrix is the input data
    for (int i = 0; i < n; ++i) {
        matrix[i] = new char[m];
    }

    char majorPreference[m]; //array that contains the major preference for each voyager
    memset(majorPreference, '0', sizeof(majorPreference));
    float numberOfPassengerByTrip[m]; //array that contains number of voyager for each travel
    memset(numberOfPassengerByTrip,0, sizeof(numberOfPassengerByTrip));
    int sumPreference[n]; //contains number of preferences == 3
    memset(sumPreference,0, sizeof(sumPreference));
    //reading data
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            fin >> matrix[i][j];
            if (matrix[i][j] != '0') {
                ++numberOfPassengerByTrip[j];
                if (bestCase == 'A' && matrix[i][j] == '3') {
                    if (majorPreference[j] == '3') { // if already exists a preference 3, then we have B case
                        bestCase = 'B';
                    }
                    ++sumPreference[i];
                }
                if (matrix[i][j] > majorPreference[j]) {
                    majorPreference[j] = matrix[i][j];
                }
            }
        }
    }
    fin.close();

    //calculing minS and maxS
    int minS[n];
    int maxS[n];
    float result;
    for (int i = 0; i < n; ++i) {
        result = 0;
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] != '0') {
                result += 1/numberOfPassengerByTrip[j];
            }
        }
        minS[i] = (int)result;
        if (minS[i] == result) {
            maxS[i] = minS[i];
        } else {
            maxS[i] = minS[i]+1;
        }
        if (sumPreference[i] > maxS[i]) { //if someone has more preference 3 than his maxS, then we get B case
            bestCase = 'B';
        }

    }
    char minor = '9' ; //minor preference of majors, if it's 0, we can sure is a D case. if 1,  C case
    for (int j = 0; j < m; ++j) {
        if (majorPreference[j] < minor) {
            minor = majorPreference[j];
        }
    }
    if (minor == '0') {
        bestCase = 'D';
    } else if (minor == '1') {
        bestCase = 'C';
    }

    //build Sap model
    Sap sap(n,m,0,n+m+1);
    while (bestCase != 'D') { //run solver until it's done or we get D case
        int reserved = sap.setEdge(matrix, minS, bestCase); //initialize with minS, reserved is number of preference 3
        sap.solve();
        if (!sap.filled()) { //if we have a maxFlow with minS, => we can complete minS condition, else ++Case
            ++bestCase;
            continue;
        }
        for (int i = 0; i < n; ++i) { //Now we have to increase Capacity to maxS.
            if (maxS[i] != minS[i]) {
                sap.addEdge(0,i+1,maxS[i]);
            }
        }
        sap.solve();
        if (sap.getMaxFlow() + reserved == m) { //if maxflow + reserved == M, then we get correct assignment
            break;
        } else {
            ++bestCase;
        }
    }

    /*  This is for test if answer is correct
    if (bestCase != 'D') {
        if (!sap.isCorrect(minS, maxS, matrix, bestCase)) {
            cout << "Wrong!" << endl;
            exit(1);
        }
    }*/
     
    cout << " " << bestCase << endl;
    //sap.coutAssign();
    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}





// See comment of buildSapModel, are almost same!
inline void buildPreflowModel(string source) {
    char bestCase = 'A';
    int n;
    int m;
    const char *file = source.c_str();
    ifstream fin(file,ios::in|ios::binary);
    if(!fin.is_open()) {
        cout<<"Could not open the file "<<file<<endl;
        cout<<"Program terminating.\n";
        exit(EXIT_FAILURE);
    }
    fin >> n >> m;
    char **matrix;
    matrix = new char*[n];
    for (int i = 0; i < n; ++i) {
        matrix[i] = new char[m];
    }
    char majorPreference[m];
    memset(majorPreference, '0', sizeof(majorPreference));
    float numberOfPassengerByTrip[m];
    memset(numberOfPassengerByTrip,0, sizeof(numberOfPassengerByTrip));
    int sumPreference[n];
    memset(sumPreference,0, sizeof(sumPreference));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            fin >> matrix[i][j];
            if (matrix[i][j] != '0') {
                ++numberOfPassengerByTrip[j];
                if (bestCase == 'A' && matrix[i][j] == '3') {
                    if (majorPreference[j] == '3') {
                        bestCase = 'B';
                    }
                    ++sumPreference[i];
                }
                if (matrix[i][j] > majorPreference[j]) {
                    majorPreference[j] = matrix[i][j];
                }
            }
        }
    }
    fin.close();

    int minS[n];

    int maxS[n];
    float result;
    for (int i = 0; i < n; ++i) {
        result = 0;
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] != '0') {
                result += 1/numberOfPassengerByTrip[j];
            }
        }
        minS[i] = (int)result;
        if (minS[i] == result) {
            maxS[i] = minS[i];
        } else {
            maxS[i] = minS[i]+1;
        }
        if (sumPreference[i] > maxS[i]) {
            bestCase = 'B';
        }

    }
    char minor = '9' ;
    for (int j = 0; j < m; ++j) {
        if (majorPreference[j] < minor) {
            minor = majorPreference[j];
        }
    }
    if (minor == '0') {
        bestCase = 'D';
    } else if (minor == '1') {
        bestCase = 'C';
    }


    Preflow preflow(n,m,0,n+m+1);

    while (bestCase != 'D') {
        preflow.setEdge(matrix, minS, bestCase);
        for (int i = 0; i < n; ++i) {
            if (maxS[i] != minS[i]) {
                preflow.addEdge(0,i+1,maxS[i]);
            }
        }//for preflow, we can directly use maxS.
        preflow.solve();
        if (preflow.getMaxFlow()  == m) {
            break;
        } else {
            ++bestCase;
        }
    }
    cout << " " << bestCase << endl;
    //preflow.coutAssign();
    /*if (bestCase != 'D') {
        if (!preflow.isCorrect(minS, maxS, matrix, bestCase)) {
            cout << "Wrong!" << endl;
            exit(1);
        }
    }*/

    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}


int main(int argc, char * argv[]) {
    if (argc < 2) {
        cout << "Usage: program [-sap | -pre] [-file  file_name | -dir directory_name | -benchmark]" << endl
                << "************************************************************" << endl
                << "-sap: using sap algorithm" << endl
                << "-pre: using preflow algorithm" << endl
                << "-file: you can read input data from file" << endl
                << "-dir:  you can read input data from each file of directory" << endl
                << "-benchmark: run benchmark testing using default files" << endl;
        exit(0);
    }
    vector<string> files;
    string dir = "";

    bool sapFlag = (!strcmp(argv[1], "-sap")); //Flag if you use sap or pre

    //reading files
    if (!strcmp(argv[2], "-file")) {
        files.push_back(argv[3]);
    } else if (!strcmp(argv[2], "-dir")) {
        dir.assign(argv[3]);
        dir += "/";
        files =  listFile(argv[3]);
        sort(files.begin(),files.end());
    } else if (!strcmp(argv[2], "-benchmark")) {
        dir="benchmark/";
        string f;
        for (int i = 0; i < 720; ++i) {
            cin >> f;
            files.push_back(f);
        }
    }

    //solving problems
    clock_t start = clock();
    for (int i = 0; i < files.size(); ++i) {
        cout << files[i];
        if (sapFlag) {
            buildSapModel(dir+files[i]); //Using sap
        } else {
            buildPreflowModel(dir+files[i]); // Or using preflow
        }
        if (!strcmp(argv[2], "-benchmark") && (i+1)%10 == 0) {
            cout << "testType:" << (i+1)/90+2 << " _ " << (i/10)%9 << " average used: "<< (double)(clock()-start)/CLOCKS_PER_SEC * 100.0 << " ms" << endl;
            start = clock();
        }
    }

    exit(0);
}
