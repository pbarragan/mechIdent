#ifndef MODEL_UTILS_H
#define MODEL_UTILS_H

#include <vector>
#include "stateStruct.h"


namespace modelUtils {

  //Overloaded
  std::vector<double> calcModelParamProbLog(std::vector<stateStruct>& stateList,std::vector<double>& probList);
  std::vector<double> calcModelParamProbLog(std::vector<stateStruct>& stateList,std::vector<double>& probList,std::vector<stateStruct>& modelParamPairs);
  
  std::vector<double> calcModelParamProbLogWOExp(std::vector<stateStruct>& stateList,std::vector<double>& probList,std::vector<stateStruct>& modelParamPairs);

  std::vector<double> calcModelProb(std::vector<stateStruct>& stateList, std::vector<double>& probList);

}
#endif //MODEL_UTILS_H
