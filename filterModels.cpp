//Filter Models
#include "translator.h"
#include "logUtils.h"
#include "filterModels.h"

double filterModels::logProbState(stateStruct sampleState, stateStruct meanState){
  //Move this outside later maybe
  double transArray[] = {0.0000001,0.0,0.0,0.0000001};
  std::vector<double> transCovMat;
  transCovMat.assign(transArray, transArray + sizeof(transArray)/sizeof(double));
  
  //sampleState is the sample vector. meanState is the mean vector. This just drops a gaussian at the meanState with a constant covariance from the class.	
  if (meanState.model==sampleState.model){
    //translate to the observation space which should be a vector directly comparable with another
    std::vector<double> sampleStateInObs = translator::translateStToObs(sampleState);
    std::vector<double> meanStateInObs = translator::translateStToObs(meanState);

    return logUtils::evaluteLogMVG(sampleStateInObs,meanStateInObs,transCovMat);
  }
  else {
    //return 0.0 in log space
    return -std::numeric_limits<double>::infinity();
  }
}

double filterModels::logProbObs(std::vector<double> obs, stateStruct state){
  //Move this outside later maybe
  double obsArray[] = {0.0000001,0.0,0.0,0.0000001};
  std::vector<double> obsCovMat;
  obsCovMat.assign(obsArray, obsArray + sizeof(obsArray)/sizeof(double));

  //obs is the sample vector. state is the mean vector. This just drops a gaussian at the state with a constant covariance from the class.
  //Obs is already translated to the observation space which should be a vector directly comparable with another
  //std::vector<double> obsInO = translator::translateObsToO(obs);
  std::vector<double> obsInObs = obs;
  std::vector<double> meanStateInObs = translator::translateStToObs(state);
  return logUtils::evaluteLogMVG(obsInObs,meanStateInObs,obsCovMat);
}
