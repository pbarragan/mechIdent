#include <algorithm>
#include <numeric>
#include <iostream> // cout

#include "actionSelection.h"
#include "logUtils.h"
#include "translator.h"
#include "modelUtils.h"
#include "setupUtils.h"

// Simplest action selection. Just go through the list.
void actionSelection::chooseActionSimple(std::vector< std::vector<double> >& actionList,int step,std::vector<double>& action){
  action = actionList[step%actionList.size()];
}

// Random action selection.
void actionSelection::chooseActionRandom(std::vector< std::vector<double> >& actionList,std::vector<double>& action){
  size_t ind = rand() % actionList.size();
  action = actionList[ind];
}

// overloaded
// Use an entropy metric to choose the next action
void actionSelection::chooseActionLog(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs){
  //The new method samples from the belief state according to the probability distribution and simulates from those states for each action. An observation is taken for the output state after the simulation. Given the observation, the belief state is updated. The probability distribution over models is calculated. The entropy of this distribution is calculated. The action which produces the lowest entropy is chosen.
  //Step 0: Assume only log probs exist. Exponentiate to get probs.
  std::vector<double> probList = logUtils::expLogProbs(filter.logProbList_);

  //Step 1: Create the CDF of the current belief from the PDF probList_.
  std::vector<double> probCDF = createCDF(probList);

  //Step 2: For each action, sample a state from the belief n times. Simulate this state with the action and get an observation. Update the belief with the action-observation pair. Calculate the entropy of the new belief. Average the entropies over the n samples.
  std::vector<double> avgEntropyList; //this is a list of average entropies, one for each action
  int nSamples = 4; //number of samples of the belief state per action
  //std::cout << "samples" << nSamples << std::endl;
  for (size_t i = 0; i<actionList.size(); i++){
    std::vector<double> entropyList; //this is per action
    for (size_t j = 0; j<nSamples; j++){

      //Step 2.0: Create a copy of the real probability list
      std::vector<double> localLogProbList = filter.logProbList_; //only for this action and sample

      //Step 2.1: Sample a state from the belief
      stateStruct sample = getSampleState(probCDF,filter.stateList_);

      //Step 2.2: Simulate the state with the action
      stateStruct nextState = translator::stateTransition(sample, actionList[i]);

      //Step 2.3: Get an observation
      //Step 2.4: Update the belief state in log space
      filter.transitionUpdateLog(localLogProbList,actionList[i]);
      filter.observationUpdateLog(localLogProbList,getNoisyObs(nextState));

      //Step 2.5: Calculate the entropy over models of the new belief state
      std::vector<double> mpProbs = modelUtils::calcModelParamProbLog(filter.stateList_,localLogProbList,modelParamPairs);
      entropyList.push_back(calcEntropy(mpProbs));
    }
    //Step 2.6: Average the entropies over the n samples
    double eSum = std::accumulate(entropyList.begin(),entropyList.end(),(double) 0.0);
    avgEntropyList.push_back(eSum/entropyList.size());

    // DELETE
    /*
    std::cout << "entropyList: ";
    for (size_t jj=0;jj<entropyList.size();jj++){
      std::cout << entropyList[jj] << ",";
    }
    std::cout << std::endl;
    */
  }
  
  // DELETE
  /*
  std::cout << "avgEntropyList: ";
  for (size_t jj=0;jj<avgEntropyList.size();jj++){
    std::cout << avgEntropyList[jj] << ",";
  }
  std::cout << std::endl;
  */

  //Step 3: Choose the action which results in the lowest entropy updated belief
  std::vector<double>::iterator minAvgEntIt = std::min_element(avgEntropyList.begin(),avgEntropyList.end());
  action = actionList[std::distance(avgEntropyList.begin(),minAvgEntIt)];
}

// overloaded
// Use an entropy metric to choose the next action
// Uses SAS list
void actionSelection::chooseActionLog(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,sasUtils::mapPairSVS& sasList){
  //The new method samples from the belief state according to the probability distribution and simulates from those states for each action. An observation is taken for the output state after the simulation. Given the observation, the belief state is updated. The probability distribution over models is calculated. The entropy of this distribution is calculated. The action which produces the lowest entropy is chosen.
  //Step 0: Assume only log probs exist. Exponentiate to get probs.
  std::vector<double> probList = logUtils::expLogProbs(filter.logProbList_);

  //Step 1: Create the CDF of the current belief from the PDF probList_.
  std::vector<double> probCDF = createCDF(probList);

  //Step 2: For each action, sample a state from the belief n times. Simulate this state with the action and get an observation. Update the belief with the action-observation pair. Calculate the entropy of the new belief. Average the entropies over the n samples.
  std::vector<double> avgEntropyList; //this is a list of average entropies, one for each action
  int nSamples = 4; //number of samples of the belief state per action
  //std::cout << "samples" << nSamples << std::endl;
  for (size_t i = 0; i<actionList.size(); i++){
    std::vector<double> entropyList; //this is per action
    for (size_t j = 0; j<nSamples; j++){

      //Step 2.0: Create a copy of the real probability list
      std::vector<double> localLogProbList = filter.logProbList_; //only for this action and sample

      //Step 2.1: Sample a state from the belief
      stateStruct sample = getSampleState(probCDF,filter.stateList_);

      //Step 2.2: Simulate the state with the action
      stateStruct nextState = translator::stateTransition(sample,actionList[i],sasList); // use SAS

      //Step 2.3: Get an observation
      //Step 2.4: Update the belief state in log space
      filter.transitionUpdateLog(localLogProbList,actionList[i],sasList); // use SAS
      filter.observationUpdateLog(localLogProbList,getNoisyObs(nextState));

      //Step 2.5: Calculate the entropy over models of the new belief state
      std::vector<double> mpProbs = modelUtils::calcModelParamProbLog(filter.stateList_,localLogProbList,modelParamPairs);
      entropyList.push_back(calcEntropy(mpProbs));

    }
    //Step 2.6: Average the entropies over the n samples
    double eSum = std::accumulate(entropyList.begin(),entropyList.end(),(double) 0.0);
    avgEntropyList.push_back(eSum/entropyList.size());

    // DELETE
    /*
    std::cout << "entropyList: ";
    for (size_t jj=0;jj<entropyList.size();jj++){
      std::cout << entropyList[jj] << ",";
    }
    std::cout << std::endl;
    */
  }

  // DELETE
  /*
  std::cout << "avgEntropyList: ";
  for (size_t jj=0;jj<avgEntropyList.size();jj++){
    std::cout << avgEntropyList[jj] << ",";
  }
  std::cout << std::endl;
  */

  //Step 3: Choose the action which results in the lowest entropy updated belief
  std::vector<double>::iterator minAvgEntIt = std::min_element(avgEntropyList.begin(),avgEntropyList.end());
  action = actionList[std::distance(avgEntropyList.begin(),minAvgEntIt)];
}

// overloaded
// No SAS list
void actionSelection::chooseActionOG(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs){
  // OG == Original Gangster
  // The original method was simply to run a bunch of actions on the top two models in terms of probability and try to determine which action produced the most different outcome and then choose that one.
  // this will only work if there is at least two models
  // and the probabilities better be positive

  // Step 1: Calculate all the model-parameter probabilities so you know which models to compare (the highest two)
  std::vector<double> mpProbs = modelUtils::calcModelParamProbLog(filter.stateList_,filter.logProbList_,modelParamPairs);

  if (mpProbs.size()<2){
    std::cout << "At least 2 model-parameter pairs needed" << std::endl;
  }
  else {
    double first=-1.0;
    double second=-1.0;
    int firstIndex=-1;
    int secondIndex=-1;
    
    for (size_t i = 0; i<mpProbs.size(); i++){
      if (mpProbs[i]>first){
	second = first;
	secondIndex = firstIndex;
	first = mpProbs[i];
	firstIndex = i;
      }
      else if (mpProbs[i]>second){
	second = mpProbs[i];
	secondIndex = i;
      }
    }

    int firstModel = modelParamPairs[firstIndex].model;
    int secondModel = modelParamPairs[secondIndex].model;

    //std::cout << "First model: " << firstModel << std::endl;
    //std::cout << "Second model: " << secondModel << std::endl;
    
    // Step 2: Figure out the maximum probability state for those models to simulate from 
    // (THIS IS IN LOG SPACE)
    stateStruct firstState;
    double firstStateProb;
    stateStruct secondState;
    double secondStateProb;
    bool foundFirst = false;
    bool foundSecond = false;
    
    for (size_t i = 0; i<filter.stateList_.size(); i++){
      if (filter.stateList_[i].model == firstModel){
	if (foundFirst == false){
	  firstState = filter.stateList_[i];
	  firstStateProb = filter.logProbList_[i];
	  foundFirst = true;
	}
	else if (filter.logProbList_[i] > firstStateProb){
	  firstState = filter.stateList_[i];
	  firstStateProb = filter.logProbList_[i];
	}
      }
      else if (filter.stateList_[i].model == secondModel){
	if (foundSecond == false){
	  secondState = filter.stateList_[i];
	  secondStateProb = filter.logProbList_[i];
	  foundSecond = true;
	}
	else if (filter.logProbList_[i] > secondStateProb){
	  secondState = filter.stateList_[i];
	  secondStateProb = filter.logProbList_[i];
	}
      }
    }
    
    // Step 3: Simulate all the actions from the states found in Step 2 + 
    // Step 4: Calculate distances between results from Step 3 and determine which is greatest
    stateStruct tempFirstNextState;
    stateStruct tempSecondNextState;
    double furthestDist = -1.0;
    double currentDist = -2.0;
    std::vector<double> bestAction;
    
    for (size_t i=0; i<actionList.size(); i++){
      tempFirstNextState = translator::stateTransition(firstState,actionList[i]);
      tempSecondNextState = translator::stateTransition(secondState,actionList[i]);
      currentDist = distPointsSquared(translator::translateStToObs(tempFirstNextState),translator::translateStToObs(tempSecondNextState));

      if (currentDist > furthestDist){
	furthestDist = currentDist;
	bestAction = actionList[i];
      }
    }
    
    // Step 5: Set the next action to do to the best action found
    action = bestAction;
  }
}

// overloaded
// Use the SAS list
void actionSelection::chooseActionOG(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,sasUtils::mapPairSVS& sasList){
  //OG == Original Gangster
  //The original method was simply to run a bunch of actions on the top two models in terms of probability and try to determine which action produced the most different outcome and then choose that one.
  //this will only work if there is at least two models
  //and the probabilities better be positive

  //Step 1: Calculate all the model-parameter probabilities so you know which models to compare (the highest two)
  //std::vector<double> modelProbs = calcModelProb();
  std::vector<double> mpProbs = modelUtils::calcModelParamProbLog(filter.stateList_,filter.logProbList_,modelParamPairs);

  if (mpProbs.size()<2){
    std::cout << "At least 2 model-parameter pairs needed" << std::endl;
  }
  else {
    //std::cout << "Let's choose a model" << std::endl;
    double first=-1.0;
    double second=-1.0;
    int firstIndex=-1;
    int secondIndex=-1;
    
    for (size_t i = 0; i<mpProbs.size(); i++){
      if (mpProbs[i]>first){
	second = first;
	secondIndex = firstIndex;
	first = mpProbs[i];
	firstIndex = i;
      }
      else if (mpProbs[i]>second){
	second = mpProbs[i];
	secondIndex = i;
      }
    }

    int firstModel = modelParamPairs[firstIndex].model;
    int secondModel = modelParamPairs[secondIndex].model;

    //std::cout << "First model: " << firstModel << std::endl;
    //std::cout << "Second model: " << secondModel << std::endl;
    
    // Step 2: Figure out the maximum probability state for those models to simulate from 
    // (THIS IS IN LOG SPACE)
    stateStruct firstState;
    double firstStateProb;
    stateStruct secondState;
    double secondStateProb;
    bool foundFirst = false;
    bool foundSecond = false;
    
    for (size_t i = 0; i<filter.stateList_.size(); i++){
      if (filter.stateList_[i].model == firstModel){
	if (foundFirst == false){
	  firstState = filter.stateList_[i];
	  firstStateProb = filter.logProbList_[i];
	  foundFirst = true;
	}
	else if (filter.logProbList_[i] > firstStateProb){
	  firstState = filter.stateList_[i];
	  firstStateProb = filter.logProbList_[i];
	}
      }
      else if (filter.stateList_[i].model == secondModel){
	if (foundSecond == false){
	  secondState = filter.stateList_[i];
	  secondStateProb = filter.logProbList_[i];
	  foundSecond = true;
	}
	else if (filter.logProbList_[i] > secondStateProb){
	  secondState = filter.stateList_[i];
	  secondStateProb = filter.logProbList_[i];
	}
      }
    }

    //Step 3: Simulate all the actions from the states found in Step 2 + 
    //Step 4: Calculate distances between results from Step 3 and determine which is greatest
    stateStruct tempFirstNextState;
    stateStruct tempSecondNextState;
    double furthestDist = -1.0;
    double currentDist = -2.0;
    std::vector<double> bestAction;
    
    //std::cout << "error is righhhhhhht here:" << std::endl;

    for (size_t i=0; i<actionList.size(); i++){
      tempFirstNextState = translator::stateTransition(firstState,actionList[i],sasList); // use SAS
      tempSecondNextState = translator::stateTransition(secondState,actionList[i],sasList); // use SAS
      currentDist = distPointsSquared(translator::translateStToObs(tempFirstNextState),translator::translateStToObs(tempSecondNextState));

      if (currentDist > furthestDist){
	furthestDist = currentDist;
	bestAction = actionList[i];
      }
    }

    //std::cout << "def not here" << std::endl;

    //Step 5: Set the next action to do to the best action found
    action = bestAction;
  }
}

////////////////////////////////////////////////////////////////////////////////
//                             Relative Section                               //
////////////////////////////////////////////////////////////////////////////////

//std::vector<double> poseInRbt should be what is passed

// Simplest action selection. Just go through the list. Doesn't really make sense
void actionSelection::chooseActionSimpleRel(std::vector< std::vector<double> >& actionList,int step,std::vector<double>& action,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace){

  std::vector< std::vector<double> > validRelActionList;
  validateRelActionList(actionList,poseInRbt,workspace,validRelActionList);

  action = validRelActionList[step%validRelActionList.size()]; // select action
  
  // DELETE THIS
  action = actionList[4];
}

// Random action selection.
void actionSelection::chooseActionRandomRel(std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace){

  std::vector< std::vector<double> > validRelActionList;
  validateRelActionList(actionList,poseInRbt,workspace,validRelActionList);

  size_t ind = rand() % validRelActionList.size();
  action = validRelActionList[ind];
}

// overloaded
// Use an entropy metric to choose the next action
void actionSelection::chooseActionLogRel(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace){
  //The new method samples from the belief state according to the probability distribution and simulates from those states for each action. An observation is taken for the output state after the simulation. Given the observation, the belief state is updated. The probability distribution over models is calculated. The entropy of this distribution is calculated. The action which produces the lowest entropy is chosen.

  //Step -1: Validate relative action list
  std::vector< std::vector<double> > validRelActionList;
  validateRelActionList(actionList,poseInRbt,workspace,validRelActionList);

  //Step 0: Assume only log probs exist. Exponentiate to get probs.
  std::vector<double> probList = logUtils::expLogProbs(filter.logProbList_);

  //Step 1: Create the CDF of the current belief from the PDF probList_.
  std::vector<double> probCDF = createCDF(probList);

  //Step 2: For each action, sample a state from the belief n times. Simulate this state with the action and get an observation. Update the belief with the action-observation pair. Calculate the entropy of the new belief. Average the entropies over the n samples.
  std::vector<double> avgEntropyList; //this is a list of average entropies, one for each action
  int nSamples = 4; //number of samples of the belief state per action
  //std::cout << "samples" << nSamples << std::endl;
  for (size_t i = 0; i<validRelActionList.size(); i++){
    std::vector<double> entropyList; //this is per action
    for (size_t j = 0; j<nSamples; j++){

      //Step 2.0: Create a copy of the real probability list
      std::vector<double> localLogProbList = filter.logProbList_; //only for this action and sample

      //Step 2.1: Sample a state from the belief
      stateStruct sample = getSampleState(probCDF,filter.stateList_);

      //Step 2.2: Simulate the state with the action
      stateStruct nextState = translator::stateTransition(sample, validRelActionList[i]);

      //Step 2.3: Get an observation
      //Step 2.4: Update the belief state in log space
      filter.transitionUpdateLog(localLogProbList,validRelActionList[i]);
      filter.observationUpdateLog(localLogProbList,getNoisyObs(nextState));

      //Step 2.5: Calculate the entropy over models of the new belief state
      std::vector<double> mpProbs = modelUtils::calcModelParamProbLog(filter.stateList_,localLogProbList,modelParamPairs);
      entropyList.push_back(calcEntropy(mpProbs));
    }
    //Step 2.6: Average the entropies over the n samples
    double eSum = std::accumulate(entropyList.begin(),entropyList.end(),(double) 0.0);
    avgEntropyList.push_back(eSum/entropyList.size());

    // DELETE
    /*
    std::cout << "entropyList: ";
    for (size_t jj=0;jj<entropyList.size();jj++){
      std::cout << entropyList[jj] << ",";
    }
    std::cout << std::endl;
    */
  }
  
  // DELETE
  /*
  std::cout << "avgEntropyList: ";
  for (size_t jj=0;jj<avgEntropyList.size();jj++){
    std::cout << avgEntropyList[jj] << ",";
  }
  std::cout << std::endl;
  */

  //Step 3: Choose the action which results in the lowest entropy updated belief
  std::vector<double>::iterator minAvgEntIt = std::min_element(avgEntropyList.begin(),avgEntropyList.end());
  action = validRelActionList[std::distance(avgEntropyList.begin(),minAvgEntIt)];
}

// overloaded
// Use an entropy metric to choose the next action
// Uses SAS list
void actionSelection::chooseActionLogRel(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,sasUtils::mapPairSVS& sasList,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace){
  //The new method samples from the belief state according to the probability distribution and simulates from those states for each action. An observation is taken for the output state after the simulation. Given the observation, the belief state is updated. The probability distribution over models is calculated. The entropy of this distribution is calculated. The action which produces the lowest entropy is chosen.

  //Step -1: Validate relative action list
  std::vector< std::vector<double> > validRelActionList;
  validateRelActionList(actionList,poseInRbt,workspace,validRelActionList);

  //Step 0: Assume only log probs exist. Exponentiate to get probs.
  std::vector<double> probList = logUtils::expLogProbs(filter.logProbList_);

  //Step 1: Create the CDF of the current belief from the PDF probList_.
  std::vector<double> probCDF = createCDF(probList);

  //Step 2: For each action, sample a state from the belief n times. Simulate this state with the action and get an observation. Update the belief with the action-observation pair. Calculate the entropy of the new belief. Average the entropies over the n samples.
  std::vector<double> avgEntropyList; //this is a list of average entropies, one for each action
  int nSamples = 4; //number of samples of the belief state per action
  //std::cout << "samples" << nSamples << std::endl;
  for (size_t i = 0; i<validRelActionList.size(); i++){
    std::vector<double> entropyList; //this is per action
    for (size_t j = 0; j<nSamples; j++){

      //Step 2.0: Create a copy of the real probability list
      std::vector<double> localLogProbList = filter.logProbList_; //only for this action and sample

      //Step 2.1: Sample a state from the belief
      stateStruct sample = getSampleState(probCDF,filter.stateList_);

      //Step 2.2: Simulate the state with the action
      stateStruct nextState = translator::stateTransition(sample,validRelActionList[i],sasList); // use SAS

      //Step 2.3: Get an observation
      //Step 2.4: Update the belief state in log space
      filter.transitionUpdateLog(localLogProbList,validRelActionList[i],sasList); // use SAS
      filter.observationUpdateLog(localLogProbList,getNoisyObs(nextState));

      //Step 2.5: Calculate the entropy over models of the new belief state
      std::vector<double> mpProbs = modelUtils::calcModelParamProbLog(filter.stateList_,localLogProbList,modelParamPairs);
      entropyList.push_back(calcEntropy(mpProbs));

    }
    //Step 2.6: Average the entropies over the n samples
    double eSum = std::accumulate(entropyList.begin(),entropyList.end(),(double) 0.0);
    avgEntropyList.push_back(eSum/entropyList.size());

    // DELETE
    /*
    std::cout << "entropyList: ";
    for (size_t jj=0;jj<entropyList.size();jj++){
      std::cout << entropyList[jj] << ",";
    }
    std::cout << std::endl;
    */
  }

  // DELETE
  /*
  std::cout << "avgEntropyList: ";
  for (size_t jj=0;jj<avgEntropyList.size();jj++){
    std::cout << avgEntropyList[jj] << ",";
  }
  std::cout << std::endl;
  */

  //Step 3: Choose the action which results in the lowest entropy updated belief
  std::vector<double>::iterator minAvgEntIt = std::min_element(avgEntropyList.begin(),avgEntropyList.end());
  action = validRelActionList[std::distance(avgEntropyList.begin(),minAvgEntIt)];
}

// overloaded
// No SAS list
void actionSelection::chooseActionOGRel(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace){
  // OG == Original Gangster
  // The original method was simply to run a bunch of actions on the top two models in terms of probability and try to determine which action produced the most different outcome and then choose that one.
  // this will only work if there is at least two models
  // and the probabilities better be positive

  //Step -1: Validate relative action list
  std::vector< std::vector<double> > validRelActionList;
  validateRelActionList(actionList,poseInRbt,workspace,validRelActionList);

  // Step 1: Calculate all the model-parameter probabilities so you know which models to compare (the highest two)
  std::vector<double> mpProbs = modelUtils::calcModelParamProbLog(filter.stateList_,filter.logProbList_,modelParamPairs);

  if (mpProbs.size()<2){
    std::cout << "At least 2 model-parameter pairs needed" << std::endl;
  }
  else {
    double first=-1.0;
    double second=-1.0;
    int firstIndex=-1;
    int secondIndex=-1;
    
    for (size_t i = 0; i<mpProbs.size(); i++){
      if (mpProbs[i]>first){
	second = first;
	secondIndex = firstIndex;
	first = mpProbs[i];
	firstIndex = i;
      }
      else if (mpProbs[i]>second){
	second = mpProbs[i];
	secondIndex = i;
      }
    }

    int firstModel = modelParamPairs[firstIndex].model;
    int secondModel = modelParamPairs[secondIndex].model;

    //std::cout << "First model: " << firstModel << std::endl;
    //std::cout << "Second model: " << secondModel << std::endl;
    
    // Step 2: Figure out the maximum probability state for those models to simulate from 
    // (THIS IS IN LOG SPACE)
    stateStruct firstState;
    double firstStateProb;
    stateStruct secondState;
    double secondStateProb;
    bool foundFirst = false;
    bool foundSecond = false;
    
    for (size_t i = 0; i<filter.stateList_.size(); i++){
      if (filter.stateList_[i].model == firstModel){
	if (foundFirst == false){
	  firstState = filter.stateList_[i];
	  firstStateProb = filter.logProbList_[i];
	  foundFirst = true;
	}
	else if (filter.logProbList_[i] > firstStateProb){
	  firstState = filter.stateList_[i];
	  firstStateProb = filter.logProbList_[i];
	}
      }
      else if (filter.stateList_[i].model == secondModel){
	if (foundSecond == false){
	  secondState = filter.stateList_[i];
	  secondStateProb = filter.logProbList_[i];
	  foundSecond = true;
	}
	else if (filter.logProbList_[i] > secondStateProb){
	  secondState = filter.stateList_[i];
	  secondStateProb = filter.logProbList_[i];
	}
      }
    }
    
    // Step 3: Simulate all the actions from the states found in Step 2 + 
    // Step 4: Calculate distances between results from Step 3 and determine which is greatest
    stateStruct tempFirstNextState;
    stateStruct tempSecondNextState;
    double furthestDist = -1.0;
    double currentDist = -2.0;
    std::vector<double> bestAction;
    
    for (size_t i=0; i<validRelActionList.size(); i++){
      tempFirstNextState = translator::stateTransition(firstState,validRelActionList[i]);
      tempSecondNextState = translator::stateTransition(secondState,validRelActionList[i]);
      currentDist = distPointsSquared(translator::translateStToObs(tempFirstNextState),translator::translateStToObs(tempSecondNextState));

      if (currentDist > furthestDist){
	furthestDist = currentDist;
	bestAction = validRelActionList[i];
      }
    }
    
    // Step 5: Set the next action to do to the best action found
    action = bestAction;
  }
}

// overloaded
// Use the SAS list
void actionSelection::chooseActionOGRel(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,sasUtils::mapPairSVS& sasList,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace){
  //OG == Original Gangster
  //The original method was simply to run a bunch of actions on the top two models in terms of probability and try to determine which action produced the most different outcome and then choose that one.
  //this will only work if there is at least two models
  //and the probabilities better be positive

  //Step -1: Validate relative action list
  std::vector< std::vector<double> > validRelActionList;
  validateRelActionList(actionList,poseInRbt,workspace,validRelActionList);

  //Step 1: Calculate all the model-parameter probabilities so you know which models to compare (the highest two)
  //std::vector<double> modelProbs = calcModelProb();
  std::vector<double> mpProbs = modelUtils::calcModelParamProbLog(filter.stateList_,filter.logProbList_,modelParamPairs);

  if (mpProbs.size()<2){
    std::cout << "At least 2 model-parameter pairs needed" << std::endl;
  }
  else {
    //std::cout << "Let's choose a model" << std::endl;
    double first=-1.0;
    double second=-1.0;
    int firstIndex=-1;
    int secondIndex=-1;
    
    for (size_t i = 0; i<mpProbs.size(); i++){
      if (mpProbs[i]>first){
	second = first;
	secondIndex = firstIndex;
	first = mpProbs[i];
	firstIndex = i;
      }
      else if (mpProbs[i]>second){
	second = mpProbs[i];
	secondIndex = i;
      }
    }

    int firstModel = modelParamPairs[firstIndex].model;
    int secondModel = modelParamPairs[secondIndex].model;

    std::vector<double> firstParams = modelParamPairs[firstIndex].params;
    std::vector<double> secondParams = modelParamPairs[secondIndex].params;
    
    // Step 2: Figure out the maximum probability state for those models to simulate from 
    // (THIS IS IN LOG SPACE)
    stateStruct firstState;
    double firstStateProb;
    stateStruct secondState;
    double secondStateProb;
    bool foundFirst = false;
    bool foundSecond = false;
    
    for (size_t i = 0; i<filter.stateList_.size(); i++){
      if (filter.stateList_[i].model == firstModel && filter.stateList_[i].params == firstParams){
	if (foundFirst == false){
	  firstState = filter.stateList_[i];
	  firstStateProb = filter.logProbList_[i];
	  foundFirst = true;
	}
	else if (filter.logProbList_[i] > firstStateProb){
	  firstState = filter.stateList_[i];
	  firstStateProb = filter.logProbList_[i];
	}
      }
      else if (filter.stateList_[i].model == secondModel && filter.stateList_[i].params == secondParams){
	if (foundSecond == false){
	  secondState = filter.stateList_[i];
	  secondStateProb = filter.logProbList_[i];
	  foundSecond = true;
	}
	else if (filter.logProbList_[i] > secondStateProb){
	  secondState = filter.stateList_[i];
	  secondStateProb = filter.logProbList_[i];
	}
      }
    }

    //Step 3: Simulate all the actions from the states found in Step 2 + 
    //Step 4: Calculate distances between results from Step 3 and determine which is greatest
    stateStruct tempFirstNextState;
    stateStruct tempSecondNextState;
    double furthestDist = -1.0;
    double currentDist = -2.0;
    std::vector<double> bestAction;
    
    //std::cout << "error is righhhhhhht here:" << std::endl;


    for (size_t i=0; i<validRelActionList.size(); i++){
      tempFirstNextState = translator::stateTransition(firstState,validRelActionList[i],sasList); // use SAS
      tempSecondNextState = translator::stateTransition(secondState,validRelActionList[i],sasList); // use SAS
      currentDist = distPointsSquared(translator::translateStToObs(tempFirstNextState),translator::translateStToObs(tempSecondNextState));

      if (currentDist > furthestDist){
	furthestDist = currentDist;
	bestAction = validRelActionList[i];
      }
    }

    //std::cout << "def not here" << std::endl;

    //Step 5: Set the next action to do to the best action found
    action = bestAction;
  }
}

////////////////////////////////////////////////////////////////////////////////
//                           End Relative Section                             //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                               Aux Section                                  //
////////////////////////////////////////////////////////////////////////////////

std::vector<double> actionSelection::getNoisyObs(stateStruct& state){
  // maybe do something about srand later. not calling now on purpose.
  std::vector<double> obs = translator::translateStToObs(state);
  // add some noise
  for (size_t i=0; i<obs.size(); i++){
    double X = randomDouble();
    obs[i]+=0.001*X; // add noise to each element
  }
  return obs;
}

std::vector<double> actionSelection::createCDF(std::vector<double>& probList){
  // this takes probabilities in regular space (not log space)
  std::vector<double> probCDF;
  probCDF.push_back(probList[0]); //initialize the first entry for the list
  for (size_t i=1; i<probList.size(); i++){
    probCDF.push_back(probCDF[i-1]+probList[i]);
  }
  return probCDF;
}

stateStruct actionSelection::getSampleState(std::vector<double>& CDF, std::vector<stateStruct>& states){
  // uniformly sample a state
  double X = randomDouble();
  std::vector<double>::iterator low = std::lower_bound(CDF.begin(),CDF.end(),X);
  return states[std::distance(CDF.begin(),low)];
}

double actionSelection::calcEntropy(std::vector<double> probs){
  // this takes probabilities in regular space (not log space)
  double sum=0;
  for (size_t i=0; i<probs.size(); i++){
    if (probs[i]!=0.0){
      sum += probs[i]*log(probs[i]);
    }
  }
  return -sum;
}

double actionSelection::randomDouble(){
  double X = ((double)rand()/(double)RAND_MAX);
  return X;
}

double actionSelection::distPointsSquared(std::vector<double> a, std::vector<double> b){
  //assumes 2d distances
  return (a[0]-b[0])*(a[0]-b[0])+(a[1]-b[1])*(a[1]-b[1]);
}

void actionSelection::relToAbsActionList(std::vector< std::vector<double> >& relActionList,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& absActionList){
  absActionList.clear(); // Clear anything in here
  std::vector<double> tempAction;
  for (size_t i=0;i<relActionList.size();i++){
    for (size_t j=0;j<poseInRbt.size();j++){
      tempAction.push_back(poseInRbt[j]+relActionList[i][j]);
    }
    absActionList.push_back(tempAction);
  }
}

void actionSelection::absToRelActionList(std::vector< std::vector<double> >& absActionList,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& relActionList){
  relActionList.clear(); // Clear anything in here
  std::vector<double> tempAction;
  for (size_t i=0;i<absActionList.size();i++){
    for (size_t j=0;j<poseInRbt.size();j++){
      tempAction.push_back(absActionList[i][j]-poseInRbt[j]);
    }
    relActionList.push_back(tempAction);
  }
}

void actionSelection::absToRelAction(std::vector<double>& tempAbsAction,std::vector<double>& poseInRbt,std::vector<double>& tempRelAction){
  tempRelAction.clear();
  for (size_t i=0;i<tempAbsAction.size();i++){
    tempRelAction.push_back(tempAbsAction[i]-poseInRbt[i]);
  }
}


void actionSelection::validateRelActionList(std::vector< std::vector<double> >& relActionList,std::vector<double>& poseInRbt,std::vector< std::vector<double> >& workspace,std::vector< std::vector<double> >& validRelActionList){

  /*
  std::vector< std::vector<double> > validAbsActionList;
  //std::vector<double> stateInRbt = translator::translateStToRbt(state); // Convert state to rbt coordinates

  relToAbsActionList(relActionList,poseInRbt,validAbsActionList); // convert rel to abs actions
  setupUtils::validateActions(validAbsActionList,workspace); // validate actions: action list is mutated to only be valid actions
  absToRelActionList(validAbsActionList,poseInRbt,validRelActionList); // convert abs to rel actions

  // All workspace stuff right now assumes a 2D workspace - FIX
  // Actions must be within the workspace
  */

  validRelActionList.clear();
  for (size_t i=0;i<relActionList.size();i++){
    if (!((poseInRbt[0]+relActionList[i][0])<workspace[0][0] || 
	  (poseInRbt[0]+relActionList[i][0])>workspace[0][1] || 
	  (poseInRbt[1]+relActionList[i][1])<workspace[1][0] || 
	  (poseInRbt[1]+relActionList[i][1])>workspace[1][1])){
      validRelActionList.push_back(relActionList[i]);
    }
  }

}


////////////////////////////////////////////////////////////////////////////////
//                             End Aux Section                                //
////////////////////////////////////////////////////////////////////////////////

