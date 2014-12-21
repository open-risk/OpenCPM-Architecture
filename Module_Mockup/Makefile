# Copyright (c) 2014 OpenRisk, https://www.openrisk.eu
#
# This source is subject to the terms of the Mozilla Public License 2.0
# Please see the License.html file for more information.
# All other rights reserved.
#
# THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
# KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.

# Compilation and linking options
CC=g++
# Dependencies: Poco, TRNG, Armadillo
CFLAGS= -c -Wall -I/usr/local/include/Poco -I /usr/local/include/trng -I .
LDFLAGS=-lPocoJSON -lPocoFoundation -larmadillo -ltrng4
COMMON=normal_distribution.cpp BivariateNormalDistribution.cpp random_var.cpp
OBJECTS=$(SOURCES:.cpp=.o) $(COMMON:.cpp=.o)
SOURCES=main.cpp spsim.cpp integrationsettings.cpp portfolio.cpp
EXECUTABLE=simulator
# Targets
standard:	$(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)
# cp $(EXECUTABLE4) /tmp


.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) 
