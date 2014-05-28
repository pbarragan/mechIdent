//Bayes Filter
//The entire filter is in log space
#include <iostream> // for cout
#include <fstream>
#include <numeric> // TAKE THIS OUT
#include <algorithm> // max_element

#include "bayesFilter.h"
#include "logUtils.h"
#include "translator.h"
#include "filterModels.h"

//Constructor
BayesFilter::BayesFilter(){
 //seed the random number generator once for the filter.
  // srand((unsigned)time(NULL));
}

////////////////////////////////////////////////////////////////////////////////
//                            For SAS Section                                 //
////////////////////////////////////////////////////////////////////////////////

//overload this function
void BayesFilter::transitionUpdateLog(std::vector<double> action, sasUtils::mapPairSVS& sasList){
  transitionUpdateLog(logProbList_, action, sasList);
}

//overload this function - this is the only one that works now
void BayesFilter::transitionUpdateLog(std::vector<double>& logProbList, std::vector<double> action, sasUtils::mapPairSVS& sasList){
	//in here, we need to for each state 
  //1) do a state transition 
  //2) drop a guassian on that as the mean, then for each state
  //3) calculate the probility of that state in that gaussian 
  //4) multiple it by the prev probability of the state in the outer for loop
  //5) sum the probability added to each state as you go through the outer for loop
	//this will require a termporary probability list.


  std::vector< std::vector<double> > tempLogProbListList ( stateList_.size(),std::vector<double> (stateList_.size(),0.0) ); //this will be the sum of the probability after each x_k-1 state

	for (size_t i=0; i<stateList_.size(); i++) {
		//this loop is for x_k-1
		std::vector<double> tempStateLogProbList (stateList_.size(),0.0); //this will hold the probability in the inner loop waiting for normalization

		// SAS: This line changed
		stateStruct nextState = translator::stateTransition(stateList_[i], action, sasList); //this will be the mean of the guassian used to calculate the transition probability
		if (nextState.model == 3){
		  std::cout << "Prismatic 3" << std::endl;
		  std::cout << nextState.model << std::endl;
		  for (size_t j=0;j<nextState.params.size();j++){
		    std::cout << nextState.params[j] << std::endl;
		  }
		  for (size_t j=0;j<nextState.vars.size();j++){
		    std::cout << nextState.vars[j] << std::endl;
		  }
		}

		for (size_t j=0; j<stateList_.size(); j++) {
		  //this loop is for x_k
		  tempStateLogProbList[j] = filterModels::logProbState(stateList_[j],nextState);
		  //std::cout << "probState: " << tempStateProbList[j] << std::endl;
		}
		tempStateLogProbList = logUtils::normalizeVectorInLogSpace(tempStateLogProbList); //normalize the distribution before you scale and add it.

		for (size_t k=0; k<stateList_.size(); k++) {
		  //save a vector of vectors so you can log-sum-exp later to get log(p(x_k|Z_k-1))
			//this loop is for x_k
			tempLogProbListList[k][i] = tempStateLogProbList[k]+logProbList[i];
		}
	}

	//extra step in the log calc. Do log-sum-exp.
	std::vector<double> tempLogProbList (stateList_.size(),0.0);
	for (size_t l=0; l<stateList_.size(); l++){
	  tempLogProbList[l] = logUtils::logSumExp(tempLogProbListList[l]);
	}

	logProbList = tempLogProbList;

	logProbList_T_.clear();
	logProbList_T_ = tempLogProbList; // You added this on 5/7/2014
}

////////////////////////////////////////////////////////////////////////////////
//                          End For SAS Section                               //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                             Update Section                                 //
////////////////////////////////////////////////////////////////////////////////

//overload this function
void BayesFilter::transitionUpdateLog(std::vector<double> action){
  transitionUpdateLog(logProbList_, action);
}

//overload this function
void BayesFilter::transitionUpdateLog(std::vector<double>& logProbList, std::vector<double> action){
	//in here, we need to for each state 
  //1) do a state transition 
  //2) drop a guassian on that as the mean, then for each state
  //3) calculate the probility of that state in that gaussian 
  //4) multiple it by the prev probability of the state in the outer for loop
  //5) sum the probability added to each state as you go through the outer for loop
	//this will require a termporary probability list.


  std::vector< std::vector<double> > tempLogProbListList ( stateList_.size(),std::vector<double> (stateList_.size(),0.0) ); //this will be the sum of the probability after each x_k-1 state

	for (size_t i=0; i<stateList_.size(); i++) {
		//this loop is for x_k-1
		std::vector<double> tempStateLogProbList (stateList_.size(),0.0); //this will hold the probability in the inner loop waiting for normalization

		stateStruct nextState = translator::stateTransition(stateList_[i], action); //this will be the mean of the guassian used to calculate the transition probability
		
		for (size_t j=0; j<stateList_.size(); j++) {
		  //this loop is for x_k
		  tempStateLogProbList[j] = filterModels::logProbState(stateList_[j],nextState);
		  //std::cout << "probState: " << tempStateProbList[j] << std::endl;
		}
		tempStateLogProbList = logUtils::normalizeVectorInLogSpace(tempStateLogProbList); //normalize the distribution before you scale and add it.

		for (size_t k=0; k<stateList_.size(); k++) {
		  //save a vector of vectors so you can log-sum-exp later to get log(p(x_k|Z_k-1))
			//this loop is for x_k
			tempLogProbListList[k][i] = tempStateLogProbList[k]+logProbList[i];
		}
	}

	//extra step in the log calc. Do log-sum-exp.
	std::vector<double> tempLogProbList (stateList_.size(),0.0);
	for (size_t l=0; l<stateList_.size(); l++){
	  tempLogProbList[l] = logUtils::logSumExp(tempLogProbListList[l]);
	}

	logProbList = tempLogProbList;
}

//overload this function
void BayesFilter::observationUpdateLog(std::vector<double> obs){
  observationUpdateLog(logProbList_,obs);
}

//overload this function
void BayesFilter::observationUpdateLog(std::vector<double>& logProbList, std::vector<double> obs){

  logProbList_O_.clear();

	for (size_t i=0; i<stateList_.size(); i++) {
	  double holdP = filterModels::logProbObs(obs,stateList_[i]);
	  logProbList[i] += holdP;
	  logProbList_O_.push_back(holdP); // you added this on 5/7/2014
	}

	//std::cout << "??????????????????????????????????????????? before normalization" << std::endl; // DELETE

	//printLogProbList();
	//std::cout << "obs prob sum: " << std::accumulate(logProbList.begin(),logProbList.end(),0.0) << std::endl; // wrong
	//std::cout << "obs prob sum: " << logUtils::logSumExp(logProbList) << std::endl;

	/*
	// figure out the max probability state at this point
	std::vector<double>::iterator result;
	result = std::max_element(logProbList.begin(),logProbList.end());
	size_t position = std::distance(logProbList.begin(),result);
	std::cout << "max probability state:" << std::endl;
	stateStruct maxState = stateList_[position];
	std::cout << "model: " << maxState.model << std::endl;
	std::cout << "parameters: ";
	for(size_t i=0;i<maxState.params.size();i++){
	  std::cout << maxState.params[i] << ",";
	}
	std::cout << std::endl;
	std::cout << "variables: ";
	for(size_t i=0;i<maxState.vars.size();i++){
	  std::cout << maxState.vars[i] << ",";
	}
	std::cout << std::endl;
	std::cout << "probability: "<< *result << std::endl;
	*/

	logProbList = logUtils::normalizeVectorInLogSpace(logProbList);

}

////////////////////////////////////////////////////////////////////////////////
//                           End  Update Section                              //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                               Aux Section                                  //
////////////////////////////////////////////////////////////////////////////////

void BayesFilter::printLogProbList(){
  std::cout << "Printing Log Probablity List:" << std::endl;
  for (size_t ii = 0; ii<logProbList_.size(); ii++) {
    std::cout << logProbList_[ii] <<std::endl;
  }
}

void BayesFilter::printStateList(){
  //you can make this prettier
  std::cout << "Printing State List:" << std::endl;
  for (size_t ii = 0; ii<stateList_.size(); ii++) {
    std::cout << "Model: " << stateList_[ii].model << std::endl;
    std::cout << "Params: ";
    for (size_t jj = 0; jj<stateList_[ii].params.size(); jj++) {
      std::cout << stateList_[ii].params[jj] << ',';
    }
    std::cout << std::endl;
    std::cout << "Vars: ";
    for (size_t jj = 0; jj<stateList_[ii].vars.size(); jj++) {
      std::cout << stateList_[ii].vars[jj] << ',';
    }
    std::cout << std::endl;
  }
}

void BayesFilter::printStatesAndProbs(){
  std::cout << "Printing States and Probs:" << std::endl;
  for (size_t ii = 0; ii<stateList_.size(); ii++) {
    std::cout << "Model: " << stateList_[ii].model << std::endl;
    std::cout << "Params: ";
    for (size_t jj = 0; jj<stateList_[ii].params.size(); jj++) {
      std::cout << stateList_[ii].params[jj] << ',';
    }
    std::cout << std::endl;
    std::cout << "Vars: ";
    for (size_t jj = 0; jj<stateList_[ii].vars.size(); jj++) {
      std::cout << stateList_[ii].vars[jj] << ',';
    }
    std::cout << std::endl;
    std::cout << "Log Prob: " << logProbList_[ii] <<std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
//                             End Aux Section                                //
////////////////////////////////////////////////////////////////////////////////
