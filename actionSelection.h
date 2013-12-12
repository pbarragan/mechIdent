#ifndef ACTION_SELECTION_H
#define ACTION_SELECTION_H

#include <vector>
#include "stateStruct.h"
#include "sasUtils.h"
#include "bayesFilter.h"

namespace actionSelection {
  
  void chooseActionSimple(std::vector< std::vector<double> >& actionList,int step,std::vector<double>& action);
  void chooseActionRandom(std::vector< std::vector<double> >& actionList,std::vector<double>& action);
  // overloaded
  void chooseActionLog(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs);
  void chooseActionLog(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,sasUtils::mapPairSVS& sasList);

  // overloaded
  void chooseActionOG(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs);
  void chooseActionOG(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,sasUtils::mapPairSVS& sasList);

  // relative
  void chooseActionSimpleRel(std::vector< std::vector<double> >& actionList,int step,std::vector<double>& action,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace);
  void chooseActionRandomRel(std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace);
  void chooseActionLogRel(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace);
  void chooseActionLogRel(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,sasUtils::mapPairSVS& sasList,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace);
  void chooseActionOGRel(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace);
  void chooseActionOGRel(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,sasUtils::mapPairSVS& sasList,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace);

  // aux
  std::vector<double> getNoisyObs(stateStruct& state);
  std::vector<double> createCDF(std::vector<double>& probList);
  stateStruct getSampleState(std::vector<double>& CDF, std::vector<stateStruct>& states);
  double calcEntropy(std::vector<double> probs);
  double randomDouble();
  double distPointsSquared(std::vector<double> a, std::vector<double> b);
  void relToAbsActionList(std::vector< std::vector<double> >& relActionList,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& absActionList);
  void absToRelActionList(std::vector< std::vector<double> >& absActionList,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& relActionList);
  void absToRelAction(std::vector<double>& tempAbsAction,std::vector<double>& poseInRbt,std::vector<double>& tempRelAction);
  void validateRelActionList(std::vector< std::vector<double> >& relActionList,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace,std::vector< std::vector<double> >& validRelActionList);

}
  
#endif //ACTION_SELECTION_H
