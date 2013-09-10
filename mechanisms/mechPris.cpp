// MechPris sub class

#include "mechPris.h"

#define _USE_MATH_DEFINES
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
//                             Redefined Section                              //
////////////////////////////////////////////////////////////////////////////////

// virtual
void MechPris::initialize(stateStruct& startState){
  // Set the size scale for objects in this world
  float sizeScale = 0.05;
  // Set offsets before running Mechanism::initialize()
  o1_ = 2*sizeScale;
  o2_ = 10;

  // Initialize standard physics for Mechanism
  Mechanism::initialize(startState);

  // Initialize additional physics for MechPris
  // REMEMBER TO SET pose_ to startPose_ and run Mechanism::updatePose()??

  //----------------OBJECT CREATION SECTION-------------------//
  // Use start poses calculated by 
  btVector3 fxdO = startPoseFxd_; // fxd origin
  btVector3 link0O = startPoseLink0_; // link0 origin
  btVector3 rbtO = startPose_; // rbt origin

  //----------make fxd rigid body (the pivot)---------//
  // shape
  const btVector3 fxdBoxHalfExtents( sizeScale, 1.0f, sizeScale );
  fxdCS_ = new btBoxShape(fxdBoxHalfExtents);
  // position
  btTransform fxdBT;
  fxdBT.setIdentity();
  fxdBT.setOrigin(fxdO);
  fxdBT.setRotation(startQuatLink0_); // same angle as link0.
  // mass
  btScalar fxdMass = 0.0f; // fixed
  // create the fxd rigid body
  fxdRB_ = createRigidBody(fxdCS_,fxdMass,fxdBT);
  // add it to the dynamics world
  dynamicsWorld_->addRigidBody(fxdRB_);
  //--------end make fxd rigid body (the sliding box)-------//
  
  //----------make link0 rigid body (the rotating arm)---------//
  // shape
  const btVector3 link0BoxHalfExtents( 1.0f, 0.5f, 0.5f );
  link0CS_ = new btBoxShape(link0BoxHalfExtents);
  // position
  btTransform link0BT;
  link0BT.setIdentity();
  link0BT.setOrigin(link0O);
  link0BT.setRotation(startQuatLink0_);
  // mass
  btScalar link0Mass = 1.0f;
  //create the link0 rigid body
  link0RB_ = createRigidBody(link0CS_,link0Mass,link0BT);
  //add it to the dynamics world
  dynamicsWorld_->addRigidBody(link0RB_);
  //--------end make link0 rigid body (the sliding box)-------//

  //--------make rbt rigid body (the robots manipulator)------//
  // shape
  const btVector3 rbtBoxHalfExtents( sizeScale, 0.5f, sizeScale );
  rbtCS_ = new btBoxShape(rbtBoxHalfExtents);
  // position
  rbtBT_.setIdentity();
  rbtBT_.setOrigin(rbtO);
  // mass
  btScalar rbtMass = 0.1f;
  //create the rbt rigid body
  rbtRB_ = createRigidBody(rbtCS_,rbtMass,rbtBT_);
  //add it to the dynamics world
  dynamicsWorld_->addRigidBody(rbtRB_);
  //------end make rbt rigid body (the robots manipulator)----//

  //--------------END OBJECT CREATION SECTION-----------------//

  //--------------CONSTRAINT CREATION SECTION-----------------//

  // Scope used in this section to avoid renaming local variables

  //--------create hinge constraint between fxd and link0------//
  {
    const btVector3 pivotInA( 0.0f, 0.0f, 0.0f );   
    const btVector3 pivotInB( -r_/2.0f, 0.0f, 0.0f );
    btVector3 axisInA( 0.0f, 1.0f, 0.0f );
    btVector3 axisInB( 0.0f, 1.0f, 0.0f );
    bool useReferenceFrameA = false;
    fxdLink0HC_ = new btHingeConstraint(*fxdRB_,*link0RB_,pivotInA,pivotInB,axisInA,axisInB,useReferenceFrameA);
    // add constraint to the world
    const bool isDisableCollisionsBetweenLinkedBodies = true; //disable collisions
    dynamicsWorld_->addConstraint(fxdLink0HC_,isDisableCollisionsBetweenLinkedBodies);
  }
  //------end create hinge constraint between fxd and link0----//
  
  //--------create hinge constraint between link0 and rbt------//
  {
    const btVector3 pivotInA( r_/2.0f, 0.0f, 0.0f );   
    const btVector3 pivotInB( 0.0f, 0.0f, 0.0f );
    btVector3 axisInA( 0.0f, 1.0f, 0.0f );
    btVector3 axisInB( 0.0f, 1.0f, 0.0f );
    bool useReferenceFrameA = false;
    link0RbtHC_ = new btHingeConstraint(*link0RB_,*rbtRB_,pivotInA,pivotInB,axisInA,axisInB,useReferenceFrameA);
    // add constraint to the world
    const bool isDisableCollisionsBetweenLinkedBodies = true; //disable collisions
    dynamicsWorld_->addConstraint(link0RbtHC_,isDisableCollisionsBetweenLinkedBodies);
  }
  //------end create hinge constraint between link0 and rbt----//

  //--------------END CONSTRAINT CREATION SECTION-----------------//

  //---------INITIALIZE REMAINING PARAMETERS SECTION----------//

  // Disable deactivation for every object
  fxdRB_->forceActivationState(DISABLE_DEACTIVATION);
  link0RB_->forceActivationState(DISABLE_DEACTIVATION);
  rbtRB_->forceActivationState(DISABLE_DEACTIVATION);
  fxdRB_->setSleepingThresholds(0.0,0.0);
  link0RB_->setSleepingThresholds(0.0,0.0);
  rbtRB_->setSleepingThresholds(0.0,0.0);

  // change friction - don't think this is necessary for this mechanism
  fxdRB_->setFriction(0.1);
  rbtRB_->setFriction(0.1);
  link0RB_->setFriction(0.1);

  // ALWAYS call updatePose() before starting to initiliaze pose_ and rbtBT_
  updatePose();

  //-------END INITIALIZE REMAINING PARAMETERS SECTION-------//
}

// virtual
void MechPris::exitPhysics(){
  // cleanup in the reverse order of creation/initialization
  // remove additional things created in this subclass
  delete link0RbtHC_;
  delete fxdLink0HC_;
  delete link0CS_;
  delete fxdCS_;
}

////////////////////////////////////////////////////////////////////////////////
//                           End Redefined Section                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                         Sub (Derived) Class Section                        //
////////////////////////////////////////////////////////////////////////////////

// virtual
MechPris::~MechPris(){
  exitPhysics();
  // This should then call Mechanism::~Mechanism() automatically
}

// Not Defined in super (base) class
// virtual
void MechPris::setStartWithState(stateStruct& startState){
  // State looks like:
  // Model: 3
  // Params: x_axis,y_axis,theta_axis in rbt space
  // Vars: d
  
  // Create an x,y,z vector in rbt for rbt, link0, fxd (object_frame)
  std::vector<double> tempVarsRbt_rbt (3,0.0);
  std::vector<double> tempVarsLink0_rbt (3,0.0);
  std::vector<double> tempVarsFxd_rbt (3,0.0);
  
  double d = startState.vars[0];
  double x_a = startState.params[0];
  double y_a = startState.params[1];
  double theta_a = startState.params[2];
  
  // Calculate
  // Rbt
  tempVarsRbt_rbt[0] = x_a+d*cos(theta_a); // set x_rbt
  tempVarsRbt_rbt[1] = y_a+d*sin(theta_a); // set y_rbt
  tempVarsRbt_rbt[2] = 1.0; // set z_rbt to 1.0

  // Link0
  tempVarsLink0_rbt[0] = x_a+(d-o1_)*cos(theta_a); // set x_link0
  tempVarsLink0_rbt[1] = y_a+(d-o1_)*sin(theta_a); // set y_link0
  tempVarsLink0_rbt[2] = 1.0; // set z_link0 to 1.0

  // Fxd
  tempVarsFxd_rbt[0] = x_a-o2_*cos(theta_a); // set x_fxd
  tempVarsFxd_rbt[1] = y_a-o2_*sin(theta_a); // set y_fxd
  tempVarsFxd_rbt[2] = 1.0; // set z_fxd to 1.0
  
  // Convert to sim and set
  std::vector<double> tempVarsRbt_sim = convCoordsRbtToSim(tempVarsRbt_rbt);
  std::vector<double> tempVarsLink0_sim = convCoordsRbtToSim(tempVarsLink0_rbt);
  std::vector<double> tempVarsFxd_sim = convCoordsRbtToSim(tempVarsFxd_rbt);

  // Set class member variables
  startPose_ = convStdVectToBtVect3(tempVarsRbt_sim);
  startPoseLink0_ = convStdVectToBtVect3(tempVarsLink0_sim);
  startPoseFxd_ = convStdVectToBtVect3(tempVarsFxd_sim);
  x_a_ = x_a;
  y_a_ = y_a;
  theta_a_ = theta_a;
  // theta in rbt. quaternion in sim.
  startQuatLink0_.setValue(0.0f, sin(0.5f*theta_a), 0.0f, cos(0.5f*theta_a));
}

// virtual
void MechPris::setGoalWithAction(std::vector<double>& action){
  // Action looks like:
  // x,y in rbt space

  // Create an x,y,z vector in rbt
  std::vector<double> tempActRbt = action;
  tempActRbt.push_back(1.0); // set z_rbt to 1.0

  // Convert to sim and set
  std::vector<double> tempActSim = convCoordsRbtToSim(tempActRbt);
  goalPose_ = convStdVectToBtVect3(tempActSim);
}

// virtual
stateStruct MechPris::returnStateOfWorld(){
  // State looks like:
  // Model: 2
  // Params: x_pivot,y_pivot in rbt space, r
  // Vars: theta in rbt space

  stateStruct endState;
  endState.model = 2;
  endState.params.push_back(x_p_);
  endState.params.push_back(y_p_);
  endState.params.push_back(r_);
  std::vector<double> tempVarsSim = convBtVect3ToStdVect(pose_);
  std::vector<double> tempVarsRbt = convCoordsSimToRbt(tempVarsSim);
  // only returns betwenn -pi and pi
  endState.vars.push_back(atan2(tempVarsRbt[1]-y_p_,tempVarsRbt[0]-x_p_));
  return endState;
}

// virtual
std::vector<double> MechPris::stToObs(stateStruct& state){
  // State looks like:
  // Model: 2
  // Params: x_pivot,y_pivot in rbt space, r
  // Vars: theta in rbt space  
  double theta = state.vars[0];
  double x_p = state.params[0];
  double y_p = state.params[1];
  double r = state.params[2];
  
  // Observation looks like:
  // x,y in rbt space
  std::vector<double> obs (2,0.0);

  // Calculate
  obs[0] = x_p+r*cos(theta); // set x_obs
  obs[1] = y_p+r*sin(theta); // set y_obs
  return obs;
}

// virtual
std::vector<double> MechPris::stToRbt(stateStruct& state){
  // State looks like:
  // Model: 2
  // Params: x_pivot,y_pivot in rbt space, r
  // Vars: theta in rbt space
  double theta = state.vars[0];
  double x_p = state.params[0];
  double y_p = state.params[1];
  double r = state.params[2];

  // Robot looks like:
  // x,y in rbt space
  std::vector<double> rbt (2,0.0);

  // Calculate
  rbt[0] = x_p+r*cos(theta); // set x_obs
  rbt[1] = y_p+r*sin(theta); // set y_obs
  return rbt;
}

////////////////////////////////////////////////////////////////////////////////
//                       End Sub (Derived) Class Section                      //
////////////////////////////////////////////////////////////////////////////////
