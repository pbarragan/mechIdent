#ifndef ROBOT_COMM_H
#define ROBOT_COMM_H

#include <vector>
#include <string>

namespace robotComm {

  bool vectorToTxtFile(std::vector<double>& data);

  std::vector<double> txtFileToVector(std::string fileName);
  
  bool sendRequest(std::vector<double> request);
  
  bool getResponse(std::vector<double>& dataVec);

}

#endif //ROBOT_COMM_H
