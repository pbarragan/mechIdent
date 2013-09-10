#ifndef STATE_STRUCT_H
#define STATE_STRUCT_H

#include <vector>

struct stateStruct {
  int model;
  std::vector<double> params;
  std::vector<double> vars;

  bool operator<(const stateStruct& rhs) const {
    if (model == rhs.model){
      if (params==rhs.params){
	return vars<rhs.vars;
      }
      else{
	return params<rhs.params;
      }
    }
    else{
      return model<rhs.model;
    }
  }

};
  
#endif //STATE_STRUCT_H
