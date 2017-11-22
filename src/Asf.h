/*
 * MyCalculator.h
 *
 *  Created on: Nov 14, 2017
 *      Author: user
 */

#ifndef ASF_H_
#define ASF_H_

class Asf {
    static const std::string input_fileName;
    static const std::string weights_fileName;
    static const std::string desired_fileName;
    static const std::string evaluation_fileName;
public:
    std::string fileName;

    double **x;
    int x_lines;
    int x_columns;


    double **z;
    int z_lines;
    int z_columns;

    double *w;
    double *z_ideal;
    double *z_nadir;

    double *x_desired;
    int x_desired_lines;
    double *weighted_x;
    double *sum_diff;

    double *ASF;

    double max_weighted_x;

    Asf();

    virtual ~Asf();

    void doCrazyMath();
};

#endif /* ASF_H_ */
