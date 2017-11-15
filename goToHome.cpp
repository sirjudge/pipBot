#include "goHome.hh"

// Constructor for the reactive class. Add any  you need (which
// will probably be stored in a state variable).

PipGoHome::PipGoHome() : ArAction("goHome") {
  myRobot = NULL;
// Store any parameters in state variables
}

// This will look the same for any reactive class. Just substitute the
// name of your class for "Drive".

void PipFollow::setRobot(ArRobot *robot) {
  myRobot = robot;
  myLaser = myRobot->findRangeDevice("urg2.0_1");                               
                               

  std::list< ArRangeDevice * > *list = myRobot->getRangeDeviceList();

// check if list is empty,else grab first item, set to myLaser, pew pew pew

  if (myLaser == NULL){
    std::cout << "deactivating cause no laser" << std::endl;
    deactivate();
  }
  std::cout << "Message: PipFollow laser check: " << myLaser->getName() << std::endl;
}
  
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
  double dist = myLaser->currentReadingPolar(-90, 90, &angle);
  double distTraveledMeters = static_cast<int>(myRobot->getOdometerDistance()/1000);
  double distTraveledMM = static_cast<int>(myRobot->getOdometerDistance())% 1000;
  double timeFromStart = myRobot->getOdometerTime();

  std::cout << "Message: Distance to closes object: " << dist << "\n" << std::endl;
  std::cout<<"\nDistance travelled: "<< distTraveledMeters <<" meters "  << distTraveledMM <<" mm in "<< timeFromStart <<" secs " << std::endl;

  if(dist > 100) {
    myDesired.setDeltaHeading(angle);
    myDesired.setVel(100);
  }
  else {
    myDesired.setVel(0);
  }




// Return the list of desired actions.
  return &myDesired;
}
