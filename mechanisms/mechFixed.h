#ifndef MECH_FIXED_H
#define MECH_FIXED_H

#include <vector>
#include <btBulletDynamicsCommon.h>

#include "../stateStruct.h"
#include "mechanism.h"

class MechFixed : public Mechanism {

 public:
  // variables

  // world


  // objects


  // constraints


  // controller


  // RB = rigid body
  // BT = body transform
  // CS = collision shape
  // F = force
  // O = origin
  // HC = hinge constraint

  // gnd = ground
  // rbt = robot
  // st = state
  // obs = observation
  // sens = sensor

  // functions
  // Constructor and Destructor
  virtual ~MechFixed();

  // Specific to subclass

  // Redefined in subclasses. Must use superclass version as well.
  virtual void initialize(stateStruct& startState);
  virtual void exitPhysics();

  // Virutal: not defined in the superclass Mechanism.
  virtual void setStartWithState(stateStruct& startState);
  // virtual void setGoalWithAction(std::vector<double>& action); // used to be virtual
  virtual stateStruct returnStateOfWorld();
  virtual std::vector<double> stToObs(stateStruct& state);
  virtual std::vector<double> stToRbt(stateStruct& state);
  virtual bool isStateValid(stateStruct& state,std::vector< std::vector<double> >& workspace);
};
  
#endif // MECH_FIXED_H
