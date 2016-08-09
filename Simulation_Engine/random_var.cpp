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

#include "math.h"
#include "random_var.h"
#include <iostream>
#include <assert.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <armadillo>

using namespace std;
using namespace arma;
using namespace Poco::JSON;

RandomVar & RandomVar::operator=(const RandomVar & R) {
    assert(R.size() == this->size()); // check that size matches
    for (size_t i = 0; i < R.size(); i++) {
        this->setX(i, R.getX(i));
        this->setP(i, R.getP(i));
        this->setC(i, R.getC(i));
    }
    return (*this);
};

void RandomVar::Sort() {
    sort(m_S);
	return;
}

void RandomVar::Cumulative() {
    m_C[0] = m_P[0];
    for (size_t i = 1; i < m_P.size(); i++)
        m_C[i] = m_C[i - 1] + m_P[i];
    return;
}

void RandomVar::Probability() {
    m_P[0] = m_C[0];
    for (size_t i = 1; i < m_P.size(); i++)
        m_P[i] = m_C[i] - m_C[i - 1];
    return;
}

double RandomVar::Average() const {
    double expectation = 0.0;
	if(m_type == 0) {
		for (size_t i = 0; i < m_P.size(); i++) {
			expectation += m_P[i] * m_X[i];
		}
	}
	else if (m_type == 1){
		for (size_t i = 0; i < m_S.size(); ++i) {
			expectation += m_S[i];
		}
		expectation /= m_S.size();
	}
    return expectation;
}

double RandomVar::Mean() const {
    return Average();
}

double RandomVar::Median() const {
	// TODO
    return 999.9;
}

double RandomVar::Variance() const {
    double var = 0;
    if(m_type == 0) {
		for (size_t i = 0; i < m_P.size(); i++)
			var += m_P[i] * m_X[i] * m_X[i];
		var -= Average() * Average();
	}
	else if (m_type == 1) {
		for (size_t i = 0; i < m_S.size(); i++)
			var += m_S[i] * m_S[i];
		var /= m_S.size();		
		var -= Average() * Average();	
	}
    return var;
}

double RandomVar::Vol() const {
    return sqrt(Variance());
}

double RandomVar::StandardDeviation() const {
    return Vol();
}

double RandomVar::Skeweness() const {
    double skew = 0;
    double mean = Average();
    for (size_t i = 0; i < m_P.size(); i++)
        skew += m_P[i] * pow(m_X[i] - mean, 3);
    skew = skew / pow(Variance(), 3 / 2);
    return skew;
}

double RandomVar::Kurtosis() const {
    double kurt = 0;
    double mean = Average();
    for (size_t i = 0; i < m_P.size(); i++)
        kurt += m_P[i] * pow(m_X[i] - mean, 4);
    kurt = kurt / pow(Variance(), 2);
    return kurt;
}

int RandomVar::Quantile_Index(double Z) const {
    size_t index;
    for (size_t i = m_P.size() - 1; i >= 0; i--) {
        if (Z >= m_C[i]) {
            index = i + 1;
            break;
        }
    }
    return (int) index;
}

double RandomVar::Quantile(double Z) const {
    int index = this->Quantile_Index(Z);
    return m_X[index];
}

double RandomVar::ShortFall(double alpha) const {
    int iVaR = this->Quantile_Index(alpha);
    double es = 0;
    for (size_t k = iVaR; k < m_P.size(); k++)
        es += m_P[k] * m_X[k];
    es /= (1.0 - alpha);
    return es;
}


ostream & operator<<(ostream &os, const RandomVar & R) {
    ostringstream out;
    for (size_t k = 0; k < R.size(); k++)
        out << R.getX(k) << "\t" << R.getP(k) << "\t" << R.getC(k) << endl;
    return os << out.str();
};

void RandomVar::Print(){
	if(this->m_type == 1)
	{
		for (size_t s = 0; s < this->m_S.size(); s++) {
			cout << s << "\t" << this->m_S[s] << endl;
		}
	}
    return;           
}

void RandomVar::ReadFromJSON(const char* fileName) {
    
    Parser loParser;
    std::ifstream t(fileName);
    std::stringstream buffer;
    buffer << t.rdbuf();
    string json = buffer.str();
    // Parse the JSON and get the Results
    Poco::Dynamic::Var loParsedJson = loParser.parse(json);
    Poco::Dynamic::Var loParsedJsonResult = loParser.result();

    // Random variable data are an array of objects
    //[
    // {"value": 1, "probability" : 0.2, "cumulative" : 0.2},
    // {"value": 2, "probability" : 0.2, "cumulative" : 0.4},
    // {"value": 3, "probability" : 0.2, "cumulative" : 0.6},
    // {"value": 4, "probability" : 0.2, "cumulative" : 0.8},
    // {"value": 5, "probability" : 0.2, "cumulative" : 1.0}
    //]    
    
    Array::Ptr arr = loParsedJsonResult.extract<Array::Ptr>();
    size_t size = arr->size();

    m_P.resize(size);
    m_C.resize(size);
    m_X.resize(size);
            
    // Individual data rows
    Object::Ptr object;
    for (size_t i = 0; i < size; i++) {
        object = arr->getObject(i);
        this->setX(i, object->getValue<double>("value"));
        this->setP(i, object->getValue<double>("probability"));
        this->setC(i, object->getValue<double>("cumulative"));        
    }

    return;
}
