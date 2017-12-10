#include "Aria.h"
/*
 * Action that drives the robot forward, but stops if obstacles are
 * detected by sonar.
 */
class Dance : public ArAction
{
public:
  // constructor, sets nothing
	Dance();
  // destructor. does not need to do anything
	virtual ~Dance(void) {};
  // called by the action resolver to obtain this action's requested behavior
	virtual ArActionDesired *fire(ArActionDesired currentDesired);
  // store the robot pointer, and it's ArSonarDevice object, or deactivate this action if there is no sonar.
	virtual void setRobot(ArRobot *robot);
protected:
  /* Our current desired action: fire() modifies this object and returns
      to the action resolver a pointer to this object.
      This object is kept as a class member so that it persists after fire()
      returns (otherwise fire() would have to create a new object each invocation,
      but would never be able to delete that object).
  */
  ArActionDesired myDesired;
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


Dance::Dance():ArAction("Go"){}

void Dance::setRobot(ArRobot *robot){
  ArAction::setRobot(robot);
}

ArActionDesired *Dance::fire(ArActionDesired currentDesired){
	//Reset desired action
	myDesired.reset();
	//myRobot -> lock();
	ArLog::log(ArLog::Normal, "simpleMotionCommands: Pose=(%.2f,%.2f,%.2f), Trans. Vel=%.2f, Rot. Vel=%.2f, Battery=%.2fV",
	myRobot -> getX(), myRobot -> getY(), myRobot -> getTh(), myRobot -> getVel(), myRobot -> getRotVel(), myRobot -> getBatteryVoltage());
	//myRobot -> unlock();

	// Sleep for 3 seconds.
	ArLog::log(ArLog::Normal, "simpleMotionCommands: Will start dancing in 3 seconds...");
	ArUtil::sleep(3000);

	// Set forward velocity to 300 mm/s
	ArLog::log(ArLog::Normal, "simpleMotionCommands: Driving forward at 300 mm/s for 5 sec...");
	//myRobot -> lock();
	myRobot -> enableMotors();
	myRobot -> setVel(300);
	//myRobot -> unlock();
	ArUtil::sleep(3000);

	ArLog::log(ArLog::Normal, "simpleMotionCommands: Stopping.");
	//myRobot -> lock();
	myRobot -> stop();
	//myRobot -> unlock();
	ArUtil::sleep(1000);

	for(int i = 0; i < 10;i ++){
	  ArLog::log(ArLog::Normal, "simpleMotionCommands: Rotating at 10 deg/s for 5 sec...");
	  //myRobot -> lock();
	  myRobot -> setRotVel(90);
	  //myRobot -> unlock();
	  ArUtil::sleep(1000);

	  ArLog::log(ArLog::Normal, "simpleMotionCommands: Rotating at -10 deg/s for 10 sec...");
	  //myRobot -> lock();
	  myRobot -> setRotVel(-90);
	  //myRobot -> unlock();
	  ArUtil::sleep(1000);
	}

	for(int j = 0; j < 5; j++){
		ArLog::log(ArLog::Normal, "simpleMotionCommands: Driving forward at 150 mm/s for 5 sec...");
		//myRobot -> lock();
		myRobot -> setRotVel(45);
		myRobot -> setVel(300);
		//myRobot -> unlock();
		ArUtil::sleep(1000);

		ArLog::log(ArLog::Normal, "simpleMotionCommands: Stopping.");
		//myRobot -> lock();
		myRobot -> stop();
		//myRobot -> unlock();
		ArUtil::sleep(1000);

		ArLog::log(ArLog::Normal, "simpleMotionCommands: Driving forward at 150 mm/s for 5 sec...");
		//myRobot -> lock();
		myRobot -> setRotVel(45);
		myRobot -> setVel(300);
		//myRobot -> unlock();
		ArUtil::sleep(1000);

		ArLog::log(ArLog::Normal, "simpleMotionCommands: Stopping.");
		//myRobot -> lock();
		myRobot -> stop();
		////myRobot -> unlock();
		ArUtil::sleep(1000);
		return &myDesired;
	}
}

int main(int argc, char** argv){
  Aria::init();
  ArArgumentParser parser(&argc, argv);
  parser.loadDefaultArguments();
  ArRobot robot;
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
  // Create instances of the actions defined above, plus ArActionStallRecover,
  // a predefined action from Aria.

  ArActionStallRecover recover;
  Dance tango = Dance();
  // Add the range device to the robot. You should add all the range
  // devices and such before you add actions

  // Add our actions in order. The second argument is the priority,
  // with higher priority actions going first, and possibly pre-empting lower
  // priority actions.
  robot.addAction(&recover, 100);

  // Enable the motors, disable amigobot sounds
  robot.enableMotors();
  // Run the robot processing cycle.
  // 'true' means to return if it loses connection,
  // after which we exit the program.
  robot.run(true);

  Aria::exit(0);
}
