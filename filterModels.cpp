//Filter Models
#include "translator.h"
#include "logUtils.h"
#include "filterModels.h"

#include <iostream> // DELETE

double filterModels::logProbState(stateStruct sampleState, stateStruct meanState){

  //sampleState is the sample vector. meanState is the mean vector. This just drops a gaussian at the meanState with a constant covariance from the class.	
  if (meanState.model==sampleState.model && meanState.params==sampleState.params){
    //Move this outside later maybe
    //double transArray[] = {0.0001,0.0,0.0,0.0001}; // change NORMAL
    //double transArray[] = {0.01,0.0,0.0,0.01};
    //std::vector<double> transCovMat;
    //transCovMat.assign(transArray, transArray + sizeof(transArray)/sizeof(double));
    
    // set additional variables
    // create inverse matrix (hard coded)
    //double invTransArray[] = {10000.0,0.0,0.0,10000.0}; // change NORMAL .01
    //double invTransArray[] = {625.0,0.0,0.0,625.0}; // change MIDDLE .04 - used for robot
    double invTransArray[] = {100.0,0.0,0.0,100.0}; // .1
    std::vector<double> invTransCovMat;
    invTransCovMat.assign(invTransArray, invTransArray + sizeof(invTransArray)/sizeof(double));

    // create determinant (hard coded)
    //double detCovMat = 0.00000001; // change NORMAL .01
    //double detCovMat = 0.00000256; // change MIDDLE .04 - used for robot
    double detCovMat = 0.0001; // .1

    //translate to the observation space which should be a vector directly comparable with another
    std::vector<double> sampleStateInObs = translator::translateStToObs(sampleState);
    std::vector<double> meanStateInObs = translator::translateStToObs(meanState);
    /*
    std::cout << "===============TRANSITION===========" << std::endl;
    std::cout << "Model: " << meanState.model << std::endl;
    std::cout << "params: " << std::endl;
    for(size_t i=0;i<meanState.params.size();i++){
      std::cout << meanState.params[i] << ",";
    }
    std::cout << std::endl;
    std::cout << "vars: " << std::endl;
    for(size_t i=0;i<meanState.vars.size();i++){
      std::cout << meanState.vars[i] << ",";
    }
    std::cout << std::endl;

    std::cout << "sampleStateInObs: " << sampleStateInObs[0] << "," << sampleStateInObs[1] << std::endl;
    std::cout << "meanStateInObs: " << meanStateInObs[0] << "," << meanStateInObs[1] << std::endl;
    */
    double hold = logUtils::evaluteLogMVG(sampleStateInObs,meanStateInObs,invTransCovMat,detCovMat);
    //std::cout << "prob: " << hold << std::endl;
    return hold;
  }
  else {
    //return 0.0 in log space
    return -std::numeric_limits<double>::infinity();
  }
}

double filterModels::logProbObs(std::vector<double> obs, stateStruct state){
  //Move this outside later maybe
  //double obsArray[] = {0.01,0.0,0.0,0.01}; // change
  //double obsArray[] = {0.0001,0.0,0.0,0.0001};
  //double obsArray[] = {0.0004,0.0,0.0,0.0004};

  //std::vector<double> obsCovMat;
  //obsCovMat.assign(obsArray, obsArray + sizeof(obsArray)/sizeof(double));

  // set additional variables
  // create inverse matrix (hard coded)
  //double invObsArray[] = {100.0,0.0,0.0,100.0}; // change // old .1
  //double invObsArray[] = {10000.0,0.0,0.0,10000.0}; // smaller .01
  double invObsArray[] = {625.0,0.0,0.0,625.0}; // middle .04
  //double invObsArray[] = {2500.0,0.0,0.0,2500.0};

  std::vector<double> invObsCovMat;
  invObsCovMat.assign(invObsArray, invObsArray + sizeof(invObsArray)/sizeof(double));
  
  // create determinant (hard coded)
  //double detCovMat = 0.0001; // change // old .1
  //double detCovMat = 0.00000001; // smaller .01
  double detCovMat = 0.00000256; // middle .04
  //double detCovMat = 0.00000016;

  //obs is the sample vector. state is the mean vector. This just drops a gaussian at the state with a constant covariance from the class.
  //Obs is already translated to the observation space which should be a vector directly comparable with another
  //std::vector<double> obsInO = translator::translateObsToO(obs);
  std::vector<double> obsInObs = obs;
  std::vector<double> meanStateInObs = translator::translateStToObs(state);
  double hold = logUtils::evaluteLogMVG(obsInObs,meanStateInObs,invObsCovMat,detCovMat);

  /*
  if (state.model==4 || state.model==0){
    std::cout << "?????????????????????????????????????????" << std::endl;
    std::cout << "model: " << state.model << std::endl;
    std::cout << "params: " << std::endl;
    for(size_t i=0;i<state.params.size();i++){
      std::cout << state.params[i] << ",";
    }
    std::cout << std::endl;
    std::cout << "vars: " << std::endl;
    for(size_t i=0;i<state.vars.size();i++){
      std::cout << state.vars[i] << ",";
    }
    std::cout << std::endl;
    std::cout << "obsInObs: " << obsInObs[0] << "," << obsInObs[1] << std::endl;
    std::cout << "meanStateInObs: " << meanStateInObs[0] << "," << meanStateInObs[1] << std::endl;
    std::cout << "prob: " << hold << std::endl;
  }
  */
  return hold;
}
