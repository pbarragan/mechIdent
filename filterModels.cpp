//Filter Models
#include "translator.h"
#include "logUtils.h"
#include "filterModels.h"

double filterModels::logProbState(std::vector<double> sampleState, std::vector<double> meanState){
  //Move this outside later maybe
  double transArray[] = {0.0000001,0.0,0.0,0.0000001};
  std::vector<double> transCovMat;
  transCovMat.assign(transArray, transArray + sizeof(transArray)/sizeof(double));
  
  //sampleState is the sample vector. meanState is the mean vector. This just drops a gaussian at the meanState with a constant covariance from the class.	
  if (meanState[0]==sampleState[0]){
    //translate to the observation space which should be a vector directly comparable with another
    std::vector<double> sampleStateInO = translator::translateStateToO(sampleState);
    std::vector<double> meanStateInO = translator::translateStateToO(meanState);
    return logUtils::evaluteLogMVG(sampleStateInO,meanStateInO,transCovMat);
  }
  else {
    //return 0.0;
    return -std::numeric_limits<double>::infinity();
  }
}

double filterModels::logProbObs(std::vector<double> obs, std::vector<double> state){
  //Move this outside later maybe
  double obsArray[] = {0.0000001,0.0,0.0,0.0000001};
  std::vector<double> obsCovMat;
  obsCovMat.assign(obsArray, obsArray + sizeof(obsArray)/sizeof(double));

  //obs is the sample vector. state is the mean vector. This just drops a gaussian at the state with a constant covariance from the class.
  //translate to the observation space which should be a vector directly comparable with another
  std::vector<double> obsInO = translator::translateObsToO(obs);
  std::vector<double> meanStateInO = translator::translateStateToO(state);
  return logUtils::evaluteLogMVG(obsInO,meanStateInO,obsCovMat);
}
