#ifndef SETUP_UTILS_H
#define SETUP_UTILS_H

#include <vector>
#include "stateStruct.h"

namespace setupUtils {
  //models
  std::vector<stateStruct> setupModel0(std::vector<stateStruct>& modelParamPairs);  
  std::vector<stateStruct> setupModel1(std::vector<stateStruct>& modelParamPairs);  
  std::vector<stateStruct> setupModel2(std::vector<stateStruct>& modelParamPairs);  
  std::vector<stateStruct> setupModel3(std::vector<stateStruct>& modelParamPairs);  
  std::vector<stateStruct> setupModel4(std::vector<stateStruct>& modelParamPairs); 
  std::vector<stateStruct> setupModel5(std::vector<stateStruct>& modelParamPairs); 

  std::vector<stateStruct> setupModel6(std::vector<stateStruct>& modelParamPairs); 
  std::vector<stateStruct> setupModel7(std::vector<stateStruct>& modelParamPairs); 
  std::vector<stateStruct> setupModel8(std::vector<stateStruct>& modelParamPairs); 
  std::vector<stateStruct> setupModel9(std::vector<stateStruct>& modelParamPairs); 

  void setupStates(std::vector<stateStruct>& stateList,std::vector<stateStruct>& modelParamPairs);

  void setupModelParamPairs(std::vector<stateStruct>& stateList,std::vector<stateStruct>& modelParamPairs,std::vector<int>& numVarTypesPerStateType);

  //overloaded
  void setupUniformPrior(std::vector<stateStruct>& stateList,std::vector<double>& probList);
  void setupUniformPrior(std::vector<stateStruct>& stateList,std::vector<double>& probList,std::vector<stateStruct>& modelParamPairs);

  //overloaded
  void setupGaussianPrior(std::vector<stateStruct>& stateList,std::vector<double>& probList);
  void setupGaussianPrior(std::vector<stateStruct>& stateList,std::vector<double>& probList,std::vector<stateStruct>& modelParamPairs);

  void setupActions(std::vector< std::vector<double> >& actionList);
  
  void validateStates(std::vector<stateStruct>& stateList,std::vector< std::vector<double> >& workspace);

  void validateActions(std::vector< std::vector<double> >& actionList,std::vector< std::vector<double> >& workspace);

  std::vector<stateStruct> setupModelFromDec(std::vector< std::vector<double> >& dRP,std::vector<int>& dNP,std::vector< std::vector<double> >& dRV,std::vector<int>& dNV,int& modelNum,std::vector<stateStruct>& modelParamPairs);

  std::vector<stateStruct> createStateListForModel(int modelNum,std::vector< std::vector<double> > paramsList,std::vector< std::vector<double> > varsList);

  std::vector< std::vector<double> > dimsToList(std::vector< std::vector<double> > dimRanges, std::vector<int> dimNums);

  std::vector< std::vector<double> > createValueList(std::vector< std::vector<double> > dimRanges, std::vector<int> dimNums);

  std::vector< std::vector<double> > recurseList(std::vector< std::vector<double> > totalList, std::vector<double> oldSeq, int level, std::vector< std::vector<double> > valueList);

}
  
#endif //SETUP_UTILS_H
