#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>

#include "robotComm.h"

bool robotComm::vectorToTxtFile(std::vector<double>& data){
  //this needs to get changed if you are using a different basicBayes package
  std::string fileName = "./files/robotCommTo.txt"; // going to the robot
  std::ofstream robotCommTo;
  robotCommTo.open(fileName.c_str());
  if (robotCommTo.is_open()){
    for(size_t i=0;i<data.size();i++){
      robotCommTo << data[i] << "\n";
    }
    robotCommTo.close();
    std::cout << "vectorToTxtFile: Wrote output file" << std::endl;
    return true;
  }
  else {
    std::cout << "vectorToTxtFile: Unable to write output file" << std::endl;
    return false;
  }
}

std::vector<double> robotComm::txtFileToVector(std::string fileName) {
	//This function reads in a .txt file of plain data
	//with 1 value on each line and converts that value
	//and places it in a vector.
	
	//This is the setup to read the file containing the data
  std::string line;
  std::ifstream myfile(fileName.c_str());
  double x;
  std::vector<double> data;
  
  //This loop will read the file line by line, convert each number, and build a vector
  if (myfile.is_open())
    {
      while(myfile.good())
	{
	  //if( myfile.eof() ) break;
	  //std::cout << myfile.eof() << std::endl;
	  //std::string line;
	  //double x;
	  getline(myfile,line);
	  if (line != ""){
	    std::istringstream i(line);
	    i >> x;
	    data.push_back(x);
	    //std::cout << line << std::endl;
	    //std::cout << x << std::endl;
	  }
	}
      myfile.close();
      std::cout << "txtFileToVector: Input file opened successfully" << std::endl;
    }
  
  else{
    std::cout << "txtFileToVector: Unable to open input file" << std::endl;
  }
  return data;
}

bool robotComm::sendRequest(std::vector<double> request){
  return vectorToTxtFile(request);
}

bool robotComm::getResponse(std::vector<double>& dataVec){
  // HACK - modify the reponse file which has 3 numbers to only have 2
  dataVec.clear();

  //this needs to get changed if you are using a different basicBayes package
  std::string receivedFile = "./files/robotCommFrom.txt"; // Coming from the robot
  std::vector<double> data = txtFileToVector(receivedFile);
  while (data.size()==0){
    usleep(1000000);
    data = txtFileToVector(receivedFile);
  }
  if(data.size()!=0){
    for(size_t i = 0; i<data.size();i++){
      //std::cout << "what: " << data[i] << std::endl;
      //std::cout << data.size() << std::endl;
    }
    dataVec.push_back(data[0]); // 2D - HACK
    dataVec.push_back(data[1]); // 2D - HACK

    remove(receivedFile.c_str());
    return true;
  }
  std::cout << "robotComm::getReponse: something weird happened" << std::endl;
  return false;
}
