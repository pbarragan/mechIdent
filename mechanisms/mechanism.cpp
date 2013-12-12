// mechanism base class

#include "../globalVars.h"

#include "mechanism.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
//                             Redefined Section                              //
////////////////////////////////////////////////////////////////////////////////

// virtual 
void Mechanism::initialize(stateStruct& startState){
  // Set necessary parameters from startState before initializing physics 
  setStartWithState(startState); // Virtual function: not defined in this file
  
  // Initialize Physics
  // set up world
  broadphase_ = new btDbvtBroadphase();
  collisionConfiguration_ = new btDefaultCollisionConfiguration();
  dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
  solver_ = new btSequentialImpulseConstraintSolver;
  dynamicsWorld_ = new btDiscreteDynamicsWorld(dispatcher_,broadphase_,solver_,collisionConfiguration_);

  // set gravity. y is apparently up.
  dynamicsWorld_->setGravity(btVector3(0,-10,0));

  //----------------OBJECT CREATION SECTION-------------------//
  //make the ground plane
  gndCS_ = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));
  btTransform gndBT;
  gndBT.setIdentity();
  gndBT.setOrigin(btVector3(0.0f,-50.0f,0.0f));
  btScalar gndMass(0.0);

  //create the ground plane rigid body
  gndRB_ = createRigidBody(gndCS_,gndMass,gndBT);
  //add it to the dynamics world
  dynamicsWorld_->addRigidBody(gndRB_);
  //--------------END OBJECT CREATION SECTION-----------------//

  //---------INITIALIZE REMAINING PARAMETERS SECTION----------//
  //set controller values
  pGains_.setValue(5.0f,50.0f,5.0f);
  dGains_.setValue(2.0f,2.0f,2.0f);

  // change friction
  gndRB_->setFriction(0.1);
  //-------END INITIALIZE REMAINING PARAMETERS SECTION-------//

}

// virtual 
void Mechanism::exitPhysics(){
  // cleanup in the reverse order of creation/initialization
  // remove the rigidbodies from the dynamics world and delete them
  // check if the dynamics world exists
  if (dynamicsWorld_){
    int i;
    for (i=dynamicsWorld_->getNumCollisionObjects()-1; i>=0 ;i--){
      btCollisionObject* obj = dynamicsWorld_->getCollisionObjectArray()[i];
      btRigidBody* body = btRigidBody::upcast(obj);
      if (body && body->getMotionState())
	{
	  delete body->getMotionState();
	}
      dynamicsWorld_->removeCollisionObject( obj );
      delete obj;
    }
  }
  // delete collision shapes
  delete gndCS_;
  delete rbtCS_;
  
  // delete world
  delete dynamicsWorld_;
  delete solver_;
  delete collisionConfiguration_;
  delete dispatcher_;
  delete broadphase_;
}

////////////////////////////////////////////////////////////////////////////////
//                           End Redefined Section                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                         Super (Base) Class Section                         //
////////////////////////////////////////////////////////////////////////////////
void Mechanism::updatePose(){
  rbtRB_->getMotionState()->getWorldTransform(rbtBT_); // update rbt transform
  pose_ = rbtBT_.getOrigin(); // get position
}

void Mechanism::stepWorld(){   
  pdController(); // apply pd control
  dynamicsWorld_->stepSimulation(1/60.f,10); // step simulation
  updatePose(); // update rbt transform and pose_
}

void Mechanism::pdController(){
  vel_ = rbtRB_->getLinearVelocity();  // get velocity
  rbtF_ = pGains_*(goalPose_-pose_)-dGains_*vel_; // calculate force
  rbtRB_->applyCentralForce(rbtF_); // apply force
}

stateStruct Mechanism::simulate(std::vector<double>& action){
  // set the necessary parameters from the startState
  setGoalWithAction(action); // Same for each mechanism: defined in this file
  // run iterations of the simulation to generate a new state
  // make sure dynamic world exists - allows fixed case - kind of a hack
  if (dynamicsWorld_){
    for(size_t i = 0; i<1000;i++){
      stepWorld();
    }
  }
  // return the state of the world after the iterations
  std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
  return returnStateOfWorld(); // Virtual function: not defined in this file
}

stateStruct Mechanism::initAndSim(stateStruct& startState,std::vector<double>& action){
  initialize(startState);
  return simulate(action);
}

// virtual 
Mechanism::~Mechanism(){
  Mechanism::exitPhysics();
}

std::vector<double> Mechanism::sensToObs(std::vector<double>& sens){
  // Sens looks like:
  // x,y in rbt space

  // Observation looks like:
  // x,y in rbt space
  return sens;
}

std::vector<double> Mechanism::rbtToObs(std::vector<double>& rbt){
  // Robot looks like:
  // x,y in rbt space

  // Observation looks like:
  // x,y in rbt space
  return rbt;
}

// used to be virtual
void Mechanism::setGoalWithAction(std::vector<double>& action){
  if (RELATIVE){
    // Action looks like:
    // x,y in rbt space relative to hand

    // Create an x,y,z vector in rbt
    std::vector<double> tempActRbt = action;
    tempActRbt.push_back(0.0); // set z_rbt to 0.0 because it's relative

    // Convert to sim and set

    // check how to add up btvect3
    std::vector<double> tempActSim = convCoordsRbtToSim(tempActRbt);
    //goalPose_ = startPose_; // set the goal pose to the start pose
    goalPose_ = pose_; // set the goal pose to the current pose
    goalPose_ += convStdVectToBtVect3(tempActSim); // add the relative action
    std::cout << "relative" << std::endl;
    std::cout << "x: " << goalPose_.getX() << std::endl;
    std::cout << "y: " << goalPose_.getY() << std::endl;
    std::cout << "z: " << goalPose_.getZ() << std::endl;
  }
  else {
    // Action looks like:
    // x,y in rbt space
    
    // Create an x,y,z vector in rbt
    std::vector<double> tempActRbt = action;
    tempActRbt.push_back(1.0); // set z_rbt to 1.0
    
    // Convert to sim and set
    std::vector<double> tempActSim = convCoordsRbtToSim(tempActRbt);
    goalPose_ = convStdVectToBtVect3(tempActSim);
    std::cout << "absolute" << std::endl;
    std::cout << "x: " << goalPose_.getX() << std::endl;
    std::cout << "y: " << goalPose_.getY() << std::endl;
    std::cout << "z: " << goalPose_.getZ() << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
//                     End Super (Base) Class Section                         //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                               Aux Section                                  //
////////////////////////////////////////////////////////////////////////////////

//Creates a rigid body I think
btRigidBody* Mechanism::createRigidBody(btCollisionShape* collisionShape,btScalar mass,const btTransform& transform){
  // calculate inertia
  btVector3 localInertia( 0.0f, 0.0f, 0.0f );
  collisionShape->calculateLocalInertia( mass, localInertia );    
  // create motion state
  btDefaultMotionState* defaultMotionState = new btDefaultMotionState( transform );
  // create rigid body
  btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(mass,defaultMotionState,collisionShape,localInertia);
  btRigidBody* rigidBody = new btRigidBody(rigidBodyConstructionInfo);      
  return rigidBody;
}

// coordinate conversions - not gonna deal with angles yet
std::vector<double> Mechanism::convCoordsRbtToSim(std::vector<double>& coordsRbt){
  std::vector<double> coordsSim (coordsRbt.size(),0.0);
  coordsSim[0]=coordsRbt[0]; // x_rbt to x_sim
  coordsSim[1]=coordsRbt[2]; // z_rbt to y_sim
  coordsSim[2]=-coordsRbt[1]; // y_rbt to -z_sim  
  return coordsSim;
  /*
  // Save in case
  std::vector<double> coordsSim (coordsRbt.size(),0.0);
  for (size_t i=0;i<coordsRbt.size(),i++){
    if (i==0){
      // x_rbt to x_sim
      coordsSim[0]=coordsRbt[i];
    }
    else if (i==1){
      // y_rbt to -z_sim
      coordsSim[2]=-coordsRbt[i];
    }
    else if (i==2){
      // z_rbt to y_sim
      coordsSim[1]=coordsRbt[i];
    }
  }
  return coordsSim;
  */
}

std::vector<double> Mechanism::convCoordsSimToRbt(std::vector<double>& coordsSim){
  std::vector<double> coordsRbt (coordsSim.size(),0.0);
  coordsRbt[0]=coordsSim[0]; // x_sim to x_rbt
  coordsRbt[1]=-coordsSim[2]; // z_sim to -y_rbt
  coordsRbt[2]=coordsSim[1]; // y_sim to z_rbt
  return coordsRbt;
  /*
  // Save in case
  std::vector<double> coordsRbt (coordsSim.size(),0.0);
  for (size_t i=0;i<coordsSim.size(),i++){
    if (i==0){
      // x_sim to x_rbt
      coordsRbt[0]=coordsSim[i];
    }
    else if (i==1){
      // y_sim to z_rbt
      coordsRbt[2]=coordsSim[i];
    }
    else if (i==2){
      // z_sim to -y_rbt
      coordsRbt[1]=-coordsSim[i];
    }
  }
  return coordsRbt;
  */
}

// std::vector to btVector3 conversions
btVector3 Mechanism::convStdVectToBtVect3(std::vector<double>& stdV){
  btVector3 btV (stdV[0],stdV[1],stdV[2]);
  return btV;
}

std::vector<double> Mechanism::convBtVect3ToStdVect(btVector3& btV){
  std::vector<double> stdV;
  stdV.push_back(btV.getX());
  stdV.push_back(btV.getY());
  stdV.push_back(btV.getZ());
  return stdV;
}

////////////////////////////////////////////////////////////////////////////////
//                             End Aux Section                                //
////////////////////////////////////////////////////////////////////////////////
