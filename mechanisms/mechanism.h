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
  // Constructor and Destructor
 Mechanism() : broadphase_(NULL),collisionConfiguration_(NULL),dispatcher_(NULL),solver_(NULL),dynamicsWorld_(NULL),gndCS_(NULL),gndRB_(NULL),rbtCS_(NULL),rbtRB_(NULL) { };
  virtual ~Mechanism(); // calls superclass exit physics specifically


  // Same for all mechanisms
  void updatePose();
  void stepWorld(); 
  void pdController();
  stateStruct simulate(std::vector<double>& action);
  stateStruct initAndSim(stateStruct& startState,std::vector<double>& action);
  std::vector<double> sensToObs(std::vector<double>& obs);
  std::vector<double> rbtToObs(std::vector<double>& rbt);
  void setGoalWithAction(std::vector<double>& action); // not virtual anymore

  btRigidBody* createRigidBody(btCollisionShape* collisionShape,btScalar mass,const btTransform& transform);
  std::vector<double> convCoordsRbtToSim(std::vector<double>& coordsRbt);
  std::vector<double> convCoordsSimToRbt(std::vector<double>& coordsSim);
  btVector3 convStdVectToBtVect3(std::vector<double>& stdVect);
  std::vector<double> convBtVect3ToStdVect(btVector3& stdVect);

  // Redefined in subclasses. Must use superclass version as well.
  virtual void initialize(stateStruct& startState);
  virtual void exitPhysics();

  // Virutal: not defined in the cpp file. defined in subclasses.
  virtual void setStartWithState(stateStruct& startState){}
  // virtual void setGoalWithAction(std::vector<double>& action){} // used to be virtual
  virtual stateStruct returnStateOfWorld(){}
  virtual std::vector<double> stToObs(stateStruct& state){}
  virtual std::vector<double> stToRbt(stateStruct& state){}
  virtual bool isStateValid(stateStruct& state,std::vector< std::vector<double> >& workspace){}
};
  
#endif // MECHANISM_H
