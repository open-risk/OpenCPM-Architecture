## A1 Simulation Engine 

This is the core simulation (scenario) engine. 

### Model characteristics (Version 1.0):
   
* single factor credit threshold model
* multi-period	
* aggregation
    * single obligor entities 
    * large pool entities (aggregates)
* default only loss
* arbitrary portfolio inputs (pd, ead, [lgd])
* single factor loadings (variable)
  
#### Methodology:

* monte carlo
* no importance sampling
* pseudo random number generation

#### Risk Metrics / Other Outputs: 

* quantile result at [99.XX] / other distribution statistics
* results statistical errors / confidence levels
* result distribution plot 

#### Internal model structure / classes:

* portfolio data object  			(read from json <- web / file)
* model data object	(pd / loadings)	(read from json <- web / file)
* configuration data object			(read from json <- web / file)
* loss distribution object 			(write to: json -> d3/web, csv -> python/desktop)
* results object					(write to: json -> web / file / stream)
* simulator object?
	
#### Implementation:
	
* C++ libraries: eigen, trng, armadillo, POCO etc...
* optimizing compilers
* memory and performance tuning (single processor)

#### Acceptance Criteria / Testing

##### Absolute convergence

* vasicek limit			(analytic - python library)
* finite pools 			(analytic - python library)
* single name + pool 	(analytic - python library)

##### Cauchy convergence

* random portfolios		(batch calculations / python scripts)
