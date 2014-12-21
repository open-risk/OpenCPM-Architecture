// Copyright (c) 2014 OpenRisk, https://www.openrisk.eu
//
// This source is subject to the terms of the Mozilla Public License 2.0
// Please see the License.html file for more information.
// All other rights reserved.
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.

#include "BivariateNormalDistribution.h"
#include "normal_distribution.h"
#include <math.h>

double
BivariateNormalDistribution::CumulativeBivariateNormalDensity(double a, double b, double rho) {
    // based on Z. Drezner, "Computation of the bivariate normal integral", Mathematics of Computation 32, pp. 277-279, 1978.
    // uses 8-point Gaussian quadrature
    // (Hull uses 4-point)


    static const double Epsilon = 1e-12;

    double value;

    if (rho > 1 - Epsilon) {
        if (a < b) value = NormalDistribution::CumulativeNormalDensity(a);
        else value = NormalDistribution::CumulativeNormalDensity(b);
        return value;
    }

    if (rho < -(1 - Epsilon)) {
        if (a < -b) value = 0;
        else value = NormalDistribution::CumulativeNormalDensity(a) - NormalDistribution::CumulativeNormalDensity(-b);
        return value;
    }

    if (a <= 0 && b <= 0 && rho <= 0)
        value = Phi_Sum(a, b, rho);
    else if (a <= 0 && b >= 0 && rho >= 0)
        value = NormalDistribution::CumulativeNormalDensity(a) - Phi_Sum(a, -b, -rho);
    else if (a >= 0 && b <= 0 && rho >= 0)
        value = NormalDistribution::CumulativeNormalDensity(b) - Phi_Sum(-a, b, -rho);
    else if (a >= 0 && b >= 0 && rho <= 0)
        value = NormalDistribution::CumulativeNormalDensity(a) + NormalDistribution::CumulativeNormalDensity(b) - 1 + Phi_Sum(-a, -b, rho);
    else {
        value = CumulativeBivariateNormalDensity(a, 0, rhoc(a, b, rho));
        value = value + CumulativeBivariateNormalDensity(b, 0, rhoc(b, a, rho));
        if ((a > 0 && b < 0) || (a < 0 && b > 0))
            value = value - 0.5;
    }

    return value;
}

double
BivariateNormalDistribution::BivariateNormalDensity(double a, double b, double rho) {
    static const double ONE_OVER_2PI = 0.15915494309189533576888376337251;
    const double ONE_OVER_SQRT_ONE_MIN_RHO_SQRD = 1. / sqrt(1. - rho * rho);

    return ONE_OVER_2PI * ONE_OVER_SQRT_ONE_MIN_RHO_SQRD * exp(-0.5 * ONE_OVER_SQRT_ONE_MIN_RHO_SQRD * ONE_OVER_SQRT_ONE_MIN_RHO_SQRD * (a * a + b * b - 2. * rho * a * b));
}

double
BivariateNormalDistribution::Phi_Sum(double a, double b, double rho) {
    static const double PI = 3.1415926535897932384626433832795;
    // static const double SQRT_2 = 1.4142135623730950488016887242097;
    static int SUMNUM = 8;
    static double Apara[] = {0.13410918845336,
        0.26833075447264,
        0.275953397988422,
        0.15744828261879,
        4.48141099174625E-02,
        5.36793575602526E-03,
        2.02063649132407E-04,
        1.19259692659532E-06};
    static double Bpara[] = {5.29786439318514E-02,
        0.267398372167767,
        0.616302884182402,
        1.06424631211623,
        1.58885586227006,
        2.18392115309586,
        2.86313388370808,
        3.6860071627244};

    double sum, temp;
    double fpa, fpb, fpai;
    double srtomr2;

    srtomr2 = sqrt(1.0 - rho) * sqrt(1.0 + rho);
    fpa = a / (sqrt(2.0) * srtomr2);
    fpb = b / (sqrt(2.0) * srtomr2);

    sum = 0.0;
    for (int i = 0; i < SUMNUM; i++) {
        temp = 0.0;
        fpai = Bpara[i] - fpa;
        for (int j = 0; j < SUMNUM; j++) {
            temp = temp + Apara[j] * exp(fpb * (2.0 * Bpara[j] - fpb) + 2.0 * rho * fpai * (Bpara[j] - fpb));
        }
        sum = sum + Apara[i] * exp(fpa * (2.0 * Bpara[i] - fpa)) * temp;
    }

    return sum * srtomr2 / PI;
}

double
BivariateNormalDistribution::rhoc(double a, double b, double rho) {
    double x = 1.0;
    if (a < 0) x = -1.0;
    return x * (rho * a - b) / sqrt(a * a - 2 * rho * a * b + b * b);
}




