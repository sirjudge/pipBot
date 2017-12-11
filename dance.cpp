#include "/usr/local/Aria/include/Aria.h"
#include </usr/local/Aria/include/ariaUtil.h>
#include </usr/local/Aria/include/ArActionGroup.h>
#include </usr/local/Aria/include/ArFunctor.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>
//TODO
/*
 * 1) add files to repository whenever tom adds git to the computer
 * 2) continue looking at the example
 *    http://robots.mobilerobots.com/docs/api/ARIA/2.9.1/docs/actionExample_8cpp-example.html
 * 3) Compile + run?
 *
 *
 * When using the ArAction class direct commands can still be used (for example
ArRobot::setVel()), but if you mix direct motion commands with ArAction objects
you must fix ArRobot’s state by calling ArRobot::clearDirectMotion() before actions will work again
 *
 */

class DanceClass : public ArAction{
public:
	virtual ArActionDesired *fire(ArActionDesired currentDesired);
	virtual void setRobot(ArRobot *robot);
	DanceClass(); //TODO change to dancingwiththestars
	~DanceClass(void);
	ArRobot *robot;
protected:
	ArActionDesired myDesired;
};

void DanceClass::setRobot(ArRobot *robot) {
  ArAction::setRobot(robot);
}
//TODO: This is the error that kills us all :(
//		it's on line
/*Warning: ArRobot cycle took too long because the loop was waiting for lock.
       The cycle took 411 ms, (100 ms normal 250 ms warning)*/

/*some more info with gdb debugging
 *
ArRobot::myPacketReader: Timed out (4) at 247574 (13916 into cycle after sleeping 200)
Losing connection because no odometry received from robot in 243575 milliseconds (greater than the timeout of 8000).
Warning: Task 'Packet Handler' took 13918 ms to run (longer than the 250 warning time)
Warning: ArRobot sync tasks too long at 13918 ms, (100 ms normal 250 ms warning)
157		//myRobot -> unlock();
(gdb)
Warning: ArRobot cycle took too long because the loop was waiting for lock.
	The cycle took 2870 ms, (100 ms normal 250 ms warning)
*/

/*
  Override ArAction::setRobot() to get the sonar device from the robot, or deactivate this action if it is missing.
  You must also call ArAction::setRobot() to properly store
  the ArRobot pointer in the ArAction base class.
*/


DanceClass::DanceClass(): ArAction("tango"){}

DanceClass::~DanceClass(void){}

//TODO: movement commands should be done with myDesired.move
ArActionDesired *DanceClass::fire(ArActionDesired currentDesired)
{
	//Reset desired action
	myDesired.reset();
	myRobot->getX(), myRobot -> getY(), myRobot -> getTh(), myRobot -> getVel(), myRobot -> getRotVel(), myRobot -> getBatteryVoltage();


	// Sleep for 3 seconds.
	ArUtil::sleep(3000);

	// Set forward velocity to 300 mm/s
	//myRobot -> lock();
	myDesired.setVel(300);
	//myRobot -> unlock();
	ArUtil::sleep(3000);
	return &myDesired;
}

int main(int argc, char** argv)
{
  Aria::init();
  ArArgumentParser parser(&argc, argv);
  parser.loadDefaultArguments();
  ArRobot pipMan;

  // Connect to the robot, get some initial data from it such as type and name,
  // and then load parameter files for this robot.
  ArRobotConnector robotConnector(&parser, &pipMan);
  //pipMan.addPacketHandler(new ArGlobalRetFunctor1<bool, ArRobotPacket*>(&handleDebugMessage));
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

  ArLog::log(ArLog::Normal, "actionExample: Connected to robot.");
  ArLog::log(ArLog::Normal, "creating DanceClass tango");
  ArLog::log(ArLog::Normal, "adding tango to pipMan's list");
  DanceClass tango;
  ArLog::log(ArLog::Normal, "tango has been created");
  pipMan.addAction(&tango,42);
  pipMan.comInt(ArCommands::ENABLE, 1); 			//Turn on the motors

  ArActionStallRecover recover;

}
