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

/*
 * Shortest Augmentation Path [Edmons and Karp, 1972].
 * Algoritmo de Preflow-Push ([Kleinberg and Tardos, 2005],
 * Cormen and Leiserton and Rivest, 1990
 *
  4 5
  2 2 1 0 0
  1 0 3 0 0
  2 2 1 3 2
  0 2 1 1 3

 * A
 * 0 2 1 2 3
 * 2 0 1 2 3
 *
 */


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

    Sap sap(n,m,0,n+m+1);
    while (bestCase != 'D') {
        int reserved = sap.setEdge(matrix, minS, bestCase);
        sap.solve();

        //cout << "bestcase = " << bestCase << "    maxflow = " << sap.getMaxFlow() << "    reserved: " << reserved  <<  endl;

        if (!sap.filled()) {
            ++bestCase;
            continue;
        }
        for (int i = 0; i < n; ++i) {
            if (maxS[i] != minS[i]) {
                sap.addEdge(0,i+1,maxS[i]);
            }
        }
        sap.solve();
        if (sap.getMaxFlow() + reserved == m) {
            break;
        } else {
            ++bestCase;
        }
    }

    /*
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
        //sumMin += minS[i];
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
        }
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



    //cout << "maxflow = " << preflow.getMaxFlow() << endl;
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

    bool sapFlag = (!strcmp(argv[1], "-sap"));

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
    clock_t start = clock();
    for (int i = 0; i < files.size(); ++i) {
        cout << files[i];
        if (sapFlag) {
            buildSapModel(dir+files[i]);
        } else {
            buildPreflowModel(dir+files[i]);
        }
        if (!strcmp(argv[2], "-benchmark") && (i+1)%10 == 0) {
            cout << "testType:" << (i+1)/90+2 << " - " << (i/10)%9 << " used "<< (double)(clock()-start)/CLOCKS_PER_SEC * 1000.0 << " ms" << endl;
            start = clock();
        }
    }

    exit(0);
}
