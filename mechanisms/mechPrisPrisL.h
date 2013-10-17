#ifndef MECH_PRIS_PRIS_L_H
#define MECH_PRIS_PRIS_L_H

#include <vector>
#include <btBulletDynamicsCommon.h>

#include "../stateStruct.h"
#include "mechanism.h"

class MechPrisPrisL : public Mechanism {

 public:
  // variables

  // world


  // initialization parameters in sim
  btVector3 startPoseLink0_;
  btVector3 startPoseLink1_;
  btVector3 startPoseFxd_;

  btVector3 startPoseFxdA_;
  btVector3 startPoseFxdB_;
  btVector3 startPoseFxdC_;

  //    - state parameters
  double x_a2_;
  double y_a2_;
  double theta_a2_;
  double d_L1_;
  double d_L2_;

  //    - geometry variables
  double sizeScale_;
  double link0Len_; // Prismatic link closer to the robot manipulator
  double link1Len_; // Prismatic link closer to the fixed point
  double o1_;
  double o2_;
  double o3_;
  double l1_;
  btQuaternion startQuatLink0_;
  btQuaternion startQuatLink1_;
  btQuaternion startQuatFxdABC_; // same as startQuatLink0_

  // objects
  btCollisionShape* link0CS_;
  btRigidBody* link0RB_;
  btCollisionShape* link1CS_;
  btRigidBody* link1RB_;
  btCollisionShape* fxdCS_;
  btRigidBody* fxdRB_;

  //     - latch rigid bodies
  btCollisionShape* fxdACS_;
  btRigidBody* fxdARB_;
  btCollisionShape* fxdBCS_;
  btRigidBody* fxdBRB_;
  btCollisionShape* fxdCCS_;
  btRigidBody* fxdCRB_;

  // constraints
  btSliderConstraint* fxdLink1SC_; // connected to the fixed sliding point
  btSliderConstraint* link1Link0SC_; // 90 degree sliding constraint
  btHingeConstraint* link0RbtHC_; // the constraint where the robot holds

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
 MechPrisPrisL() : link0CS_(NULL),link0RB_(NULL),link1CS_(NULL),link1RB_(NULL),fxdCS_(NULL),fxdRB_(NULL),fxdACS_(NULL),fxdARB_(NULL),fxdBCS_(NULL),fxdBRB_(NULL),fxdCCS_(NULL),fxdCRB_(NULL),fxdLink1SC_(NULL),link1Link0SC_(NULL),link0RbtHC_(NULL) { };
  virtual ~MechPrisPrisL();

  // Specific to subclass
  // Only exists in latch classes
  bool inContact();

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
  
#endif // MECH_PRIS_PRIS_L_H
