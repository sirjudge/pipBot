#include "Aria.h"
#include <iostream>


class GoHome: public ArAction{
public:
	virtual ArActionDesired *fire(ArActionDesired currentDesired);
	virtual void setRobot(ArRobot *robot);
	virtual void setNewHome(ArPose pos);
	GoHome();
	virtual ~GoHome(void){};
protected:
	ArActionDesired myDesired;
	ArRangeDevice *myLaser;
	ArRangeDevice *mySonar;
	ArPose currPos;
	ArPose newHome;
};
void GoHome::setNewHome(ArPose pos){
	newHome = pos;
	currPos = myRobot -> getPose();
	ArActionGoto gotoPoseAction("goto",ArPose(0.0, 0.0, 0.0),100,400, 150,7);
	//could also maybe just do myRobot -> moveTo(ArPos)
	myRobot-> addAction(&gotoPoseAction,100);
	myRobot -> resetTripOdometer();
}
GoHome::GoHome():ArAction("goHome", "goes to original 0,0 position") {
	mySonar = NULL;
	myLaser = NULL;
}
void GoHome::setRobot(ArRobot *robot) {
  ArAction::setRobot(robot);
  currPos = myRobot -> getPose();
}

ArActionDesired *GoHome::fire(ArActionDesired currentDesired) {
	  myDesired.reset();
	  ArActionGoto gotoPoseAction("goto",ArPose(0.0, 0.0, 0.0),100,400, 150,7);
	  ArLog::log(ArLog::Normal,"Going home ...\n");
	  if (myRobot == NULL){
		  ArLog::log(ArLog::Terse, "myRobot variable in goHome fire is NULL");
	  }
	  else{
		  myRobot -> addAction(&gotoPoseAction,40);
	  }
	  // Return the list of desired actions.
	  return &myDesired;
}


/* ^^^^^^^^^^^^^^^^^^^^^^^^^^
 * ==========================
 * Above this is goHome  stuff
 * ==========================
 * below this is dance stuff
 * ==========================
 * vvvvvvvvvvvvvvvvvvvvvvvvvv
 */

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
	//myRobot->getX(), myRobot -> getY(), myRobot -> getTh(), myRobot -> getVel(), myRobot -> getRotVel(), myRobot -> getBatteryVoltage();
	//std::cout << "x" << myRobot->getX() << "," << myRobot -> getY();
  	// Set forward velocity to 300 mm/s
    ArLog::log(ArLog::Normal, "sleeping for 2 seconds");
	ArUtil::sleep(1000);
    ArLog::log(ArLog::Normal, "setting velocity and delta heading");

     ArLog::log(ArLog::Normal, "starting swtich statement");

 	 myRobot->lock();
     int randNum = rand()%(3 + 1) + 0;
     switch(randNum){
     case 0:
         ArLog::log(ArLog::Normal, "starting case 0");
    	myDesired.setVel(0);
    	myDesired.setDeltaHeading(180);
    	break;
     case 1:
         ArLog::log(ArLog::Normal, "starting case 1");
    	 myDesired.setVel(0);
    	 myDesired.setDeltaHeading(-180);
    	 break;
     case 2:
         ArLog::log(ArLog::Normal, "starting case 2");
    	 myDesired.setVel(50);
    	 myDesired.setDeltaHeading(60);
    	 break;
     case 3:
         ArLog::log(ArLog::Normal, "starting case 3");
    	 myDesired.setVel(-50);
    	 myDesired.setDeltaHeading(-60);
    	 break;
     }
     myRobot->unlock();
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
    PipFollow(double dist);
    virtual ArActionDesired *fire(ArActionDesired currentDesired);
    virtual ~PipFollow(void){}
    virtual void setRobot(ArRobot *robot);
  protected:
    ArActionDesired myDesired;
    ArRangeDevice *mySonar;
    ArRangeDevice *myLaser;
    double stopDistance = 100;
};
PipFollow::PipFollow(double dist):ArAction("follow","follows a thing"){
	mySonar = NULL;
	myLaser = NULL;
	stopDistance = dist;
}

void PipFollow::setRobot(ArRobot *robot){
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

ArActionDesired *PipFollow::fire(ArActionDesired currentDesired) {
	  myDesired.reset();
	  double angle = 0;
	  double distTraveledMeters = static_cast<int>(myRobot->getOdometerDistance()/1000);
	  double distTraveledMM = static_cast<int>(myRobot->getOdometerDistance())% 1000;
	  double timeFromStart = myRobot->getOdometerTime();
	  double angleFromStart = myRobot->getOdometerDegrees();
	  double PoseX=myRobot->getPose().getX();
	  double PoseY=myRobot->getPose().getY();
      // The current readings are a set of obstacle readings (with X,Y positions as well as other attributes) that are the most recent set from teh laser.
      std::list<ArPoseWithTime*> *currentReadings = myLaser->getCurrentBuffer(); // see ArRangeDevice interface doc
      // The raw readings are just range or other data supplied by the sensor. It may also include some device-specific extra values associated with each reading as well. (e.g. Reflectance for LMS200)
      const std::list<ArSensorReading*> *rawReadings = myLaser->getRawReadings();

      myLaser -> lockDevice();
	  double dist = myLaser -> currentReadingPolar(-90, 90, &angle);
      ArLog::log(ArLog::Normal, "lasersExample: Laser #%d  %s.\n\tHave %d 'current' readings.\n\tHave %d 'raw' readings.\n\tClosest reading is at %3.0f degrees and is %2.4f meters away.",
        myLaser->getName(),
        currentReadings->size(),
        rawReadings->size(),
        angle, dist/1000.0);

      // There is a utility to find the closest reading wthin a range of degrees around the laser, here we use this laser's full field of view (start to end)
      // If there are no valid closest readings within the given range, dist will be greater than laser->getMaxRange().
      myLaser->unlockDevice();

	  if(dist < 100) {
		ArLog::log(ArLog::Normal, "if statement");
	    myDesired.setDeltaHeading(angle);
	    myDesired.setVel(300);
	  }
	  else {
		ArLog::log(ArLog::Normal, "else statement");
	    myDesired.setVel(100);
	  }
    return &myDesired;
  }


/* ^^^^^^^^^^^^^^^^^^^^^^^^^^
 * ==========================
 * Above this is goHome  stuff
 * ==========================
 * below this is Main
 * ==========================
 * vvvvvvvvvvvvvvvvvvvvvvvvvv
 */

void stopMovement(ArRobot *robot);
void move(int millimeters, ArRobot *robot);
void turn(int degrees, ArRobot *robot);
ArActionGroup *followGroup;
ArActionGroup *stopGroup;
ArActionGroup *homeGroup;
ArPose homePose;

// Activate the teleop action group. activateExlcusive() causes
 // all other active action groups to be deactivated.
 void setFollow(void)
 {
   followGroup->activateExclusive();
   printf("\n== Follow Mode ==\n");
   printf("  Press escape to exit. S to stop.\n");
 }

 void setStop(void)
 {
   stopGroup->activateExclusive();
   printf("\n== Stop Mode ==\n");
   printf("  Press escape to exit. W to go to follow. H to go home.\n");
 }

void setHome(void)
 {
   homeGroup->activateExclusive();
   printf("\n== Going Home ==\n");
   printf(" Press escape to exit.\n");
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
  if(!robotConnector.connectRobot()){
    ArLog::log(ArLog::Terse, "actionExample: Could not connect to the robot.");
    if(parser.checkHelpAndWarnUnparsed())
    {
        // -help not given
        Aria::logOptions();
        Aria::exit(1);
    }
  }
  if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed()){
    Aria::logOptions();
    Aria::exit(1);
  }
  ArLog::log(ArLog::Normal, "actionExample: Connected to robot.");
  //because it's an external device we can't do the same thing as the sonar device
  //connect to the lasers
  ArLaserConnector laserConnector(&parser, &robot, &robotConnector);
  if(!laserConnector.connectLasers()){
    ArLog::log(ArLog::Terse, "Could not connect to configured lasers. Exiting.");
    Aria::exit(3);
    return 3;
  }

  /* EXPERIMENTAL CODE */
  /* vvvvvvvvvvvvvvvvv */
  // create limiters (make sure speed doesn't get too high)
  	ArActionLimiterForwards limiter("speed limiter near", 300, 1000, 250);
  	ArActionLimiterForwards limiterFar("speed limiter far", 300, 2000, 400);
  	//activate both limiters
  	limiter.activate();
	limiterFar.activate();

	ArKeyHandler *keyHandler = Aria::getKeyHandler();
  	if (keyHandler == NULL){
  	  //bool ArKeyHandler::addKeyHandler( int 	keyToHandle, ArFunctor * 	functor)
      //ArKeyHandler::addKeyHandler('f',follow)
	  keyHandler = new ArKeyHandler;
   	  Aria::setKeyHandler(keyHandler);
   	  robot.attachKeyHandler(keyHandler);
 	}
  	std::cout << "Message: Begin robot main loop. W to set to Follow, S to set to Stop." << std::endl;
  	ArGlobalFunctor followCB(&setFollow);
  	ArGlobalFunctor stopCB(&setStop);
  	ArGlobalFunctor homeCB(&setHome);
  	keyHandler->addKeyHandler('g', &followCB);
  	keyHandler->addKeyHandler('g', &followCB);
  	keyHandler->addKeyHandler('s', &stopCB);
  	keyHandler->addKeyHandler('S', &stopCB);
  	keyHandler->addKeyHandler('h', &homeCB);
  	keyHandler->addKeyHandler('H', &homeCB);
  	// start running the robot
  	robot.run(true);
  	std::cout << "Message: End robot main loop." << std::endl;


  	ArLog::log(ArLog::Normal, "Ending robot thread...");
  	robot.stopRunning();

  	ArActionGoto gotoPoseAction("goto",ArPose(0.0, 0.0, 0.0),100,400, 150,7);

  	followGroup = new ArActionGroup(&robot);
  	// add the action follow with the lowest priority
  	// add the action limiter with the largest priority
  	followGroup->addAction(&limiter, 95);
  	// add the action limiterfar with the second largest priority
  	followGroup->addAction(&limiterFar, 90);
  	// unlock the robot after adding the actions

  	stopGroup = new ArActionGroup(&robot);
  	stopGroup->addAction(new ArActionConstantVelocity("Constant Velocity", 0), 96);

  	homeGroup = new ArActionGroup(&robot);
  	homeGroup->addAction(&gotoPoseAction, 90);
  	// add the action limiter with the largest priority
  	homeGroup->addAction(&limiter, 95);
  	// add the action limiterfar with the second largest priority
  	homeGroup->addAction(&limiterFar, 90);
  	// unlock the robot after adding the actions
  /* ^^^^^^^^^^^^^^^^^^^^^ */
  /* END EXPERIMENTAL CODE */


  // Create instances of the actions defined above, plus ArActionStallRecover,
  // a predefined action from Aria.
  ArActionStallRecover recover;
  PipFollow follow(200);
  DanceClass tango;
  GoHome home;
  // Add the range device to the robot. You should add all the range devices and such before you add actions
  robot.addRangeDevice(&sonar);

  // Add our actions in order. The second argument is the priority,
  // with higher priority actions going first
  robot.addAction(&recover, 101);
  robot.addAction(&tango, 90);
  //robot.addAction(&home,80);
  //robot.addAction(&follow,90);

  ArLog::log(ArLog::Normal, "starting sleep");
  ArUtil::sleep(2000);
  ArLog::log(ArLog::Normal, "done sleeping");

  // Enable the motors, disable amigobot sounds
  robot.enableMotors();
  // Run the robot processing cycle.
  // 'true' means to return if it loses connection,
  // after which we exit the program.
  robot.run(true);
  robot.waitForRunExit();
  Aria::exit(0);
}
