#ifndef SAS_UTILS_H
#define SAS_UTILS_H

#include <vector>
#include <string>
#include <map>

#include "stateStruct.h"

namespace sasUtils {

  typedef std::pair<stateStruct,std::vector<double> > pairSV; // pair: state, action
  typedef std::map< pairSV, stateStruct > mapPairSVS; // map: state, action, state
  typedef std::pair< pairSV, stateStruct > pairPairSVS; // fix this
  typedef mapPairSVS::iterator mapPairSVS_it; // iterator for mapPairSVS
  
  mapPairSVS populateSAS(std::vector<stateStruct>& states, std::vector< std::vector<double> >& actions);

  void setupSAS(mapPairSVS& sasList,std::vector<stateStruct>& states, std::vector< std::vector<double> >& actions,bool overwriteCSV,std::string fileName);
		
  bool isMatch(mapPairSVS& sasList, std::vector<stateStruct>& states, std::vector< std::vector<double> >& actions);

  void insertIntoSAS(mapPairSVS& sasList,stateStruct& prevState, std::vector<double>& action, stateStruct& nextState);

  stateStruct getFromSAS(mapPairSVS& sasList,stateStruct& prevState, std::vector<double>& action);

  bool readSASfromCSV(mapPairSVS& sasList,std::string fileName);
  
  bool writeSAStoCSV(mapPairSVS& sasList,std::string fileName);

  std::string doubleToBitString(double input);
  double bitStringToDouble(std::string s);
}


#endif //SAS_UTILS_H
