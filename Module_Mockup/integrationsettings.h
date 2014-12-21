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

#ifndef INTEGRATIONSETTINGS_H
#define INTEGRATIONSETTINGS_H

#include <string>

using namespace std;

class IntegrationSettings {
	
public:

    // Constructor

    IntegrationSettings() {
    };

    // Destructor

    ~IntegrationSettings() {
    };

    // Interface
    void ReadFromFile(const string fileName);
    void ReadFromJSON(const string fileName);
    void LoadFromJSON(const string JSONstringName);
    void Print();
    
    // Accessors

    double GetLowerBound() const {
        return m_lowerBound;
    }

    double GetUpperBound() const {
        return m_upperBound;
    }

    int GetNumberGridPoints() const {
        return m_numberGridPoints;
    }

    double GetStepSize() const {
        return m_stepSize;
    }

    int GetSimulations() const {
        return m_Simulations;
	}
        
    int GetiAssets() const {
        return m_iAssets;    
	}  

    int GetpAssets() const {
        return m_pAssets; 
	}

private:

    // Member variables
    int m_Simulations;
    double m_lowerBound;
    double m_upperBound;
    int m_numberGridPoints;
    double m_stepSize;
    int m_iAssets;
    int m_pAssets;

};

#endif
