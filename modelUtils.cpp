//Model Utilities
#include "modelUtils.h"
#include "logUtils.h"

//Overloaded
//Returns the model-param probabilities when given a list of probabilities in log space
std::vector<double> modelUtils::calcModelParamProbLog(std::vector<stateStruct>& stateList,std::vector<double>& probList){

  //just a little test
  //std::vector<double> probList = logUtils::expLogProbs(logProbList);

  //return calcModelProb(probList);

  //1. Find different instances and collect their probabilities
  //A state type is a model-parameter pair
  std::vector<stateStruct> foundStateTypes; //how many different model-parameter pairs
  std::vector< std::vector<double> > sumHoldVect; //hold list of log probabilities for the log-sum-exp
  bool addStateType;
  for (size_t i=0; i<stateList.size(); i++){
    addStateType = true;
    for (size_t j=0; j<foundStateTypes.size(); j++){
      if (stateList[i].model == foundStateTypes[j].model && stateList[i].params == foundStateTypes[j].params){
	addStateType = false;
	sumHoldVect[j].push_back(probList[i]);
	break; //the break assumes we didn't somehow add the same pair twice to the found list
      }
    }
    if (addStateType){
      foundStateTypes.push_back(stateList[i]);
      sumHoldVect.push_back(std::vector<double> (1,probList[i]));
    }
  }

  //2. Log-sum-exp the lists created to get the actual probabilities for the different types
  std::vector<double> sumHold (sumHoldVect.size(),0.0);

  for (size_t j=0; j<sumHoldVect.size(); j++){
    sumHold[j] = logUtils::safe_exp(logUtils::logSumExp(sumHoldVect[j]));
  }

  return sumHold;
}

//Overloaded - this one has the model-parameter pairs passed to it from the state setup function
//Returns the model-param probabilities when given a list of probabilities in log space
std::vector<double> modelUtils::calcModelParamProbLog(std::vector<stateStruct>& stateList,std::vector<double>& probList,std::vector<stateStruct>& modelParamPairs){

  // 1. Find different instances and collect their probabilities
  // A state type is a model-parameter pair
  std::vector< std::vector<double> > sumHoldVect (modelParamPairs.size(),std::vector<double> ()); // hold list of log probabilities for the log-sum-exp
  for (size_t i=0; i<stateList.size(); i++){
    for (size_t j=0; j<modelParamPairs.size(); j++){
      if (stateList[i].model == modelParamPairs[j].model && stateList[i].params == modelParamPairs[j].params){
	sumHoldVect[j].push_back(probList[i]);
	break; // the break assumes we didn't somehow add the same pair twice to the found list
      }
    }
  }

  // 2. Log-sum-exp the lists created to get the log probabilities for the different types and then exponentiate.
  std::vector<double> sumHold (sumHoldVect.size(),0.0);

  for (size_t j=0; j<sumHoldVect.size(); j++){
    sumHold[j] = logUtils::safe_exp(logUtils::logSumExp(sumHoldVect[j]));
  }

  return sumHold;
}

std::vector<double> modelUtils::calcModelParamProbLogWOExp(std::vector<stateStruct>& stateList,std::vector<double>& probList,std::vector<stateStruct>& modelParamPairs){

  // 1. Find different instances and collect their probabilities
  // A state type is a model-parameter pair
  std::vector< std::vector<double> > sumHoldVect (modelParamPairs.size(),std::vector<double> ()); // hold list of log probabilities for the log-sum-exp
  for (size_t i=0; i<stateList.size(); i++){
    for (size_t j=0; j<modelParamPairs.size(); j++){
      if (stateList[i].model == modelParamPairs[j].model && stateList[i].params == modelParamPairs[j].params){
	sumHoldVect[j].push_back(probList[i]);
	break; // the break assumes we didn't somehow add the same pair twice to the found list
      }
    }
  }

  // 2. Log-sum-exp the lists created to get the log probabilities for the different types and then exponentiate.
  std::vector<double> sumHold (sumHoldVect.size(),0.0);

  for (size_t j=0; j<sumHoldVect.size(); j++){
    sumHold[j] = logUtils::logSumExp(sumHoldVect[j]);
  }

  return sumHold;
}


// Not used
// Calculate model probabilities from a set of probabilities in normal probability space
std::vector<double> modelUtils::calcModelProb(std::vector<stateStruct>& stateList, std::vector<double>& probList){
  
  std::vector<double> sumHold (5,0.0);  
  for (size_t i=0; i<stateList.size(); i++){
    sumHold[stateList[i].model] += probList[i];
  }  
  return sumHold;  
}
