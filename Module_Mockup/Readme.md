**Objective:**

- Create a open source c++ based simulation framework to support a credit portfolio management toolkit

**Approach:**

- Look to adopt / evaluate: poco++, array classes (eigen, armadillo), random number generators (trng), other utility classes. Compare with Boost
- Library structure (components usable in multiple configurations)
- Minimise model dependency
- REST API for control, portolio and outcomes data
- Command line interface, stylized portfolios as inputs
- Testing Framework (numerical / algorithm and software / interface)
- Performance tuning 

**Deliverables:**

- Set of c++ classes / methods
- Documentation (markdown, doxygen?)
- Illustrative implementation for toy problems

**Usage of current example**

- Linux environment
- Download OpenCPM files
- Install Poco++, TRNG and Armadillo. Poco++ and TRNG should be in /usr/local/include, otherwise adjust the makefile
- Make simulator