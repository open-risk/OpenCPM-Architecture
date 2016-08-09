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


// system libraries
#include <iostream>
#include <fstream>
#include "math.h"
#include "time.h"

// own libraries
#include "normal_distribution.h"
#include "integrationsettings.h"
#include "portfolio.h"
#include "random_var.h"

// third party libraries
#include <armadillo>
#include <trng/yarn2.hpp>
#include <trng/normal_dist.hpp>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>


using namespace std;
using namespace arma;

std::string SingleStepMC(Portfolio & P, IntegrationSettings & IS) {

    // vec  q       = randu<vec>(10);
    // uvec indices = sort_index(q);  

    // initialize random number engine
    trng::yarn2 R;
    R.seed(time(NULL));
    // normal distribution with mean 0 and standard deviation 1
    trng::normal_dist<> normal(0.0, 1.0);

	// P.Print();
	// IS.Print();
	

    // generate N normal distributed random numbers
    int N = IS.GetSimulations();
    // Storage of loss scenarios
    // vec loss(N);
    
    RandomVar loss(N,1);


    // calculate thresholds
    vec oa(P.GetIAssets());
    vec pa(P.GetPAssets());
    for (int o = 0; o < P.GetIAssets(); o++) {
        oa[o] = NormalDistribution::InverseCumulativeNormalDensity(P.GetOP(o));
    }
    for (int o = 0; o < P.GetPAssets(); o++) {
        pa[o] = NormalDistribution::InverseCumulativeNormalDensity(P.GetPP(o));
    }

    // direct mc loop

    // scenario losses    
    // int zero_loss_count = 0;
    for (int i = 0; i < N; ++i) {
        double scenario_loss = 0.0;
        double zt = normal(R);

        // Individual Obligor Loss Loop
        for (int o = 0; o < P.GetIAssets(); o++) {
            double rho = P.GetOB(o);
            double rho2 = sqrt(1 - rho * rho);
            double epsilon = normal(R);
            // Counterparty risk driver
            double zd = rho * zt + rho2 * epsilon;
            if (zd < oa[o]) {
				// Counterparty exposure added to scenario loss
                scenario_loss += P.GetOE(o);
            }
        }

        // Pooled Exposures Loss Loop
        for (int o = 0; o < P.GetPAssets(); o++) {
			cout << "Oops" << endl;
            double rho = P.GetPB(o);
            // Conditional pool loss fraction
	        double arg = (pa[o] - rho * zt) / sqrt(1 - rho * rho);
	        double Q = NormalDistribution::CumulativeNormalDensity(arg);
	        // Pool fractional loss added to scenario loss
            scenario_loss += P.GetPE(o) * Q;
        }
        loss.setS(i, scenario_loss);
    }

    /*
    double average1 = 0.0;
    for (int i = 0; i < N; ++i) {
        average1 += loss.getS(i);
    }
    average1 /= N;
    */
    cout << "Average: " << loss.Average() << endl;
    // cout << "Variance: " << loss.Variance() << endl;
    cout << "Volatility: " << loss.Vol() << endl;
 
 
     // Write results to JSON object
    Poco::JSON::Object object;
    object.set("N", N);
    object.set("EL", loss.Average());
    object.set("UL", loss.Vol());
  
    ostringstream results;  
    object.stringify(results);  
   
    /*
    loss.Sort();
    int q_index = loss.size() * 99.9 / 100;
    cout << "Quantile1: " << loss.getS(q_index) << endl;
    loss.Print();
	*/

    return results.str();

}
