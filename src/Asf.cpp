/*
 * MyCalculator.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: user
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

#include "Asf.h"
#include "MyFileReader.h"

#define _USE_MATH_DEFINES

using namespace std;

const string Asf::input_fileName = "myfile.dat";
const string Asf::weights_fileName = "objvector.dat";
const string Asf::desired_fileName = "DesiredSolution.dat";

Asf::Asf() {

    ///////// file reading ///////////
    MyFileReader *fileReader;

    fileReader = new MyFileReader(input_fileName);
    x = fileReader->read();
    x_lines = fileReader->linesRead;
    x_columns = fileReader->columnsRead;
    delete fileReader;

    fileReader = new MyFileReader(weights_fileName);
    z = fileReader->read();
    z_lines = fileReader->linesRead;
    z_columns = fileReader->columnsRead;
    delete fileReader;

    fileReader = new MyFileReader(desired_fileName);
    double **x_desiredFromDisk = fileReader->read();
    int x_desiredLinesRead = fileReader->linesRead;
    int x_desiredLColumnsRead = fileReader->columnsRead;
    delete fileReader;

    x_desired_lines = 0;
    if (x_desiredLinesRead > 1 && x_desiredLColumnsRead == 1)
        x_desired_lines = x_desiredLinesRead;
    else if (x_desiredLinesRead == 1 && x_desiredLColumnsRead > 1)
        x_desired_lines = x_desiredLColumnsRead;
    else {
        cout << "Not expecting format for DesiredSolution.txt>" << endl;
        return;
    }

    x_desired = new double[x_desired_lines];
    for (int i = 0; i < x_desiredLinesRead; i++)
        for (int j = 0; j < x_desiredLColumnsRead; j++)
            x_desired[i] = x_desiredFromDisk[i][j];

    for (int i = 0; i < x_desiredLinesRead; i++)
        delete[] x_desiredFromDisk[i];
    delete[] x_desiredFromDisk;

    ///////// file reading END ///////////

    ///////// rest initialization ///////////
    w = new double[z_lines];
    z_ideal = new double[z_lines];
    z_nadir = new double[z_lines];
    sum_diff = new double[x_lines];
    ASF = new double[x_lines];
    weighted_x = new double[x_lines];

    ///////// rest initialization end ///////////

}

Asf::~Asf() {
    delete[] w;
    delete[] z_ideal;
    delete[] z_nadir;
    delete[] x_desired;
    delete[] sum_diff;
    delete[] ASF;
    delete[] weighted_x;

    for (int i = 0; i < x_lines; i++) {
        delete[] x[i];
    }
    delete[] x;
}

void Asf::doCrazyMath() {

    int i, j;
    double ro = 0.001;

    i = -1;
    for (j = 0; j < z_columns; j++) {
        z_ideal[j] = z[i + 1][0];
        z_nadir[j] = z[i + 1][1];
        i++;
    }

    for (i = 0; i < z_columns; i++) {
        w[i] = 1.0 / (z_nadir[i] - z_ideal[i]);
    }

    for (i = 0; i < x_lines; i++) {
        sum_diff[i] = 0;
    }

    for (i = 0; i < x_lines; i++) {
        for (j = 0; j < x_columns; j++) {
            weighted_x[i] = w[j] * (x[i][j] - x_desired[j]);
            sum_diff[i] = sum_diff[i] + weighted_x[i];
        }
    }

    max_weighted_x = weighted_x[0];

    for (i = 0; i < x_lines; i++) {
        if (weighted_x[i] > max_weighted_x) {
            max_weighted_x = weighted_x[i];
        }
    }

    for (i = 0; i < x_lines; i++) {
        ASF[i] = max_weighted_x + ro * sum_diff[i];
    }

    ofstream evaluation("ASFevaluation.dat");
    for (i = 0; i < x_lines; i++) {
        evaluation << fixed << setprecision(5) << ASF[i] << endl;
    }
    evaluation.close();

}
