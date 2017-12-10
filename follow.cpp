//#include "follow.hh"
#include "/usr/local/Aria/include/Aria.h"
#include </usr/local/Aria/include/ariaUtil.h>
#include </usr/local/Aria/include/ArActionGroup.h>
#include </usr/local/Aria/include/ArFunctor.h>
#include <list>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

class PipFollow: public ArAction{
  public:
    ArRangeDevice *sonar;
    ArRangeDevice *laser;
    ArRobot *myRobot;
    virtual ArActionDesired *fire(ArActionDesired currentDesired);
    PipFollow(ArRobot *robot);
    ~PipFollow(void);
  protected:
    ArActionDesired myDesired;
};

PipFollow::PipFollow(ArRobot *robit):ArAction("follow","follows a thing"){
	ArAction::setRobot(robit);
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
  // just a bunch of print statements and stuff
  double angle = 0;
  double dist = laser->currentReadingPolar(-90, 90, &angle);
  double distTraveledMeters = static_cast<int>(myRobot->getOdometerDistance()/1000);
  double distTraveledMM = static_cast<int>(myRobot->getOdometerDistance())% 1000;
  double timeFromStart = myRobot->getOdometerTime();
  double angleFromStart = myRobot->getOdometerDegrees();
  double PoseX=myRobot->getX();
  double PoseY=myRobot->getY();
  bool contFollow = true;

  std::cout <<  "\nMessage: Distance to closes object: " << dist;
  std::cout <<  "\nDistance traveled: "<< distTraveledMeters <<" meters "  << distTraveledMM <<" mm in "<< timeFromStart <<" secs ";
  std::cout <<  "\nRotated " << angleFromStart<< " degrees.";

  //std::cout <<  "\nAt " << PoseX<< "," << PoseY;
  ArLog::log(ArLog::Normal,"\nPress S to Stop");
  ArLog::log(ArLog::Normal,"\nPress H to go home");
  while(contFollow){
    if(dist > 100){
      myDesired.setDeltaHeading(angle);
      myDesired.setVel(600);
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
        //pip.goHome
      }
    }
    // Return the list of desired actions.
    return &myDesired;
  }
}

int main(int argc, char** argv){
  Aria::init();
  ArArgumentParser parser(&argc, argv);
  parser.loadDefaultArguments();
  ArRobot pipMan;
  ArRobotConnector robotConnector(&parser, &pipMan);

  if (!robotConnector.connectRobot())
    {
      // Error connecting:
      // if the user gave the -help argumentp, then just print out what happened,
      // and continue so options can be displayed later.
      if (!parser.checkHelpAndWarnUnparsed()){
        ArLog::log(ArLog::Terse, "Could not connect to robot, will not have parameter file so options displayed later may not include everything");
      }
      // otherwise abort
      else{
        ArLog::log(ArLog::Terse, "Error, could not connect to robot.");
        Aria::logOptions();
        Aria::exit(1);
      }
    }
    if(!pipMan.isConnected())
    {
      ArLog::log(ArLog::Terse, "Internal error: robot connector succeeded but ArRobot::isConnected() is false!");
    }


    if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
      {
        Aria::logOptions();
        Aria::exit(1);
        return 1;
      }
  ArLaserConnector laserConnector(&parser, &pipMan, &robotConnector);

  ArSonarDevice sonar;

  pipMan.addRangeDevice(&sonar);

  if (!laserConnector.connectLasers(
         false,  // continue after connection failures
         false,  // add only connected lasers to ArRobot
         true    // add all lasers to ArRobot
   ))
   {
      printf("Warning: Could not connect to laser(s). Set LaserAutoConnect to false in this robot's individual parameter file to disable laser connection.\n");
   }
   ArLog::log(ArLog::Normal, "Connected and ready to go");

  ArUtil::sleep(1000);
  pipMan.runAsync(true); 	//Run in asynchronous mode

\
  PipFollow follow = PipFollow(&pipMan);
  ArLog::log(ArLog::Normal, "follow action object created");


  pipMan.addAction(&follow,42);
  ArLog::log(ArLog::Normal, "follow action added to pip");

  pipMan.comInt(ArCommands::ENABLE,1);
}
