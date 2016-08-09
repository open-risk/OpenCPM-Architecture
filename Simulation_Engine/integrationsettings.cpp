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

#include "integrationsettings.h"
#include <fstream>
#include <iostream>
#include <streambuf>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

using namespace std;
using namespace Poco::JSON;

void IntegrationSettings::Print(){
	
        cout << m_Simulations << endl;
        cout << m_lowerBound << endl;
        cout << m_upperBound << endl;
        cout << m_numberGridPoints << endl;
        cout << m_stepSize << endl;        
        cout << m_iAssets << endl;     
        cout << m_pAssets << endl;     
                
}

void IntegrationSettings::ReadFromFile(const string fileName) {
    // Open input file
    ifstream inputFile(fileName.c_str());

    // Reading integration settings from file
    string dummy;
    inputFile >> dummy >> m_Simulations;
    inputFile >> dummy >> m_lowerBound;
    inputFile >> dummy >> m_upperBound;
    inputFile >> dummy >> m_numberGridPoints;

    if (m_numberGridPoints > 1)
        m_stepSize = (m_upperBound - m_lowerBound) / (m_numberGridPoints - 1);
    else
        m_stepSize = 0.0;

    // Close input file
    inputFile.close();
}

void IntegrationSettings::ReadFromJSON(const string fileName) {
        
    int debug = 0;
    const char *cstr = fileName.c_str();
    Parser loParser;
    std::ifstream t(cstr);
    std::stringstream buffer;
    buffer << t.rdbuf();
    string json = buffer.str();
    
    // Parse the JSON and get the Results
    Poco::Dynamic::Var loParsedJson = loParser.parse(json);
    Poco::Dynamic::Var loParsedJsonResult = loParser.result();

    Object::Ptr JsonObject = loParsedJsonResult.extract<Object::Ptr>();

    //  cout << json << endl;
    
   
    // Reading integration settings from JSON file
    m_Simulations = JsonObject->getValue<int>("Simulations");
    m_lowerBound = JsonObject->getValue<double>("LowerIntegrationBound");
    m_upperBound = JsonObject->getValue<int>("UpperIntegrationBound");
    m_numberGridPoints = JsonObject->getValue<int>("NumberOfGridPoints");
    m_iAssets = JsonObject->getValue<int>("iAssets");
    m_pAssets = JsonObject->getValue<int>("pAssets");
      
    if (m_numberGridPoints > 1)
        m_stepSize = (m_upperBound - m_lowerBound) / (m_numberGridPoints - 1);
    else
        m_stepSize = 0.0;

    if (debug) {
        ofstream out("test.txt");

        out << this->m_Simulations << endl;
        out << this->m_lowerBound << endl;
        out << this->m_upperBound << endl;
        out << this->m_numberGridPoints << endl;
                
        out.close();
    }    
    
}


void IntegrationSettings::LoadFromJSON(const string json) {
     
    Parser loParser;        
    // Parse the JSON and get the Results
    Poco::Dynamic::Var loParsedJson = loParser.parse(json);
    Poco::Dynamic::Var loParsedJsonResult = loParser.result();

    Object::Ptr JsonObject = loParsedJsonResult.extract<Object::Ptr>();

    //  cout << json << endl;
    
   
    // Reading integration settings from JSON file
    m_Simulations = JsonObject->getValue<int>("Simulations");
    m_lowerBound = JsonObject->getValue<double>("LowerIntegrationBound");
    m_upperBound = JsonObject->getValue<int>("UpperIntegrationBound");
    m_numberGridPoints = JsonObject->getValue<int>("NumberOfGridPoints");
    m_iAssets = JsonObject->getValue<int>("iAssets");
    m_pAssets = JsonObject->getValue<int>("pAssets");
      
    if (m_numberGridPoints > 1)
        m_stepSize = (m_upperBound - m_lowerBound) / (m_numberGridPoints - 1);
    else
        m_stepSize = 0.0;

    /*
    if (debug) {
        ofstream out("test.txt");

        out << this->m_Simulations << endl;
        out << this->m_lowerBound << endl;
        out << this->m_upperBound << endl;
        out << this->m_numberGridPoints << endl;
                
        out.close();
    }    
    */
    
}
