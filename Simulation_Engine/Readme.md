**Objective:**

- Create a open source c++ based simulation engine to support a credit portfolio management toolkit

**Approach:**

* Look to evaluate and adopt a minimal set of libraries: 
    * poco++ (general utility classes)
    * array classes (eigen, armadillo)
    * random number generators (trng)
* Library structure
    * Components usable in multiple configurations)
    * Minimize simulation model dependency
* Use REST API for control, portolio and outcomes data
* Have Command line interface, stylized portfolios as inputs
* Have  Testing Framework (numerical / algorithm and software / interface)
* Tools for performance tuning 

**Deliverables:**

* Set of c++ classes / methods
* Documentation (markdown, doxygen?)
* Illustrative implementation for toy problems

**Ho to use the current example**

1. Requires linux environment
2. Download OpenCPM files
3. Install Poco++, TRNG and Armadillo. Poco++ and TRNG should be in /usr/local/include, otherwise adjust the makefile
4. Type 'Make simulator' in the code directory