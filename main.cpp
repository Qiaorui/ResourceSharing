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

double time_total;
double time_calS;
double time_read;
double time_sap;


inline double diffclock(clock_t clock1,clock_t clock2)
{
    double diffticks=clock1-clock2;
    double diffms=diffticks/(CLOCKS_PER_SEC/1000);
    return diffms;
}


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


inline void buildModel(string source) {


    clock_t read_start = clock();
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
            //cout << matrix[i][j] << " ";
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
        //cout << endl;
    }
    fin.close();

    clock_t calS_start = clock();
    time_read += diffclock(calS_start,read_start);
    int minS[n];
    //int debt = 0;
    //int sumMin = 0;
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
        /*if (sumPreference[i] > minS[i]) {
            debt += sumPreference[i] - minS[i];
        }*/
    }
    //analisi
    char minor = '9' ;
    for (int j = 0; j < m; ++j) {
        if (majorPreference[j] < minor) {
            minor = majorPreference[j];
        }
    }
    //cout << "minor = " << minor;
    if (minor == '0') {
        bestCase = 'D';
    } else if (minor == '1') {
        bestCase = 'C';
    }
    //cout << "This best case will be " << bestCase << endl;


/*
    for (int i = 0; i < n; ++i) {
        cout << "minS[" << i << "]: " << minS[i]<< "           ";
        cout << "maxS[" << i << "]: " << maxS[i]<< endl;
    }
*/
    clock_t sap_start = clock();
    time_calS += diffclock(sap_start,calS_start);
    Sap sap(n,m,0,n+m+1);
    while (bestCase != 'D') {
        int reserved = sap.setEdge(matrix, minS, bestCase);
        sap.solve();

        cout << "bestcase = " << bestCase << "    maxflow = " << sap.getMaxFlow() << "    reserved: " << reserved  <<  endl;

        if (!sap.filled()) {
            ++bestCase;
            cout << "lower case into ->" << bestCase << endl;
            continue;
        }
        for (int i = 0; i < n; ++i) {
            if (maxS[i] != minS[i]) {
                sap.addEdge(0,i+1,maxS[i]);
            }
        }
        sap.solve();
        cout << "maxflow = " << sap.getMaxFlow() << endl;
        if (sap.getMaxFlow() + reserved == m) {
            break;
        } else {
            ++bestCase;
            cout << "lower case into ->" << bestCase << endl;
        }
    }
    /*
    for (int i = 0; i < n; ++i) {
        sap.setEdge(0,i+1,minS[i]);
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] > '1') {
                sap.setEdge(i+1,n+1+j,1);
            }
        }
    }
    for (int j = 0; j < m; ++j) {
        sap.setEdge(n+j+1,n+m+1,1);
    }
     */
    
    if (bestCase != 'D') {
        if (!sap.isCorrect(minS, maxS, matrix, bestCase)) {
            cout << "Wrong!" << endl;
            exit(1);
        }
    }
     
    cout << bestCase << endl;
    sap.coutAssign();
    cout << "maxflow = " << sap.getMaxFlow() << endl;
    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
    time_sap += diffclock(clock(),sap_start);
}

inline void buildPreflowModel(string source) {


    clock_t read_start = clock();
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
            //cout << matrix[i][j] << " ";
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
        //cout << endl;
    }
    fin.close();

    clock_t calS_start = clock();
    time_read += diffclock(calS_start,read_start);
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
    //analisi
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


/*
    for (int i = 0; i < n; ++i) {
        cout << "minS[" << i << "]: " << minS[i]<< "           ";
        cout << "maxS[" << i << "]: " << maxS[i]<< endl;
    }
*/
    clock_t sap_start = clock();
    time_calS += diffclock(sap_start,calS_start);
    Preflow preflow(n,m,0,n+m+1);
    while (bestCase != 'D') {
        int reserved = preflow.setEdge(matrix, minS, bestCase);
        for (int i = 0; i < n; ++i) {
            if (maxS[i] != minS[i]) {
                preflow.addEdge(0,i+1,maxS[i]);
            }
        }
       /* preflow.solve();
        cout << "bestcase = " << bestCase << "    maxflow = " << preflow.getMaxFlow() << "    reserved: " << reserved  << endl;
        preflow.coutAssign();*/
        /*if (!preflow.filled()) {
            ++bestCase;
            cout << "1.lower case into ->" << bestCase << endl;
            continue;
        }*/
        /*for (int i = 0; i < n; ++i) {
            if (maxS[i] != minS[i]) {
                preflow.addEdge(0,i+1,maxS[i]);
            }
        }*/
        preflow.solve();
        //cout << "maxflow = " << preflow.getMaxFlow() << endl;
        if (preflow.getMaxFlow()  == m) {
            break;
        } else {
            ++bestCase;
            cout << "2. lower case into ->" << bestCase << endl;
        }
    }
    cout << bestCase << endl;
    //preflow.coutAssign();
    if (bestCase != 'D') {
        if (!preflow.isCorrect(minS, maxS, matrix, bestCase)) {
            cout << "Wrong!" << endl;
            exit(1);
        }
    }



    //cout << "maxflow = " << preflow.getMaxFlow() << endl;
    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
    time_sap += diffclock(clock(),sap_start);
}


int main(int argc, char * argv[]) {
    if (argc < 2) {
        cout << "Usage: program [-file  file_name | -dir directory_name | -input]" << endl
                << "************************************************************" << endl
                << "-file: you can read input data from file" << endl
                << "-dir:  you can read input data from each file of directory" << endl
                << "-input: you can read input data with standard input stream" << endl;
        exit(0);
    }
    time_read = 0;
    time_calS = 0;
    time_sap = 0;
    clock_t start = clock();
    vector<string> files;
    string dir = "";

    if (!strcmp(argv[1], "-file")) {
        files.push_back(argv[2]);
    } else if (!strcmp(argv[1], "-dir")) {
        dir.assign(argv[2]);
        dir += "/";
        files =  listFile(argv[2]);
        sort(files.begin(),files.end());
    }
    for (int i = 0; i < files.size(); ++i) {
        cout << files[i] << endl;
        buildPreflowModel(dir+files[i]);
    }
    time_total = diffclock(clock(),start);
    cout << "time for read input:   " << time_read << " ms      ratio:" << time_read/time_total*100 << "%" << endl;
    cout << "time for calc S:   " << time_calS << " ms      ratio:" << time_calS/time_total*100 << "%" << endl;
    cout << "time for sap algoritm:   " << time_sap << " ms      ratio:" << time_sap/time_total*100 << "%" << endl;
    cout << "time total:   " << time_total << " ms" << endl;
    exit(0);
}
