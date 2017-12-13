/*
 * Asf.cpp
 *
 * Created on: Nov 18, 2017
 * Author: Athanasios Fotos
 */

#include <iostream>
#include <fstream>
#include <iomanip>

#include "Asf.h"
#include "MyFileReader.h"

#define _USE_MATH_DEFINES

using namespace std;

const string Asf::input_fileName = "../resources/myfile.dat";
const string Asf::weights_fileName = "../resources/objvector.dat";
const string Asf::desired_fileName = "../resources/DesiredSolution.dat";
const string Asf::evaluation_fileName = "../resources/ASFevaluation.dat";
const string Asf::log = "../resources/out_L1_G353.log";

Asf::Asf() {

    ///////// file reading ///////////
    MyFileReader *fileReader;

    fileReader = new MyFileReader(log);
    x = fileReader->newRead();

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
        cout << "Not expecting format for DesiredSolution.dat>" << endl;
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
    max_weighted_x = new double[x_lines];
    weighted_x = new double*[x_lines];
    for (int i = 0; i < x_lines; i++){
        weighted_x[i] = new double[x_columns];
    }

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
    delete[] max_weighted_x;

    for (int i = 0; i < x_lines; i++) {
        delete[] x[i];
    }
    delete[] x;
}

void Asf::doCrazyMath() {

    int i, j;
    double ro = 0.001;

    for (i = 0; i < z_lines; i++) {
        z_ideal[i] = z[i][0];
        z_nadir[i] = z[i][1];
    }

    for (i = 0; i < z_lines; i++) {
        w[i] = 1.0 / (z_nadir[i] - z_ideal[i]);
    }

    for (i = 0; i < x_lines; i++) {
        sum_diff[i] = 0;
    }

    for (i = 0; i < x_lines; i++) {
        for (j = 0; j < x_columns; j++) {
            weighted_x[i][j] = w[j] * (x[i][j] - x_desired[j]);
            sum_diff[i] = sum_diff[i] + weighted_x[i][j];
        }
    }

    for (i = 0; i < x_lines; i++) {
        max_weighted_x[i] = weighted_x[i][0];
    }

    for (i = 0; i < x_lines; i++) {
        for (j = 0; j < x_columns; j++) {
            if (weighted_x[i][j] > max_weighted_x[i]) {
                max_weighted_x[i] = weighted_x[i][j];
            }
        }
    }

    for (i = 0; i < x_lines; i++) {
        ASF[i] = max_weighted_x[i] + ro * sum_diff[i];
    }

    ofstream evaluation(evaluation_fileName.c_str());
    for (i = 0; i < x_lines; i++) {
        evaluation << fixed << setprecision(5) << ASF[i] << endl;
    }
    evaluation.close();

}
