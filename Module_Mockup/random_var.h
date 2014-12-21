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

#ifndef RANDOM_VARIABLE_H
#define RANDOM_VARIABLE_H

#include <iostream>

#include <armadillo>
using namespace std;
using namespace arma;

class RandomVar {
    friend ostream & operator<<(ostream &os, const RandomVar & R);
public:

    // constructor with size and type
    
    RandomVar(size_t S, int type) {
		if(type == 0) {
			m_type = 0;
			m_P.resize(S + 1);
			m_C.resize(S + 1);
			m_X.resize(S + 1);
		}
		else if (type == 1) {
			m_type = 1;
			m_S.resize(S+1);
		}
		else {
			cout << "Error in Random variable type" << endl;
		}
    }

    // constructor with existing data
    RandomVar(vec x, vec p, const int size) {
		m_type = 0;
		m_X.resize(size);
		m_P.resize(size);
		m_C.resize(size);
		for (int i = 0; i < size; i++) {
			m_X[i] = x[i];
			m_P[i] = p[i];
		}
	}

	// overload assignment operator

    RandomVar & operator=(const RandomVar & R);

    size_t size() const {
        return m_P.size();
    };

	int type() const {
		return m_type;
	};

    double getP(int index) const {
        return m_P[index];
    };

    double getC(int index) const {
        return m_C[index];
    };

    double getX(int index) const {
        return m_X[index];
    };

    double getS(int index) const {
        return m_S[index];
    };

    void setP(int index, double arg) {
        m_P[index] = arg;
    };

    void setC(int index, double arg) {
        m_C[index] = arg;
    };

    void setX(int index, double arg) {
        m_X[index] = arg;
    };

    void setS(int index, double arg) {
        m_S[index] = arg;
    };
    
	void Sort();
    void Cumulative();
    void Probability();

    double Average() const;
    double Mean() const;
    double Median() const;
    double Variance() const;
    double Vol() const;
    double StandardDeviation() const;
    double Kurtosis() const;
    double Skeweness() const;
    double ShortFall(double Z) const;
    double Quantile(double Z) const;
    int Quantile_Index(double Z) const;

    void ReadFromJSON(const char* fileName);
    void Print();

private:
	// 0 Type: exact representation
	// 1 Type: sampling representation
	int m_type; 
    vec m_P; // storage of probability mass
    vec m_C; // storage of cumulative probability
    vec m_X; // storage of discrete values (random variable range)
    vec m_S; // storage of sampling data from simulation experiments
};

#endif
