//#include "follow.hh"
#include "/usr/local/Aria/include/Aria.h"
#include </usr/local/Aria/include/ariaUtil.h>
#include </usr/local/Aria/include/ArActionGroup.h>
#include </usr/local/Aria/include/ArFunctor.h>
#include <list>

class PipFollow: public ArAction{
public:
	ArRangeDevice *sonar;
	ArRangeDevice *laser;
	ArRobot *myRobot;
	virtual ArActionDesired *fire(ArActionDesired currentDesired);
	PipFollow();
	~PipFollow(void);
protected:
	ArActionDesired myDesired;
};


PipFollow::PipFollow():ArAction("follow","follows a thing"){
  laser = myRobot->findRangeDevice("urg2.0_1");
  sonar = myRobot->findRangeDevice("sonar");
  //std::list<ArRangeDevice> *list = myRobot->getRangeDeviceList();

// check if list is empty,else grab first item, set to myLaser, pew pew pew
  //double check to see if this is how you check if something is null
  if (laser == 0){
	ArLog::log(ArLog::Normal, "deactivating because no laser");
    deactivate();
  }
  ArLog::log(ArLog::Normal, "Laser connected, preparing to fire!");
}

//destructor
PipFollow::~PipFollow(void){}



// This will look the same for any reactive class. Just substitute the
// name of your class for "Drive".
  
// This is the key method for your reactive classes, as it defines what
// action should be taken. Again, add the sensing/action code you need.

ArActionDesired *PipFollow::fire(ArActionDesired currentDesired) {
// The "myDesired" variable is a place to store the list of actions 
// that you want the robot to take if this rule fires. Initially, we
// must clear any previous actions.
  myDesired.reset();

// Add your code to access the sonar sensors, decide whether the reaction
// fires, and add commands to myDesired if it does.
  double angle = 0;
  double dist = laser->currentReadingPolar(-90, 90, &angle);
  double distTraveledMeters = static_cast<int>(myRobot->getOdometerDistance()/1000);
  double distTraveledMM = static_cast<int>(myRobot->getOdometerDistance())% 1000;
  double timeFromStart = myRobot->getOdometerTime();
  double angleFromStart = myRobot->getOdometerDegrees();
  double PoseX=myRobot->getX();
  double PoseY=myRobot->getY();

  //TODO change this because the lil << doesn't work
  distLog = "Message: Distance to closes object: " << dist << "\n"
  ArLog::log(ArLog::Normal,distLog);

  ArLog::log(ArLog::Normal,"\nDistance traveled: "<< distTraveledMeters <<" meters "  << distTraveledMM <<" mm in "<< timeFromStart <<" secs ");
  ArLog::log(ArLog::Normal,"\nRotated " << angleFromStart<< " degrees.");
  ArLog::log(ArLog::Normal, "\nAt " << PoseX<< "," << PoseY);
  ArLog::log(ArLog::Normal,"\nPress S to Stop");
  ArLog::log(ArLog::Normal,"\nPress H to go home");

  if(dist > 100) {
    myDesired.setDeltaHeading(angle);
    myDesired.setVel(600);
  }
  else {
    myDesired.setVel(0);
  }

// Return the list of desired actions.
  return &myDesired;
}


int main(int argc, char** argv){
	Aria::init();
	ArArgumentParser parser(&argc, argv);
	parser.loadDefaultArguments();
	ArRobot pipMan;
	ArSonarDevice sonar;
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

	  ArLog::log(ArLog::Normal, "Connected and ready to go");
	  PipFollow follow;
	  pipMan.addRangeDevice(&sonar);
	  pipMan.addAction(&follow,42);
	  pipMan.comInt(ArCommands::ENABLE,1);
}
