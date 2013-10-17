#ifndef MECH_PRIS_H
#define MECH_PRIS_H

#include <vector>
#include <btBulletDynamicsCommon.h>

#include "../stateStruct.h"
#include "mechanism.h"

class MechPris : public Mechanism {

 public:
  // variables

  // world


  // initialization parameters in sim
  btVector3 startPoseLink0_;
  btVector3 startPoseFxd_;
  double x_a_;
  double y_a_;
  double theta_a_;
  double o1_;
  double o2_;
  btQuaternion startQuatLink0_;


  // objects
  btCollisionShape* link0CS_;
  btRigidBody* link0RB_;
  btCollisionShape* fxdCS_;
  btRigidBody* fxdRB_;

  // constraints
  btSliderConstraint* fxdLink0SC_; //this is the pivot constraint
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
  // Constructor and Destructor
 MechPris() : link0CS_(NULL),link0RB_(NULL),link0RbtHC_(NULL),fxdCS_(NULL),fxdRB_(NULL),fxdLink0SC_(NULL) { };
  virtual ~MechPris();

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
  
#endif // MECH_PRIS_H
