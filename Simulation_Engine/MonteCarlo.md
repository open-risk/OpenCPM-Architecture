### Monte Carlo Algorithms for basic credit portfolio modelling

#### Basic example

	for all z factors (integration grid C)
  		compute probability of z factor from normal density n(z)
  		compute conditional probabilities of default for N entities from thresholds, beta and z Q(z;b;a)
  		compute conditional default thresholds A for N entities
  
	do monte carlo simulation of M iterations for N independent normals 
    	for all N
      		draw normals W
      		compare W with A
      		compute individual loss
      		cumulate total loss

  	store M loss outcome pairs with probability weight n(z) x (1/M)
	sort all loss outcome pairs (C x M)
	while cumulative probability < 0.001
  	sum loss outcome probability weight
	return loss outcome

#### (Some) Complications

* Large Pools
* Multi Factor Models
* Multi Period Simulation
* Importance Sampling
* Valuation Functions
* Contagion Models