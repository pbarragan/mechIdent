//Filter Models
#include "translator.h"
#include "logUtils.h"
#include "filterModels.h"

#include <iostream> // DELETE

double filterModels::logProbState(stateStruct sampleState, stateStruct meanState){

  //sampleState is the sample vector. meanState is the mean vector. This just drops a gaussian at the meanState with a constant covariance from the class.	
  if (meanState.model==sampleState.model && meanState.params==sampleState.params){
  //Move this outside later maybe
  double transArray[] = {0.0001,0.0,0.0,0.0001};
  std::vector<double> transCovMat;
  transCovMat.assign(transArray, transArray + sizeof(transArray)/sizeof(double));

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
    double hold = logUtils::evaluteLogMVG(sampleStateInObs,meanStateInObs,transCovMat);
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
  double obsArray[] = {0.01,0.0,0.0,0.01};
  std::vector<double> obsCovMat;
  obsCovMat.assign(obsArray, obsArray + sizeof(obsArray)/sizeof(double));

  //obs is the sample vector. state is the mean vector. This just drops a gaussian at the state with a constant covariance from the class.
  //Obs is already translated to the observation space which should be a vector directly comparable with another
  //std::vector<double> obsInO = translator::translateObsToO(obs);
  std::vector<double> obsInObs = obs;
  std::vector<double> meanStateInObs = translator::translateStToObs(state);
  double hold = logUtils::evaluteLogMVG(obsInObs,meanStateInObs,obsCovMat);

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
