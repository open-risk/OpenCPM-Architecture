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

#include <fstream>
#include <iostream>
#include "portfolio.h"

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

using namespace std;
using namespace Poco::JSON;

void Portfolio::Resize(int IAssets, int PAssets) {
        sizeI = IAssets;
        sizeP = PAssets;
        op.resize(IAssets);
        oe.resize(IAssets);
        ob.resize(IAssets);
        pp.resize(PAssets);
        pe.resize(PAssets);
        pb.resize(PAssets);
    }

void Portfolio::Print(){
	
    for (int o = 0; o < this->sizeI; o++) {
        cout << o << "\t" << this->op[o] << "\t" << this->oe[o] << "\t" << this->ob[o] << endl;
    }
    for (int o = 0; o < this->sizeP; o++) {
        cout << o << "\t" << this->pp[o] << "\t" << this->pe[o] << "\t" << this->pb[o] << endl;
    }
    return;
                
}

void Portfolio::ReadFromFile(string name) {

    ifstream port(name.c_str());
    // Read pd, exposure, beta
    int id;
    for (int o = 0; o < this->sizeI; o++) {
        port >> id >> this->op[o] >> this->oe[o] >> this->ob[o];
        cout << id << "\t" << this->op[o] << "\t" << this->oe[o] << "\t" << this->ob[o] << endl;
    }
    for (int o = 0; o < this->sizeP; o++) {
        port >> id >> this->pp[o] >> this->pe[o] >> this->pb[o];
        cout << id << "\t" << this->pp[o] << "\t" << this->pe[o] << "\t" << this->pb[o] << endl;
    }
    return;
}

void Portfolio::ReadFromJSON(const string fileName) {

    // int debug = 0;
    const char *cstr = fileName.c_str();
    Parser loParser;
    std::ifstream t(cstr);
    std::stringstream buffer;
    buffer << t.rdbuf();
    string json = buffer.str();

    // Parse the JSON input file and get the Results
    Poco::Dynamic::Var loParsedJson = loParser.parse(json);
    Poco::Dynamic::Var loParsedJsonResult = loParser.result();

    // Portfolio data are an array of objects
    Poco::JSON::Array::Ptr arr = loParsedJsonResult.extract<Array::Ptr>();
    int table_size = arr->size();
            
    if(table_size > this->sizeI + this->sizeP) {
		   cout << "Portfolio file size does not match Test Parameters" << endl;
	   }

    // Individual data rows
    Poco::JSON::Object::Ptr object;
    for (int i = 0; i < table_size; i++) {
        object = arr->getObject(i);
        if (object->getValue<string>("sector") == "Corp") {
            this->oe[i] = object->getValue<double>("exposure");
            this->op[i] = object->getValue<double>("pd");
            this->ob[i] = 0.2;
        } else if (object->getValue<string>("sector") == "Portfolio") {
            this->pe[i] = object->getValue<double>("exposure");
            this->pp[i] = object->getValue<double>("pd");
            this->pb[i] = 0.2;
        } else {
            cout << "Error in Portfolio Read" << endl;
        }
    }
    return;
}

void Portfolio::LoadFromJSON(const string json) {

    Poco::JSON::Parser loParser;
    // Parse the JSON input file and get the Results
    Poco::Dynamic::Var loParsedJson = loParser.parse(json);
    Poco::Dynamic::Var loParsedJsonResult = loParser.result();

    // Portfolio data are an array of objects
    Poco::JSON::Array::Ptr arr = loParsedJsonResult.extract<Array::Ptr>();
    int table_size = arr->size();
            
    if(table_size > this->sizeI + this->sizeP) {
		   cout << "Portfolio file size does not match Test Parameters" << endl;
	   }

    // Individual data rows
    Poco::JSON::Object::Ptr object;
    for (int i = 0; i < table_size; i++) {
        object = arr->getObject(i);
        if (object->getValue<string>("sector") == "Corp") {
            this->oe[i] = object->getValue<double>("exposure");
            this->op[i] = object->getValue<double>("pd");
            this->ob[i] = 0.2;
        } else if (object->getValue<string>("sector") == "Portfolio") {
            this->pe[i] = object->getValue<double>("exposure");
            this->pp[i] = object->getValue<double>("pd");
            this->pb[i] = 0.2;
        } else {
            cout << "Error in Portfolio Read" << endl;
        }
    }
    return;
}
