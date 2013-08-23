//Setup Utilities

#include "stateStruct.h"

////////////////////////////////////////////////////////////////////////////////
//                              Model Section                                 //
////////////////////////////////////////////////////////////////////////////////

//dimRanges (dimension ranges) gives you the min and max in each dimension.
//dimNums gives you the number of discrete points along a dimension.

std::vector<stateStruct> setupModel0(){  
  //Model 0 is the free model
  int modelNum = 0;
  int paramNum = 2; //how many parameters
  int varNum = 2; //how many variables

  //Dimension Ranges for Params
  std::vector< std::vector<double> > dRP (paramNum, std::vector<double> (2,0.0));
  dRP[0][0] = 0.0;
  dRP[0][1] = 1.0;
  dRP[1][0] = 0.0;
  dRP[1][1] = 1.0;
  //Dimension Numbers for Params
  std::vector<int> dNP (paramNum, 0);
  dNP[0] = 2;
  dNP[1] = 2;
  //Dimension Ranges for Vars
  std::vector< std::vector<double> > dRV (2, std::vector<double> (2,0.0));
  dRV[0][0] = -0.1;
  dRV[0][1] = 0.1;
  dRV[1][0] = -0.1;
  dRV[1][1] = 0.1;
  //Dimension Numbers for Vars
  std::vector<int> dNV (paramNum, 0);
  dNV[0] = 3;
  dNV[1] = 3;

  //setup for Model 0
  std::vector< std::vector<double> > paramsList = dimsToList(dRP,dNP);
  std::vector< std::vector<double> > varsList = dimsToList(dRV,dNV);
  return createStateListForModel(modelNum,paramsList,varsList);
}

std::vector<stateStruct> setupModel1(){  
  //Model 1 is the fixed model
  int modelNum = 1;
  int paramNum = 2; //how many parameters
  int varNum = 2; //how many variables

  //Dimension Ranges for Params
  std::vector< std::vector<double> > dRP (paramNum, std::vector<double> (2,0.0));
  dRP[0][0] = 0.0;
  dRP[0][1] = 1.0;
  dRP[1][0] = 0.0;
  dRP[1][1] = 1.0;
  //Dimension Numbers for Params
  std::vector<int> dNP (paramNum, 0);
  dNP[0] = 2;
  dNP[1] = 2;
  //Dimension Ranges for Vars
  std::vector< std::vector<double> > dRV (2, std::vector<double> (2,0.0));
  dRV[0][0] = -0.1;
  dRV[0][1] = 0.1;
  dRV[1][0] = -0.1;
  dRV[1][1] = 0.1;
  //Dimension Numbers for Vars
  std::vector<int> dNV (paramNum, 0);
  dNV[0] = 3;
  dNV[1] = 3;

  //setup for Model 1
  std::vector< std::vector<double> > paramsList = dimsToList(dRP,dNP);
  std::vector< std::vector<double> > varsList = dimsToList(dRV,dNV);
  return createStateListForModel(modelNum,paramsList,varsList);
}

std::vector<stateStruct> setupModel2(){  
  //Model 2 is the revolute model
  int modelNum = 2;
  int paramNum = 2; //how many parameters
  int varNum = 2; //how many variables

  //Dimension Ranges for Params
  std::vector< std::vector<double> > dRP (paramNum, std::vector<double> (2,0.0));
  dRP[0][0] = 0.0;
  dRP[0][1] = 1.0;
  dRP[1][0] = 0.0;
  dRP[1][1] = 1.0;
  //Dimension Numbers for Params
  std::vector<int> dNP (paramNum, 0);
  dNP[0] = 2;
  dNP[1] = 2;
  //Dimension Ranges for Vars
  std::vector< std::vector<double> > dRV (2, std::vector<double> (2,0.0));
  dRV[0][0] = -0.1;
  dRV[0][1] = 0.1;
  dRV[1][0] = -0.1;
  dRV[1][1] = 0.1;
  //Dimension Numbers for Vars
  std::vector<int> dNV (paramNum, 0);
  dNV[0] = 3;
  dNV[1] = 3;

  //setup for Model 2
  std::vector< std::vector<double> > paramsList = dimsToList(dRP,dNP);
  std::vector< std::vector<double> > varsList = dimsToList(dRV,dNV);
  return createStateListForModel(modelNum,paramsList,varsList);
}

std::vector<stateStruct> setupModel3(){  
  //Model 1 is the prismatic model
  int modelNum = 3;
  int paramNum = 2; //how many parameters
  int varNum = 2; //how many variables

  //Dimension Ranges for Params
  std::vector< std::vector<double> > dRP (paramNum, std::vector<double> (2,0.0));
  dRP[0][0] = 0.0;
  dRP[0][1] = 1.0;
  dRP[1][0] = 0.0;
  dRP[1][1] = 1.0;
  //Dimension Numbers for Params
  std::vector<int> dNP (paramNum, 0);
  dNP[0] = 2;
  dNP[1] = 2;
  //Dimension Ranges for Vars
  std::vector< std::vector<double> > dRV (2, std::vector<double> (2,0.0));
  dRV[0][0] = -0.1;
  dRV[0][1] = 0.1;
  dRV[1][0] = -0.1;
  dRV[1][1] = 0.1;
  //Dimension Numbers for Vars
  std::vector<int> dNV (paramNum, 0);
  dNV[0] = 3;
  dNV[1] = 3;

  //setup for Model 3
  std::vector< std::vector<double> > paramsList = dimsToList(dRP,dNP);
  std::vector< std::vector<double> > varsList = dimsToList(dRV,dNV);
  return createStateListForModel(modelNum,paramsList,varsList);
}

std::vector<stateStruct> setupModel4(){  
  //Model 4 is the revolute prismatic (latch) model
  int modelNum = 4;
  int paramNum = 2; //how many parameters
  int varNum = 2; //how many variables

  //Dimension Ranges for Params
  std::vector< std::vector<double> > dRP (paramNum, std::vector<double> (2,0.0));
  dRP[0][0] = 0.0;
  dRP[0][1] = 1.0;
  dRP[1][0] = 0.0;
  dRP[1][1] = 1.0;
  //Dimension Numbers for Params
  std::vector<int> dNP (paramNum, 0);
  dNP[0] = 2;
  dNP[1] = 2;
  //Dimension Ranges for Vars
  std::vector< std::vector<double> > dRV (2, std::vector<double> (2,0.0));
  dRV[0][0] = -0.1;
  dRV[0][1] = 0.1;
  dRV[1][0] = -0.1;
  dRV[1][1] = 0.1;
  //Dimension Numbers for Vars
  std::vector<int> dNV (paramNum, 0);
  dNV[0] = 3;
  dNV[1] = 3;

  //setup for Model 4
  std::vector< std::vector<double> > paramsList = dimsToList(dRP,dNP);
  std::vector< std::vector<double> > varsList = dimsToList(dRV,dNV);
  return createStateListForModel(modelNum,paramsList,varsList);
}

////////////////////////////////////////////////////////////////////////////////
//                             End Model Section                              //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                              Setup Section                                 //
////////////////////////////////////////////////////////////////////////////////

//Create the list of states
void setupUtils::setupStates(std::vector<stateStruct>& stateList){

  //Set up a state list for each model. 
  //Then stick together all of the lists into one master list. 
  
  std::vector<stateStruct> stateList0 = setupModel0;
  std::vector<stateStruct> stateList1 = setupModel1;  
  std::vector<stateStruct> stateList2 = setupModel2;
  std::vector<stateStruct> stateList3 = setupModel3;
  std::vector<stateStruct> stateList4 = setupModel4;

  stateList = stateList0;
  stateList.insert( stateList.end(), stateList1.begin(), stateList1.end());
  stateList.insert( stateList.end(), stateList2.begin(), stateList2.end());
  stateList.insert( stateList.end(), stateList3.begin(), stateList3.end());
  stateList.insert( stateList.end(), stateList4.begin(), stateList4.end());

}

//setup a uniform prior over model and parameter pairs
void setupUtils::setupUniformPrior(std::vector<double>& stateList,std::vector<double>& probList){
  //1. Count up how many times certain instances occur
  std::vector<stateStruct> foundStateTypes; //how many different model-parameter pairs
  std::vector<int> numVarTypesPerStateType; //how many different variable sets per model-parameter pair
  bool addStateType;
  for (size_t i=0; i<stateList.size(); i++){
    addStateType = true;
    for (size_t j=0; j<foundStateTypes.size(); j++){
      if (stateList[i].model == foundStateTypes[j].model && stateList[i].params == foundStateTypes[j].params){
	addStateType = false;
	numVarTypesPerStateType[j]++;
	break;
      }
    }
    if (addStateType){
      foundStateType.push_back(stateList[i]);
      numVarTypesPerStateType.push_back(1);
    }
  }

  //2. Figure out how much probability to assign to each instance
  double probPerStateType = (1.0/foundStateTypes.size());
  std::vector<double> probAmounts; //how much probability to assign per var type per state type. Same order as above.
  for (size_t i=0; i<foundStateTypes.size(); i++){
    probAmounts.push_back(probPerStateType/numVarTypesPerStateType[i]);
  }

  //3. Assing the probability to the right instances
  for (size_t i=0; i<stateList.size(); i++){
    for (size_t j=0; j<foundStateTypes.size(); j++){
      if (stateList[i].model == foundStateTypes[j].model && stateList[i].params == foundStateTypes[j].params){
	probList[i] = probAmounts[j];
      }	      
    }
  }

}

////////////////////////////////////////////////////////////////////////////////
//                             End Setup Section                              //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                               Aux Section                                  //
////////////////////////////////////////////////////////////////////////////////

std::vector<stateStruct> setupUtils::createStateListForModel(int modelNum,std::vector< std::vector<double> > paramsList,std::vector< std::vector<double> > varsList){
  std::vector<stateStruct> stateList;
  for (size_t i=0; i<paramsList.size(); i++){
    stateStruct tempState;
    for (size_t j=0; j<varsList.size(); j++){
      tempState.model=modelNum;
      tempState.params=paramsList[i];
      tempState.vars=varsList[j];
    }
    stateList.push_back(tempState);
  }
  return stateList;
}

//convert from set of dimension ranges and number of points to a list of points
std::vector< std::vector<double> > setupUtils::dimsToList(std::vector< std::vector<double> > dimRanges, std::vector<int> dimNums){
  std::vector< std::vector<double> > valueList = createValueList(dimRanges,dimNums);
  return recurseList(std::vector< std::vector<double> > (), std::vector<double> (), 0, valueList);
}

//create the value list to set up states
std::vector< std::vector<double> > setupUtils::createValueList(std::vector< std::vector<double> > dimRanges, std::vector<int> dimNums){
  //there might be a shorter way 
  std::vector< std::vector<double> > valueList;
  int dims = dimNums.size(); //how many dimension numbers is the number of dimensions
  for (size_t i=0; i<dims; i++) {
    double delta = (dimRanges[i][1]-dimRanges[i][0])/(dimNums[i]-1); //-1 because of the spacing
    std::vector<double> tempVect;
    for (size_t j=0; j<dimNums[i]; j++) {
      tempVect.push_back(dimRanges[i][0]+delta*j);
    }
    valueList.push_back(tempVect);
  }
  return valueList;
}


//the recursive function
std::vector< std::vector<double> > setupUtils::recurseList(std::vector< std::vector<double> > totalList, std::vector<double> oldSeq, int level, std::vector< std::vector<double> > valueList){
  //bottomLevel = valueList.size();
  if (level>valueList.size()-1) {
    totalList.push_back(oldSeq);
  }
  else {
    for (size_t i=0; i<valueList[level].size(); i++) {
      oldSeq.push_back(valueList[level][i]);
      totalList = recurseList(totalList,oldSeq,level+1,valueList);
      oldSeq.pop_back();
    }
  }
  return totalList;
}

////////////////////////////////////////////////////////////////////////////////
//                             End Aux Section                                //
////////////////////////////////////////////////////////////////////////////////
