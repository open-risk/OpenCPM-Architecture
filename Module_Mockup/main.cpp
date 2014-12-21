// CGI calculator of portfolio loss distribution
//
// Read JSON stream of portfolio data
// Populate C++ arrays with portfolio and model data
// Pass portfolio / model data to the loss calculation function
// Return quantile result in JSON format

#include <fstream>
#include <iostream>
#include <streambuf>

#include <armadillo>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <ctime>
#include <string.h>

#include "integrationsettings.h"
#include "portfolio.h"

using namespace std;
using namespace Poco::JSON;
using namespace arma;

std::string SingleStepMC(Portfolio & P , IntegrationSettings & IS) ;

int main(int argc, char* argv[]) {

    IntegrationSettings IS;
    Portfolio P;

// Read from command line or hardcoded files from drive

    int mode = 1;
    if(mode){
// Read files holding JSON formated data from drive
		cout << "Reading hardcoded input files from drive" << endl;
		IS.ReadFromJSON("integrationsettings.json");  
		int IAssets = IS.GetiAssets();
		int PAssets = IS.GetpAssets();
		P.Resize(IAssets, PAssets);   
		P.ReadFromJSON("data.json");
    }  
    else {
// Parse streams holding JSON formated data from command line	   
       cout << "Reading the two input files from command line" << endl;
       ifstream myfile1, myfile2;
       std::string json1, json2;
       std::string line;

       myfile1.open(argv[1]);		
       myfile2.open(argv[2]);
       while (std::getline(myfile1, line))
			json1.append(line);

       while (std::getline(myfile2, line))
			json2.append(line);
			
       myfile1.close();
       myfile2.close();
       
// Load JSON strings
       IS.LoadFromJSON(json1);
	   int IAssets = IS.GetiAssets();
	   int PAssets = IS.GetpAssets();
	   P.Resize(IAssets, PAssets);          
       P.LoadFromJSON(json2);       
   }
      
    
// Invoke Calculation Engine
// Pass portfolio / Model data to the loss simulation function
    
    std::string results = SingleStepMC(P, IS);  
    ofstream myfile;
    myfile.open ("output.json");
    myfile << results;
    myfile.close();
    myfile.flush();
      
    return 0;
}
