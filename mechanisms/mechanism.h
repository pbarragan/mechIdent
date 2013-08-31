#ifndef MECHANISM_H
#define MECHANISM_H

#include <vector>
#include <btBulletDynamicsCommon.h>

#include "../stateStruct.h"

class Mechanism {

 public:
  // variables

  // world
  btBroadphaseInterface* broadphase_;
  btDefaultCollisionConfiguration* collisionConfiguration_;
  btCollisionDispatcher* dispatcher_;
  btConstraintSolver* solver_;
  btDiscreteDynamicsWorld* dynamicsWorld_;

  // objects
  btCollisionShape* gndCS_;
  btRigidBody* gndRB_;
  btCollisionShape* rbtCS_;
  btRigidBody* rbtRB_;
  btTransform rbtBT_;

  // controller
  btVector3 pose_;
  btVector3 vel_;
  btVector3 startPose_;
  btVector3 goalPose_;
  btVector3 pGains_;
  btVector3 dGains_;
  btVector3 rbtF_;

  // RB = rigid body
  // BT = body transform
  // CS = collision shape
  // F = force

  // gnd = ground
  // rbt = robot
  // st = state
  // obs = observation
  // sens = sensor

  // functions
  // Same for all mechanisms
  void updatePose();
  void stepWorld(); 
  void pdController();
  stateStruct simulate(std::vector<double>& action);
  stateStruct initAndSim(stateStruct& startState,std::vector<double>& action);
  virtual ~Mechanism(); // calls superclass exit physics specifically
  std::vector<double> sensToObs(std::vector<double>& obs);
  std::vector<double> rbtToObs(std::vector<double>& rbt);

  btRigidBody* createRigidBody(btCollisionShape* collisionShape,btScalar mass,const btTransform& transform);
  std::vector<double> convCoordsRbtToSim(std::vector<double>& coordsRbt);
  std::vector<double> convCoordsSimToRbt(std::vector<double>& coordsSim);
  btVector3 convStdVectToBtVect3(std::vector<double>& stdVect);
  std::vector<double> convBtVect3ToStdVect(btVector3& stdVect);

  // Redefined in subclasses. Must use superclass version as well.
  virtual void initialize(stateStruct& startState);
  virtual void exitPhysics();

  // Virutal: not defined in the cpp file. defined in subclasses.
  virtual void setStartWithState(stateStruct& startState) = 0;
  virtual void setGoalWithAction(std::vector<double>& action) = 0;
  virtual void returnStateOfWorld() = 0;
  virtual std::vector<double> stToObs(stateStruct& state) = 0;
  virtual std::vector<double> stToRbt(stateStruct& state) = 0;
}
  
#endif // MECHANISM_H
