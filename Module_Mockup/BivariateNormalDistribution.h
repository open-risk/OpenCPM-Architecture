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

#ifndef BIVARIATE_NORMAL_DISTRIBUTION_H
#define BIVARIATE_NORMAL_DISTRIBUTION_H

class BivariateNormalDistribution
{
public:
  static double CumulativeBivariateNormalDensity(double x, double y, double rho);
  static double BivariateNormalDensity(double x, double y, double rho);

private:
	static double Phi_Sum(double,double,double);
	static double rhoc(double,double,double);
};

#endif
