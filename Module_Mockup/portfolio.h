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

#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <string>

#include <armadillo>
using namespace std;
using namespace arma;

// Portfolio of individual credit exposures and pooled credit exposures

class Portfolio {
public:

	Portfolio(){
		sizeI = 0;
		sizeP = 0;
	}		


    Portfolio(int IAssets, int PAssets) {
        sizeI = IAssets;
        sizeP = PAssets;
        op.resize(IAssets);
        oe.resize(IAssets);
        ob.resize(IAssets);
        pp.resize(PAssets);
        pe.resize(PAssets);
        pb.resize(PAssets);
    }

    ~Portfolio() {
    };

    void Resize(int IAssets, int PAssets);
    void ReadFromFile(const string portfolioFileName);
    void ReadFromJSON(const string portfolioFileName);
    void LoadFromJSON(const string JSONstringName);
    void Print();

    int GetIAssets() const {
        return sizeI;
    }

    int GetPAssets() const {
        return sizeP;
    }

    double GetOP(int i) const {
        return op[i];
    }

    double GetOE(int i) const {
        return oe[i];
    }

    double GetOB(int i) const {
        return ob[i];
    }

    double GetPP(int i) const {
        return pp[i];
    }

    double GetPE(int i) const {
        return pe[i];
    }

    double GetPB(int i) const {
        return pb[i];
    }

private:
    int sizeI;
    int sizeP;
    // Individual exposure data
    vec op;
    vec oe;
    vec ob;
    // Pooled exposure data
    vec pp;
    vec pe;
    vec pb;
};

#endif
