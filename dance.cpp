#include "/usr/local/Aria/include/Aria.h"
#include </usr/local/Aria/include/ariaUtil.h>
#include </usr/local/Aria/include/ArActionGroup.h>
#include </usr/local/Aria/include/ArFunctor.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>



//TODO
/*
 *
 * 1) add files to repository whenever tom adds git to the computer
 * 2) continue looking at the example
 *    http://robots.mobilerobots.com/docs/api/ARIA/2.9.1/docs/actionExample_8cpp-example.html
 * 3) Compile + run?
 *
 *
 */

/*
 * When using the ArAction class direct commands can still be used (for example
ArRobot::setVel()), but if you mix direct motion commands with ArAction objects
you must fix ArRobot’s state by calling ArRobot::clearDirectMotion() before actions will work again
 *
 */
class DanceTime : public ArAction{
public:
	//constructor
	virtual danceTime();
	//Destructor
	~danceTime(void);
	//link to fire method
	ArRangeDevice *mySonar;
	virtual ArActionDesired *fire(ArActionDesired currentDesired);
	ArRobot myRobot = 0;
	virtual void setRobot(ArRobot *robot);
protected:
	ArActionDesired myDesired;
};


/*
  Override ArAction::setRobot() to get the sonar device from the robot, or deactivate this action if it is missing.
  You must also call ArAction::setRobot() to properly store
  the ArRobot pointer in the ArAction base class.
*/
void DanceTime::setRobot(ArRobot *robot)
{
  ArAction::setRobot(robot);
  mySonar = robot->findRangeDevice("sonar");
  if (robot == NULL)
    {
      ArLog::log(ArLog::Terse, "actionExample: ActionGo: Warning: I found no sonar, deactivating.");
      deactivate();
    }
}


ArActionDesired *DanceTime::fire(ArActionDesired currentDesired)
{
	//Reset desired action
	myDesired.reset();
	myRobot.lock();
	ArLog::log(ArLog::Normal, "simpleMotionCommands: Pose=(%.2f,%.2f,%.2f), Trans. Vel=%.2f, Rot. Vel=%.2f, Battery=%.2fV",
	myRobot.getX(), myRobot.getY(), myRobot.getTh(), myRobot.getVel(), myRobot.getRotVel(), myRobot.getBatteryVoltage());
	myRobot.unlock();

	// Sleep for 3 seconds.
	ArLog::log(ArLog::Normal, "simpleMotionCommands: Will start dancing in 3 seconds...");
	ArUtil::sleep(3000);

	// Set forward velocity to 50 mm/s
	ArLog::log(ArLog::Normal, "simpleMotionCommands: Driving forward at 250 mm/s for 5 sec...");
	myRobot.lock();
	myRobot.enableMotors();
	myRobot.setVel(300);
	myRobot.unlock();
	ArUtil::sleep(3000);

	ArLog::log(ArLog::Normal, "simpleMotionCommands: Stopping.");
	myRobot.lock();
	myRobot.stop();
	myRobot.unlock();
	ArUtil::sleep(1000);

	for(int i = 0; i < 10;i ++){
	  ArLog::log(ArLog::Normal, "simpleMotionCommands: Rotating at 10 deg/s for 5 sec...");
	  myRobot.lock();
	  myRobot.setRotVel(50);
	  myRobot.unlock();
	  ArUtil::sleep(1000);

	  ArLog::log(ArLog::Normal, "simpleMotionCommands: Rotating at -10 deg/s for 10 sec...");
	  myRobot.lock();
	  myRobot.setRotVel(-50);
	  myRobot.unlock();
	  ArUtil::sleep(1000);
	}

	ArLog::log(ArLog::Normal, "simpleMotionCommands: Driving forward at 150 mm/s for 5 sec...");
	myRobot.lock();
	myRobot.setRotVel(0);
	myRobot.setVel(300);
	myRobot.unlock();
	ArUtil::sleep(3000);

	ArLog::log(ArLog::Normal, "simpleMotionCommands: Stopping.");
	myRobot.lock();
	myRobot.stop();
	myRobot.unlock();
	ArUtil::sleep(1000);
    // ArRobot contains an exit action for the Escape key. It also
	// stores a pointer to the keyhandler so that other parts of the program can
	// use the same keyhandler.
	// robot.attachKeyHandler(&keyHandler);

	printf("You may press escape to exit\n");
	myRobot.waitForRunExit();
	Aria::exit(0);
	return &myDesired;
}


int main(int argc, char** argv)
{
  Aria::init();
  ArArgumentParser parser(&argc, argv);
  parser.loadDefaultArguments();
  ArRobot pipMan;
  ArSonarDevice sonar;
  // Connect to the robot, get some initial data from it such as type and name,
  // and then load parameter files for this robot.
  ArRobotConnector robotConnector(&parser, &pipMan);
  if(!robotConnector.connectRobot())
  {
    ArLog::log(ArLog::Terse, "actionExample: Could not connect to the robot.");
    if(parser.checkHelpAndWarnUnparsed())
    {
        Aria::logOptions();
        Aria::exit(1);
    }
  }
  if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();
    Aria::exit(1);
  }
  pipMan.runAsync(true); 							//Run in asynchronous mode
  pipMan.lock();									//Lock robot during set up

  pipMan.unlock(); 									//Unlock the robot
  pipMan.addRangeDevice(&sonar);

  ArLog::log(ArLog::Normal, "actionExample: Connected to robot.");
  DanceTime tango;
  pipMan.addRangeDevice(&sonar);
  pipMan.addAction(&tango,42);
  pipMan.comInt(ArCommands::ENABLE, 1); 			//Turn on the motors


  ArActionStallRecover recover;



