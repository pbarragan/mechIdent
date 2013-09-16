#include <algorithm>
#include <numeric>
#include <iostream> // cout

#include "actionSelection.h"
#include "logUtils.h"
#include "translator.h"
#include "modelUtils.h"

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
    std::cout << "entropyList: ";
    for (size_t jj=0;jj<entropyList.size();jj++){
      std::cout << entropyList[jj] << ",";
    }
    std::cout << std::endl;

  }
  
  // DELETE
  std::cout << "avgEntropyList: ";
  for (size_t jj=0;jj<avgEntropyList.size();jj++){
    std::cout << avgEntropyList[jj] << ",";
  }
  std::cout << std::endl;

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
  std::cout << "samples" << nSamples << std::endl;
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
    std::cout << "entropyList: ";
    for (size_t jj=0;jj<entropyList.size();jj++){
      std::cout << entropyList[jj] << ",";
    }
    std::cout << std::endl;

  }

  // DELETE
  std::cout << "avgEntropyList: ";
  for (size_t jj=0;jj<avgEntropyList.size();jj++){
    std::cout << avgEntropyList[jj] << ",";
  }
  std::cout << std::endl;

  //Step 3: Choose the action which results in the lowest entropy updated belief
  std::vector<double>::iterator minAvgEntIt = std::min_element(avgEntropyList.begin(),avgEntropyList.end());
  action = actionList[std::distance(avgEntropyList.begin(),minAvgEntIt)];
}

////////////////////////////////////////////////////////////////////////////////
//                               Aux Section                                  //
////////////////////////////////////////////////////////////////////////////////

std::vector<double> actionSelection::getNoisyObs(stateStruct& state){
  // maybe do something about srand later. nor calling now on purpose.
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

////////////////////////////////////////////////////////////////////////////////
//                             End Aux Section                                //
////////////////////////////////////////////////////////////////////////////////

