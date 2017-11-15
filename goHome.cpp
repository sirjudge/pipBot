#include "/usr/local/Aria/include/Aria.h"
#include </usr/local/Aria/include/ariaUtil.h>
#include </usr/local/Aria/include/ArActionGroup.h>
#include </usr/local/Aria/include/ArFunctor.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <list>
#include "goHome.hh"

class GoHome: public ArAction{
public:
	virtual ArActionDesired *fire(ArActionDesired currentDesired);
	virtual void setRobot(ArRobot *robot);
	GoHome(ArRobot *robot);
	~GoHome(void);
	ArRangeDevice *myLaser;
	ArRobot *myRobot;
protected:
	ArActionDesired myDesired;
};

//destructor
GoHome::~GoHome(void){}
//constructor
GoHome::GoHome(ArRobot *robot):ArAction("goHome","goes to original 0,0 position") {
  myRobot = robot;
}


// This will look the same for any reactive class. Just substitute the
// name of your class for "Drive".

void GoHome::setRobot(ArRobot *robot) {
  myRobot = robot;
  myLaser = myRobot->findRangeDevice("urg2.0_1");                               
  std::list<ArRangeDevice*> *list = myRobot -> getRangeDeviceList();

  //check if list is empty,else grab first item, set to myLaser, pew pew pew

  if (myLaser == 0){
	  ArLog::log(ArLog::Normal,"deactivating cause no laser");
      deactivate();
    }
  ArLog::log(ArLog::Normal,"Message: PipFollow laser check");
}
  
  // This is the key method for your reactive classes, as it defines what
  // action should be taken. Again, add the sensing/action code you need.
  ArActionDesired *GoHome::fire(ArActionDesired currentDesired) {
  myDesired.reset();
  ArActionGoto gotoPoseAction("goto",ArPose(0.0, 0.0, 0.0),100,400, 150,7);

  ArLog::log(ArLog::Normal,"Going home ...\n");
  myRobot->addAction(&gotoPoseAction,40);
// Return the list of desired actions.
  return &myDesired;
}
