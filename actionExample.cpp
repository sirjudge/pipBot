#include "Aria.h"
#include <iostream>

class DanceClass : public ArAction{
public:
	virtual ArActionDesired *fire(ArActionDesired currentDesired);
	virtual void setRobot(ArRobot *robot);
	DanceClass();
	virtual ~DanceClass(void){};
protected:
	ArActionDesired myDesired;
};

void DanceClass::setRobot(ArRobot *robot){
  ArAction::setRobot(robot);
}

DanceClass::DanceClass():ArAction("dancing"){}

ArActionDesired *DanceClass::fire(ArActionDesired currentDesired){
    ArLog::log(ArLog::Normal, "starting DanceClass fire method");

	// reset the actionDesired (must be done), to clear
	// its previous values.
	myDesired.reset();
	// if the sonar is null we can't do anything, so deactivate
	myRobot->getX(), myRobot -> getY(), myRobot -> getTh(), myRobot -> getVel(), myRobot -> getRotVel(), myRobot -> getBatteryVoltage();
	std::cout << "x" << myRobot->getX() << "," << myRobot -> getY();
  	// Set forward velocity to 300 mm/s
    ArLog::log(ArLog::Normal, "sleeping for 2 seconds");
	ArUtil::sleep(2000);
    ArLog::log(ArLog::Normal, "setting velocity and delta heading");

     ArLog::log(ArLog::Normal, "starting while loop,initiaing deep sleep");
     int count = 0;
     //for(int i = 0; i < 10; i ++){
     while(count < 10){
     myRobot->lock();
     myDesired.setVel(100);
     myDesired.setDeltaHeading(180);
     myRobot->unlock();

     myRobot->lock();
     myDesired.setVel(100);
     myDesired.setDeltaHeading(180);
     myRobot->unlock();
     count ++;
     ArLog::log(ArLog::Normal, "finished while loop");
     }
    ArLog::log(ArLog::Normal, "returning myDesired");
    // return a pointer to the actionDesired to the resolver to make our request
    return &myDesired;
}


/* ^^^^^^^^^^^^^^^^^^^^^^^^^^
 * ==========================
 * Above this is dance  stuff
 * ==========================
 * below this is follow stuff
 * ==========================
 * vvvvvvvvvvvvvvvvvvvvvvvvvv
 */



class PipFollow: public ArAction{
  public:
    PipFollow();
    virtual ArActionDesired *fire(ArActionDesired currentDesired);
    virtual ~PipFollow(void){}
    virtual void setRobot(ArRobot *robot);
  protected:
    ArActionDesired myDesired;
    ArRangeDevice *mySonar;
    ArRangeDevice *myLaser;
};
PipFollow::PipFollow():ArAction("follow","follows a thing"){
	mySonar = NULL;
	myLaser = NULL;
}

void PipFollow::setRobot(ArRobot *robot)
{
  ArAction::setRobot(robot);

  mySonar = robot->findRangeDevice("sonar");
  myLaser = robot->findRangeDevice("urg2.0_1");
  if (robot == NULL){
      ArLog::log(ArLog::Terse, "actionExample: pipFollow: setRobot: Warning: I found no robot, deactivating.");
      deactivate();
    }
  if (mySonar == NULL){
        ArLog::log(ArLog::Terse, "actionExample:  pipFollow: setRobot: Warning: I found no sonar, deactivating.");
        deactivate();
      }
  if (myLaser == NULL){
        ArLog::log(ArLog::Terse, "actionExample:  pipFollow: setRobot: Warning: I found no laser, deactivating.");
        deactivate();
      }
}
//TODO: This doesn't work at all please fix
ArActionDesired *PipFollow::fire(ArActionDesired currentDesired) {

  myDesired.reset();
  // just a bunch of print statements and stuff
  double angle = 0;
  double dist = myLaser->currentReadingPolar(-90, 90, &angle);
  double distTraveledMeters = static_cast<int>(myRobot->getOdometerDistance()/1000);
  double distTraveledMM = static_cast<int>(myRobot->getOdometerDistance())% 1000;
  double timeFromStart = myRobot->getOdometerTime();
  double angleFromStart = myRobot->getOdometerDegrees();
  double PoseX=myRobot->getX();
  double PoseY=myRobot->getY();
  bool contFollow = true;
  double range;
  double speed;


  range = mySonar->currentReadingPolar(-70, 70) - myRobot->getRobotRadius();






  //myDesired.setVel(100);
    if(dist > 200){
      myDesired.setDeltaHeading(angle);
      myDesired.setVel(100);
    }
    else{
      myDesired.setVel(0);
      printf("Do you want to continue following? press y to continue and n to go home");
      char temp;
      std::cin >> temp;
      if (temp == 'y'){
        contFollow = true;
      }
      else{
    	  ArActionGoto gotoPoseAction("goto",ArPose(0.0, 0.0, 0.0),100,400, 150,7);
    	  ArLog::log(ArLog::Normal,"Going home ...\n");
    	  myRobot->addAction(&gotoPoseAction,100);
      }
    }
    // Return the list of desired actions.*/
    return &myDesired;
  }

/*
 * =============================================================
 * =============================================================
 * ABOVE THIS IS ALL CODE I WROTE PLEASE DO NOT CHANGE ANYTHING
 * BELOW. IF YOU DO I WILL PERSONALLY SLAP YOU. THANK YOU
 * ==============================================================
 * =============================================================
 */


/* Action that drives the robot forward, but stops if obstacles are
 * detected by sonar.
 */
class ActionGo : public ArAction
{
public:
  // constructor, sets myMaxSpeed and myStopDistance
  ActionGo(double maxSpeed, double stopDistance);
  // destructor. does not need to do anything
  virtual ~ActionGo(void) {};
  // called by the action resolver to obtain this action's requested behavior
  virtual ArActionDesired *fire(ArActionDesired currentDesired);
  // store the robot pointer, and it's ArSonarDevice object, or deactivate this action if there is no sonar.
  virtual void setRobot(ArRobot *robot);
protected:
  // the sonar device object obtained from the robot by setRobot()
  ArRangeDevice *mySonar;
  /* Our current desired action: fire() modifies this object and returns
      to the action resolver a pointer to this object.
      This object is kept as a class member so that it persists after fire()
      returns (otherwise fire() would have to create a new object each invocation,
      but would never be able to delete that object).
  */
  ArActionDesired myDesired;
  double myMaxSpeed;
  double myStopDistance;
};
/* Action that turns the robot away from obstacles detected by the
 * sonar. */
class ActionTurn : public ArAction
{
public:
  // constructor, sets the turnThreshold, and turnAmount
  ActionTurn(double turnThreshold, double turnAmount);
  // destructor, its just empty, we don't need to do anything
  virtual ~ActionTurn(void) {};
  // fire, this is what the resolver calls to figure out what this action wants
  virtual ArActionDesired *fire(ArActionDesired currentDesired);
  // sets the robot pointer, also gets the sonar device, or deactivates this action if there is no sonar.
  virtual void setRobot(ArRobot *robot);
protected:
  // this is to hold the sonar device form the robot
  ArRangeDevice *mySonar;
  // what the action wants to do; used by the action resover after fire()
  ArActionDesired myDesired;
  // distance at which to start turning
  double myTurnThreshold;
  // amount to turn when turning is needed
  double myTurnAmount;
  // remember which turn direction we requested, to help keep turns smooth
  int myTurning; // -1 == left, 1 == right, 0 == none
};
/*
  Note the use of constructor chaining with
  ArAction(actionName). Also note how it uses setNextArgument, which makes it so that
  other parts of the program could find out what parameters this action has, and possibly modify them.
*/
ActionGo::ActionGo(double maxSpeed, double stopDistance) :
  ArAction("Go")
{
  mySonar = NULL;
  myMaxSpeed = maxSpeed;
  myStopDistance = stopDistance;
  setNextArgument(ArArg("maximum speed", &myMaxSpeed, "Maximum speed to go."));
  setNextArgument(ArArg("stop distance", &myStopDistance, "Distance at which to stop."));
}
/*
  Override ArAction::setRobot() to get the sonar device from the robot, or deactivate this action if it is missing.
  You must also call ArAction::setRobot() to properly store
  the ArRobot pointer in the ArAction base class.ArActionDesired *ActionGo::fire(ArActionDesired currentDesired)
{
  double range;
  double speed;
  // reset the actionDesired (must be done), to clear
  // its previous values.
  myDesired.reset();
  // if the sonar is null we can't do anything, so deactivate
  if (mySonar == NULL)
  {
    deactivate();
    return NULL;
  }
  // get the range of the sonar
  range = mySonar->currentReadingPolar(-70, 70) - myRobot->getRobotRadius();
  // if the range is greater than the stop distance, find some speed to go
  if (range > myStopDistance)
  {
    // just an arbitrary speed based on the range
    speed = range * .3;
    // if that speed is greater than our max, cap it
    if (speed > myMaxSpeed)
      speed = myMaxSpeed;
    // now set the velocity
    myDesired.setVel(speed);
  }
  // the range was less than the stop distance, so request stop
  else
  {
    myDesired.setVel(0);
  }
  // return a pointer to the actionDesired to the resolver to make our request
  return &myDesired;
}
*/
void ActionGo::setRobot(ArRobot *robot)
{
  ArAction::setRobot(robot);
  mySonar = robot->findRangeDevice("sonar");
  if (robot == NULL)
    {
      ArLog::log(ArLog::Terse, "actionExample: ActionGo: Warning: I found no sonar, deactivating.");
      deactivate();
    }
}
/*
  This fire is the whole point of the action.
  currentDesired is the combined desired action from other actions
  previously processed by the action resolver.  In this case, we're
  not interested in that, we will set our desired
  forward velocity in the myDesired member, and return it.
  Note that myDesired must be a class member, since this method
  will return a pointer to myDesired to the caller. If we had
  declared the desired action as a local variable in this method,
  the pointer we returned would be invalid after this method
  returned.
*/
//TODO: this is where I get all my inspiration
ArActionDesired *ActionGo::fire(ArActionDesired currentDesired)
{
  double range;
  double speed;
  // reset the actionDesired (must be done), to clear
  // its previous values.
  myDesired.reset();
  // if the sonar is null we can't do anything, so deactivate
  if (mySonar == NULL)
  {
    deactivate();
    return NULL;
  }
  // get the range of the sonar
  range = mySonar->currentReadingPolar(-70, 70) - myRobot->getRobotRadius();
  // if the range is greater than the stop distance, find some speed to go
  if (range > myStopDistance)
  {
    // just an arbitrary speed based on the range
    speed = range * .3;
    // if that speed is greater than our max, cap it
    if (speed > myMaxSpeed)
      speed = myMaxSpeed;
    // now set the velocity
    myDesired.setVel(speed);
  }
  // the range was less than the stop distance, so request stop
  else
  {
    myDesired.setVel(0);
  }
  // return a pointer to the actionDesired to the resolver to make our request
  return &myDesired;
}
/*
  This is the ActionTurn constructor, note the use of constructor chaining
  with the ArAction. also note how it uses setNextArgument, which makes
  it so that other things can see what parameters this action has, and
  set them.  It also initializes the classes variables.
*/
ActionTurn::ActionTurn(double turnThreshold, double turnAmount) :
  ArAction("Turn")
{
  myTurnThreshold = turnThreshold;
  myTurnAmount = turnAmount;
  setNextArgument(ArArg("turn threshold (mm)", &myTurnThreshold, "The number of mm away from obstacle to begin turnning."));
  setNextArgument(ArArg("turn amount (deg)", &myTurnAmount, "The number of degress to turn if turning."));
  myTurning = 0;
}
/*
  Sets the myRobot pointer (all setRobot overloaded functions must do this),
  finds the sonar device from the robot, and if the sonar isn't there,
  then it deactivates itself.
*/
void ActionTurn::setRobot(ArRobot *robot)
{
  ArAction::setRobot(robot);
  mySonar = robot->findRangeDevice("sonar");
  if (mySonar == NULL)
  {
    ArLog::log(ArLog::Terse, "actionExample: ActionTurn: Warning: I found no sonar, deactivating.");
    deactivate();
  }
}
/*
  This is the guts of the Turn action.
*/
ArActionDesired *ActionTurn::fire(ArActionDesired currentDesired)
{
  double leftRange, rightRange;
  // reset the actionDesired (must be done)
  myDesired.reset();
  // if the sonar is null we can't do anything, so deactivate
  if (mySonar == NULL)
  {
    deactivate();
    return NULL;
  }
  // Get the left readings and right readings off of the sonar
  leftRange = (mySonar->currentReadingPolar(0, 100) -
        myRobot->getRobotRadius());
  rightRange = (mySonar->currentReadingPolar(-100, 0) -
        myRobot->getRobotRadius());
  // if neither left nor right range is within the turn threshold,
  // reset the turning variable and don't turn
  if (leftRange > myTurnThreshold && rightRange > myTurnThreshold)
  {
    myTurning = 0;
    myDesired.setDeltaHeading(0);
  }
  // if we're already turning some direction, keep turning that direction
  else if (myTurning)
  {
    myDesired.setDeltaHeading(myTurnAmount * myTurning);
  }
  // if we're not turning already, but need to, and left is closer, turn right
  // and set the turning variable so we turn the same direction for as long as
  // we need to
  else if (leftRange < rightRange)
  {
    myTurning = -1;
    myDesired.setDeltaHeading(myTurnAmount * myTurning);
  }
  // if we're not turning already, but need to, and right is closer, turn left
  // and set the turning variable so we turn the same direction for as long as
  // we need to
  else
  {
    myTurning = 1;
    myDesired.setDeltaHeading(myTurnAmount * myTurning);
  }
  // return a pointer to the actionDesired, so resolver knows what to do
  return &myDesired;
}

int main(int argc, char** argv)
{
  Aria::init();
  ArArgumentParser parser(&argc, argv);
  parser.loadDefaultArguments();
  ArRobot robot;
  ArSonarDevice sonar;
  // Connect to the robot, get some initial data from it such as type and name,
  // and then load parameter files for this robot.
  ArRobotConnector robotConnector(&parser, &robot);
  if(!robotConnector.connectRobot())
  {
    ArLog::log(ArLog::Terse, "actionExample: Could not connect to the robot.");
    if(parser.checkHelpAndWarnUnparsed())
    {
        // -help not given
        Aria::logOptions();
        Aria::exit(1);
    }
  }
  if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();
    Aria::exit(1);
  }
  ArLog::log(ArLog::Normal, "actionExample: Connected to robot.");


  //connect to the lasers
  ArLaserConnector laserConnector(&parser, &robot, &robotConnector);
  if(!laserConnector.connectLasers())
  {
    ArLog::log(ArLog::Terse, "Could not connect to configured lasers. Exiting.");
    Aria::exit(3);
    return 3;
  }


  // Create instances of the actions defined above, plus ArActionStallRecover,
  // a predefined action from Aria.
  ActionGo go(500, 350);
  ActionTurn turn(400, 10);
  ArActionStallRecover recover;
  PipFollow follow;
  DanceClass tango;
  // Add the range device to the robot. You should add all the range
  // devices and such before you add actions
  robot.addRangeDevice(&sonar);
  // Add our actions in order. The second argument is the priority,
  // with higher priority actions going first, and possibly pre-empting lower
  // priority actions.
  //robot.addAction(&recover, 101);
  //robot.addAction(&tango, 90);
  robot.addAction(&follow,90);
  //robot.addAction(&go, 50);
  //robot.addAction(&turn, 49);
  // Enable the motors, disable amigobot sounds
  robot.enableMotors();
  // Run the robot processing cycle.
  // 'true' means to return if it loses connection,
  // after which we exit the program.
  robot.run(true);

  Aria::exit(0);
}
