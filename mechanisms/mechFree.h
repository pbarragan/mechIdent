#ifndef MECHANISM_H
#define MECHANISM_H

#include <vector>
#include <btBulletDynamicsCommon.h>

#include "../stateStruct.h"
#include "mechanism.h"

class MechFree : public Mechanism {

 public:
  // variables

  // world


  // objects
  btCollisionShape* link0CS_;
  btRigidBody* link0RB_;

  // constraints
  btHingeConstraint* link0RbtHC_; //this is the constraint where the robot holds

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
  // Specific to subclass
  virtual ~MechFree();

  // Redefined in subclasses. Must use superclass version as well.
  virtual void initialize(stateStruct& startState);
  virtual void exitPhysics();

  // Virutal: not defined in the superclass Mechanism.
  virtual void setStartWithState(stateStruct& startState) = 0;
  virtual void setGoalWithAction(std::vector<double>& action) = 0;
  virtual void returnStateOfWorld() = 0;
  virtual std::vector<double> stToObs(stateStruct& state);
  virtual std::vector<double> stToRbt(stateStruct& state);
}
  
#endif // MECHANISM_H
