#ifndef _FOLLOW_DEFINED_
#define _FOLLOW_DEFINED_

#include "Aria.h"
#include <ArUrg_2_0.h>
#include <ariaUtil.h>
#include <iostream>

class PipFollow : public ArAction
{
 public:
  // constructor
  PipFollow ();
  // destructor, it’s just empty, we don't need to do anything
  virtual ~PipFollow (void) {};
  // fire is what the resolver calls to figure out what this action wants
  virtual ArActionDesired *fire(ArActionDesired currentDesired);
  // sets the robot pointer, also gets the sonar device
  virtual void setRobot(ArRobot *robot);
 protected:
  // this is to hold the laser device form the robot (required)
  ArRangeDevice *myLaser;
  // what the action wants to do (required)
  ArActionDesired myDesired;

  // This is the only part of the class definition you may need to change.
  // If you have any variables you want the robot to keep track of or
  // remember, declare them here.
};

#endif
