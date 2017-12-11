#include "/usr/local/Aria/include/Aria.h"
#include </usr/local/Aria/include/ariaUtil.h>
#include </usr/local/Aria/include/ArActionGroup.h>
#include </usr/local/Aria/include/ArFunctor.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <list>

class GoHome: public ArAction{
public:
	virtual ArActionDesired *fire(ArActionDesired currentDesired);
	virtual void setRobot(ArRobot *robot);
	virtual void setNewHome(ArPose pos);
	GoHome(ArRobot *robot);
	~GoHome(void);
protected:
	ArActionDesired myDesired;
	ArRangeDevice *myLaser;
	ArRobot *myRobot;
	ArPose currPos;
	ArPose newHome;
};

//destructor
GoHome::~GoHome(void){}
//constructor
GoHome::GoHome(ArRobot *robot):ArAction("goHome", "goes to original 0,0 position") {
	myRobot = NULL;
	myLaser = NULL;
}

void GoHome::setNewHome(ArPose pos){
	newHome = pos;
	currPos = myRobot -> getPose();
	ArActionGoto gotoPoseAction("goto",ArPose(0.0, 0.0, 0.0),100,400, 150,7);
	//could also maybe just do myRobot -> moveTo(ArPos)
	myRobot-> addAction(&gotoPoseAction,100);
	myRobot -> resetTripOdometer();
}

void GoHome::setRobot(ArRobot *robot) {
  ArAction::setRobot(robot);
  myLaser = myRobot->findRangeDevice("urg2.0_1");
  currPos = myRobot -> getPose();
  //check if list is empty,else grab first item, set to myLaser, pew pew pew
  if (myLaser == 0){
	  ArLog::log(ArLog::Normal,"deactivating cause no laser");
      deactivate();
    }
  ArLog::log(ArLog::Normal,"Message: PipFollow laser check");
}
  
  ArActionDesired *GoHome::fire(ArActionDesired currentDesired) {
	  myDesired.reset();
	  ArActionGoto gotoPoseAction("goto",ArPose(0.0, 0.0, 0.0),100,400, 150,7);
	  ArLog::log(ArLog::Normal,"Going home ...\n");
	  myRobot->addAction(&gotoPoseAction,40);
	  // Return the list of desired actions.
	  return &myDesired;
}


  int main(int argc, char **argv) {
	  Aria::init();
	  ArArgumentParser parser(&argc, argv);
	  parser.loadDefaultArguments();
	  ArRobot pipMan;
	  ArRobotConnector robotConnector(&parser, &pipMan);
	  if(!robotConnector.connectRobot()){
	      ArLog::log(ArLog::Terse, "actionExample: Could not connect to the robot.");
	      if(parser.checkHelpAndWarnUnparsed()){
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

  }
