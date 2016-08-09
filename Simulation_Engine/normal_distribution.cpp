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

#include <math.h>
#include "normal_distribution.h"

const double NormalDistribution::SQRT_TWO_PI = sqrt(2.0 * 3.14159265358979);

double NormalDistribution::NormalDensity(double x) {
    return exp(-0.5 * x * x) / SQRT_TWO_PI;
}

//  10 point approximation.

double NormalDistribution::CumulativeNormalDensity(double x) {
    static const double a = 0.3535533905933;
    static const double b1 = -1.26551223;
    static const double b2 = 1.00002368;
    static const double b3 = 0.37409196;
    static const double b4 = 0.09678418;
    static const double b5 = -0.18628806;
    static const double b6 = 0.27886807;
    static const double b7 = -1.13520398;
    static const double b8 = 1.48851587;
    static const double b9 = -0.82215223;
    static const double b10 = 0.17087277;
    double t, term, result;

    double d = (x > 0) ? 1.0 : 0.0;
    double s = (x > 0) ? 1.0 : -1.0;

    if (fabs(x) > 10) {
        result = d;
    } else {
        t = 1.0 / (1.0 + s * a * x);
        term = b9 + t * b10;
        term = b8 + t * term;
        term = b7 + t * term;
        term = b6 + t * term;
        term = b5 + t * term;
        term = b4 + t * term;
        term = b3 + t * term;
        term = b2 + t * term;
        term = b1 + t * term;
        term += -0.5 * (x * x);
        result = d - s * 0.5 * t * exp(term);
    }
    return result;
}

double NormalDistribution::InverseCumulativeNormalDensity(double u) {
    // From: "The Full Monte", by Boris Moro (UBS) in: RISK 1995:2

    static const double a0 = 2.50662823884;
    static const double a1 = -18.61500062529;
    static const double a2 = 41.39119773534;
    static const double a3 = -25.44106049637;

    static const double b0 = -8.47351093090;
    static const double b1 = 23.08336743743;
    static const double b2 = -21.06224101826;
    static const double b3 = 3.13082909833;

    static const double c0 = 0.3374754822726147;
    static const double c1 = 0.9761690190917186;
    static const double c2 = 0.1607979714918209;
    static const double c3 = 0.0276438810333863;
    static const double c4 = 0.0038405729373609;
    static const double c5 = 0.0003951896511919;
    static const double c6 = 0.0000321767881768;
    static const double c7 = 0.0000002888167364;
    static const double c8 = 0.0000003960315187;

    double x, r;

    if (u > 1 - 1E-10) {
        u = 1 - 1E-10;
    }

    if (u < 1E-10) {
        u = 1E-10;
    }

    x = (u - 0.5);

    if (x > -0.42 && x < 0.42) {
        r = x * x;
        r = x * (((a3 * r + a2) * r + a1) * r + a0) / ((((b3 * r + b2) * r + b1) * r + b0) * r + 1.0);
    } else {
        r = u;

        if (x > 0.0) r = (1.0 - u);

        r = log(-log(r));
        r = c0 + r * (c1 + r * (c2 + r * (c3 + r * (c4 + r * (c5 + r * (c6 + r * (c7 + r * c8)))))));

        if (x < 0.0) r = -r;
    }

    return r;
}







