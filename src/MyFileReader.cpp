#include <iostream>
#include <fstream>
#include <sstream>

#include "MyFileReader.h"

using namespace std;

MyFileReader::MyFileReader(string _fileName, int _dumpedLines) :
        fileName(_fileName), dumpedLines(_dumpedLines), values(0), linesRead(-1), columnsRead(-1) {
    cout << "constructed - " << fileName << endl;
}

MyFileReader::~MyFileReader() {
}

double **MyFileReader::read() {

    double **values;
    double tempArray[128][128];

    string line;
    ifstream myfile(fileName.c_str());

    // bypass [dump_lines] lines
    for (int i = 0; i < dumpedLines; i++)
        getline(myfile, line);

    while (getline(myfile, line)) {
        columnsRead = -1;

        istringstream iss(line);
        while (iss >> tempArray[linesRead][++columnsRead]);

        linesRead++;
    }

    myfile.close();

    // initialise with the right sizes
    values = new double *[linesRead];
    for (int i = 0; i < linesRead; i++)
        values[i] = new double[columnsRead];

    // initialise with the right values
    for (int i = 0; i < linesRead; i++)
        for (int j = 0; j < columnsRead; j++)
            values[i][j] = tempArray[i][j];

    return values;
}

double **MyFileReader::newRead() {

    double **values;
    double tempArray[128][128];

    string line;
    string word;

    ifstream myfile(fileName.c_str());

    int columnTemp = 0;
    int lineTemp = 0;

    bool isDone = false;

    // bypass [dump_lines] lines
    for (int i = 0; i < dumpedLines; i++)
        getline(myfile, line);

    while (getline(myfile, line) && !isDone) {
        columnTemp = 0;

        istringstream iss(line);
        while (iss >> word && !isDone)
            // first read the arguments
            if (word == "#") {
                while (iss >> word)
                    if (word == "entries:")
                        iss >> linesRead;
                    else if (word == "objectives:")
                        iss >> columnsRead;
            }// then, if arguments are set, read the values
            else if (linesRead != -1 && columnsRead != -1) {
                tempArray[lineTemp][columnTemp++] = atof(word.c_str());
                while (columnTemp < columnsRead
                       && lineTemp < linesRead
                       && iss >> tempArray[lineTemp][columnTemp++]);
                lineTemp++;

                // have already read what i need
                if (lineTemp >= linesRead)
                    isDone = true;

                break;
            }
    }

    for (int k = 0; k < linesRead; ++k) {
        for (int i = 0; i < columnsRead; ++i) {
            cout << tempArray[k][i] << " ";
        }
        cout << endl;
    }

    myfile.close();

    // initialise with the right sizes
    values = new double *[linesRead];
    for (int i = 0; i < linesRead; i++)
        values[i] = new double[columnsRead];

    // initialise with the right values
    for (int i = 0; i < linesRead; i++)
        for (int j = 0; j < columnsRead; j++)
            values[i][j] = tempArray[i][j];

    return values;
}
