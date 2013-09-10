#include <iostream>
#include <fstream>
#include <sstream>
#include "sasUtils.h"
#include "translator.h"

//this is super specific to the exact state representation I am using as of 9/4/2013 (Nate and Julia's Wedding Anniversary and Frank's Birthday)

////////////////////////////////////////////////////////////////////////////////
//                              Setup Section                                 //
////////////////////////////////////////////////////////////////////////////////

sasUtils::mapPairSVS sasUtils::populateSAS(std::vector<stateStruct>& states, std::vector< std::vector<double> >& actions){
  std::cout << "Populating the SAS map from scratch" << std::endl;
  sasUtils::mapPairSVS sasHolder;
  double total = states.size()*actions.size();
  int statesSize = states.size();
  for (size_t i=0;i<actions.size();i++){
    for (size_t j=0;j<states.size();j++){
      stateStruct nextState = translator::stateTransition(states[j],actions[i]);
      sasUtils::insertIntoSAS(sasHolder,states[j],actions[i],nextState);
      if((i*statesSize+j) % 50 == 0) std::cout << "Percent: " << (i*statesSize+j)/total*100 << std::endl;
    }
  }
  std::cout << "Repopulated the earth. No. Big. Deal." << std::endl;
  //sasListExists_ = true;
  return sasHolder;
}

void sasUtils::setupSAS(mapPairSVS& sasList,std::vector<stateStruct>& states, std::vector< std::vector<double> >& actions,bool overwriteCSV){
  //overwriteCSV_ = true;
  
  std::string fileName = "files/sasSave.txt";
  if (sasUtils::readSASfromCSV(sasList,fileName)){
    std::cout << "Got the saved SAS data! Score!" << std::endl;
    // 1) Create a function that populates the SAS if it isn't
    // 2) make a check to see if the SAS is a match using isMatch
    // 3) if it isn't a match, populate, or if you couldn't read, populate
    // 4) check on what happens if you overwrite a file
    if (sasUtils::isMatch(sasList,states,actions)){
      std::cout << "Don't worry, Son, we have a match." << std::endl;
    }
    else{
      // No match. fix it.
      std::cout << "It didn't match. Why does this always happen to me?" << std::endl;
      sasList.clear(); // Not necessary, but just to be safe
      sasList = populateSAS(states,actions);
      if (overwriteCSV){
	if (sasUtils::writeSAStoCSV(sasList,fileName)){
	  std::cout << "Saved SAS data! Gnarboots!" << std::endl;
	}
	else std::cout << "Failed to save the SAS data. lame." << std::endl;
      }
    }
  }
  else{
    std::cout << "Couldn't get the saved SAS data. Raspberries." << std::endl;
    sasList.clear(); // Not necessary, but just to be safe
    sasList = populateSAS(states,actions);
    if (overwriteCSV){
      if (sasUtils::writeSAStoCSV(sasList,fileName)){
	std::cout << "Saved SAS data! Gnarboots!" << std::endl;
      }
      else std::cout << "Failed to save the SAS data. lame." << std::endl;
    }
  } 
}

////////////////////////////////////////////////////////////////////////////////
//                           End Setup Section                                //
////////////////////////////////////////////////////////////////////////////////

// done x
bool sasUtils::isMatch(mapPairSVS& sasList, std::vector<stateStruct>& states, std::vector< std::vector<double> >& actions){
  // Check if the the new state and action setup is the same as was written
  // in the file to save the next states. This does not find if the next
  // state are the same (for example, if the simulations changed). Doing so
  // would require resimulating all the pairs which would defeat the point.

  // make sure the number of state action pairs is the same as the number of map elements 
  if (states.size()*actions.size() == sasList.size()){
    // check every state action pair to make sure that they exist
    for (size_t i=0;i<actions.size();i++){
      for (size_t j=0;j<states.size();j++){
	if(sasList.count(pairSV (states[j],actions[i]))==0){
	  return false;
	}
      }
    }
    return true;
  }
  else return false;
}

//done x
void sasUtils::insertIntoSAS(mapPairSVS& sasList,stateStruct& prevState,std::vector<double>& action,stateStruct& nextState){
  sasList.insert(pairPairSVS (pairSV (prevState,action),nextState));
}

//done x
stateStruct sasUtils::getFromSAS(mapPairSVS& sasList,stateStruct& prevState, std::vector<double>& action){
  // this is only going to work if the SAS map was populated properly
  // such that you will never ask for a pair that isn't in the map.
  // If that happens, it will throw an error.
  // map::at does not create a new element if one doesn't exist.
  // it will throw an out_of_range exception
  return sasList.at(pairSV (prevState,action));
}

//done x
bool sasUtils::readSASfromCSV(mapPairSVS& sasList,std::string fileName){
  // Clear out the SAS list
  sasList.clear();
  
  std::ifstream data(fileName.c_str());
  // Specific assumption that the file is written with:
  // 1) 7 lines per SAS key-value pair
  // 2) 3 lines for previous state
  // 3) 1 line for action
  // 4) 3 lines for next state
  // 5) A state's lines are model, params, vars
  // 6) Model is a single number
  // 7) Params, vars, and action are an arbitrary length list of value separated by commas
  if (data.is_open()){
    
    std::string line;
    size_t stepCount = 0; // step of process of reading a SAS in.
    stateStruct tmpPrevState;
    std::vector<double> tmpAction;
    stateStruct tmpNextState;
    
    // 7 steps to read in a SAS. 
    // 3 for prevState, 1 for action, 3 for nextState.
    // Add SAS and reset on step 7.
    while(std::getline(data,line)){
      //initialize everyting you need for the loop
      std::stringstream  lineStream(line);
      std::string        cell;
      // previous state
      if (stepCount == 0){
	// add model
	if(std::getline(lineStream,cell)){
	  std::stringstream cellStream(cell); 
	  int result;
	  cellStream >> result;
	  tmpPrevState.model = result;
	}
	stepCount++;
      }
      else if (stepCount == 1){
	// add params
	while(std::getline(lineStream,cell,',')){
	  // You have a cell
	  std::stringstream cellStream(cell); 
	  double result;
	  cellStream >> result;
	  tmpPrevState.params.push_back(result);
	}
	stepCount++;
      }
      else if (stepCount == 2){
	// add vars
	while(std::getline(lineStream,cell,',')){
	  // You have a cell
	  std::stringstream cellStream(cell); 
	  double result;
	  cellStream >> result;
	  tmpPrevState.vars.push_back(result);
	}
	stepCount++;
      }
      // action
      else if (stepCount == 3){
	// add action
	while(std::getline(lineStream,cell,',')){
	  // You have a cell
	  std::stringstream cellStream(cell); 
	  double result;
	  cellStream >> result;
	  tmpAction.push_back(result);
	}
	stepCount++;
      }
      // next state
      else if (stepCount == 4){
	// add model
	if(std::getline(lineStream,cell)){
	  std::stringstream cellStream(cell);
	  int result;
	  cellStream >> result;
	  tmpNextState.model = result;
	}
	stepCount++;
      }
      else if (stepCount == 5){
	// add params
	while(std::getline(lineStream,cell,',')){
	  // You have a cell
	  std::stringstream cellStream(cell); 
	  double result;
	  cellStream >> result;
	  tmpNextState.params.push_back(result);
	}
	stepCount++;
      }
      else if (stepCount == 6){
	// add vars
	while(std::getline(lineStream,cell,',')){
	  // You have a cell
	  std::stringstream cellStream(cell); 
	  double result;
	  cellStream >> result;
	  tmpNextState.vars.push_back(result);
	}
	
	// insert into SAS list
	insertIntoSAS(sasList,tmpPrevState,tmpAction,tmpNextState);
	
	// Reset for the next SAS key-value pair
	stepCount=0;
	tmpPrevState.params.clear(); // clear prev state params vector
	tmpPrevState.vars.clear(); // clear prev state vars vector
	tmpAction.clear(); // clear action vector
	tmpNextState.params.clear(); // clear next state params vector
	tmpNextState.vars.clear(); // clear next state vars vector
      }
    }
    // Make full SAS key-value pairs were created before saying success
    if (stepCount == 0){
      return true;
    }
    else return false;
  }
  else return false;
}

//done x
bool sasUtils::writeSAStoCSV(mapPairSVS& sasList,std::string fileName){
  //try to write to csv
  std::ofstream outFile(fileName.c_str());
  
  if (outFile.is_open()){
    for (mapPairSVS_it mapIt = sasList.begin(); mapIt !=sasList.end(); mapIt++){
      // Pull out the pieces of this key-vaule pair in the map
      stateStruct tmpPrevState = mapIt->first.first;
      std::vector<double> tmpAction = mapIt->first.second;
      stateStruct tmpNextState = mapIt->second;
      
      // Write the pieces of the previous state
      // model
      outFile << tmpPrevState.model << "\n";
      // params
      for (size_t i=0;i<tmpPrevState.params.size();i++){
	outFile << tmpPrevState.params[i] << ",";
      }
      outFile << "\n";
      // vars
      for (size_t i=0;i<tmpPrevState.vars.size();i++){
	outFile << tmpPrevState.vars[i] << ",";
      }
      outFile << "\n";
      // Write the pieces of the action
      for (size_t i=0;i<tmpAction.size();i++){
	outFile << tmpAction[i] << ",";
      }
      outFile << "\n";
      // Write the pieces of the next state
// model
      outFile << tmpNextState.model << "\n";
      // params
      for (size_t i=0;i<tmpNextState.params.size();i++){
	outFile << tmpNextState.params[i] << ",";
      }
      outFile << "\n";
      // vars
      for (size_t i=0;i<tmpNextState.vars.size();i++){
	outFile << tmpNextState.vars[i] << ",";
      }
      outFile << "\n";
    }
    outFile.close();
    return true;
  }
  else{
    std::cout << "Unable to open output SAS file" << std::endl;
    return false;
  }
}
