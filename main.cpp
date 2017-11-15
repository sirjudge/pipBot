#include "/usr/local/Aria/include/Aria.h"
#include </usr/local/Aria/include/ArUrg_2_0.h>
#include </usr/local/Aria/include/ariaUtil.h>
#include <iostream>
#include <iomanip>
#include "goHome.hh"
#include </usr/local/Aria/include/ArActionGroup.h>
#include </usr/local/Aria/include/ArFunctor.h>
#include <stdio.h>
#include "follow.hh"

#define MAIN_ROBOT_PORT 8101
#define IS_USING_SIM false

using namespace std;

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

/* ToDo */
// Create new goHome actionGroup
// Save first getPose
// Create ArActionGoto
// Set to Key Press


/* Start main */
int main(int argc, char **argv) {
	ArRobot robot;			// Initialize Robot

	Aria::init();
	ArArgumentParser parser(&argc, argv);
	parser.loadDefaultArguments();

	/* Connect to robot port */
	ArRobotConnector robotConnector(&parser, &robot);
	ArLaserConnector laserConnector(&parser, &robot, &robotConnector);

	/* If connection to robot fails */
	if (!robotConnector.connectRobot()) {
		ArLog::log(ArLog::Terse, "Could not connect to the robot.");
		if (parser.checkHelpAndWarnUnparsed()) {
			// -help not given
			Aria::logOptions();
			Aria::exit(1);
		}
	}
	if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed()) {
		Aria::logOptions();
		Aria::exit(1);
	}

	/* Else add that the robot connected to the log */
	ArLog::log(ArLog::Normal, "Connected to robot.");

	// Create laser pointer (not a pun)
	ArLaser *laser = new ArUrg_2_0(0, "laser 1");

	// turn on motors
	robot.enableMotors();

	// run robot threaded. be sure to use lock when accessing / modifying robot or its properties
	// robot.runAsync(true);

	// create actions
	ArActionLimiterForwards limiter("speed limiter near", 300, 1000, 250);
	ArActionLimiterForwards limiterFar("speed limiter far", 300, 2000, 400);

	if (!laserConnector.connectLasers()) {
		ArLog::log(ArLog::Terse,
				"Could not connect to configured lasers. Exiting.");
		Aria::exit(3);
		return 3;
	}

	robot.lock();
	std::map<int, ArLaser*> *lasers = robot.getLaserMap();
        robot.unlock();

	std::cout << lasers->size() << endl;
	std::map<int, ArLaser*>::iterator it = lasers->begin();
	laser = it->second;

	//laser->asyncConnect();
	//laser->log();

	cout << (laser->isConnected() ? "connected" : "disconnected") << endl;

	printf("Unit Choices: %s\n", laser->getUnitsChoicesString());
	/*
	std::list<std::string> units = laser->getUnitsChoices();
	for (std::list<std::string>::const_iterator iterator = units.begin(), end = units.end(); iterator != end; ++iterator) {
    		std::cout << "Unit Choices: " << *iterator << endl;
	}
	*/

	limiter.activate();
	limiterFar.activate();
	// Save the first postion for goHome


  	ArKeyHandler *keyHandler = Aria::getKeyHandler();
  	if (keyHandler == NULL)
  	{
  	  keyHandler = new ArKeyHandler;
   	  Aria::setKeyHandler(keyHandler);
   	  robot.attachKeyHandler(keyHandler);
 	}

	// Set Follow desired action
	PipFollow follow = PipFollow();
/*
	// lock the robot for adding actions
	robot.lock();
*/
	//PipHome goHome = PipHome();

	ArActionGoto gotoPoseAction("goto",ArPose(0.0, 0.0, 0.0),100,400, 150,7);

	followGroup = new ArActionGroup(&robot);

	// add the action follow with the lowest priority
	//followGroup->addAction(&follow, 10);
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


	std::cout << "Message: Begin robot main loop. W to set to Follow, S to set to Stop." << std::endl;
 	 ArGlobalFunctor followCB(&setFollow);
	 ArGlobalFunctor stopCB(&setStop);
	 ArGlobalFunctor homeCB(&setHome);
 	 keyHandler->addKeyHandler('w', &followCB);
 	 keyHandler->addKeyHandler('W', &followCB);
 	 keyHandler->addKeyHandler('s', &stopCB);
 	 keyHandler->addKeyHandler('S', &stopCB);
 	 keyHandler->addKeyHandler('h', &homeCB);
 	 keyHandler->addKeyHandler('H', &homeCB);
	// start running the robot
	robot.run(true);
	std::cout << "Message: End robot main loop." << std::endl;
	

	ArLog::log(ArLog::Normal, "Ending robot thread...");
	robot.stopRunning();

	// wait for the thread to stop
	robot.waitForRunExit();

	// exit
	ArLog::log(ArLog::Normal, "Exiting.");
	Aria::exit(0);
	return 0;
}





