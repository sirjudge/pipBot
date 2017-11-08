#include "/usr/local/Aria/include/Aria.h"
#include </usr/local/Aria/include/ariaUtil.h>
#include </usr/local/Aria/include/ArActionGroup.h>
#include </usr/local/Aria/include/ArFunctor.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>

//Nico, Konrad, Cat

ArSick pewpew;
ArSonarDevice sonar;
ArRobot pipMan;

	void obstacleAvoid(double minAng, double driveSpeed, ArRobot thisRobot){
		ArRobot ar = thisRobot;
		double avoidAngle; //Angle to turn to avoid obstacle
		if (minAng >= 0 && minAng < 46 ){
			std::cout << "TURNING RIGHT!\n";
			avoidAngle = -30.0;
		} //If obstacle is to the left
		if (minAng > -46 && minAng < 0){
			std::cout << "TURNING LEFT!\n";
			avoidAngle = 30.0;
		}
		ar.setRotVel(avoidAngle);
		ar.setVel(driveSpeed);
		//Turn right
		//If obstacle is to the right
		//Turn left
		//Set rotational speed
		//Set translational speed
	}

	//TODO: change pointer stuff
		/*
		 foo(int *bar){
			*bar++;
		 }
		 when passing in variables with * operator you do need to point to it
			int x = 1
			foo(*x)

		foo(int& bar){
			bar++;
		}
		when passing in variables with & operator you don't need to point to anything in calling the method
			int x = 1
			foo(x)
	*/
	void follow(ArRobot *ar,ArSick& l){
		ar->enableMotors();
		std::cout<<"am I working?";
		double angle = 0;
		double dist = l.currentReadingPolar(-90, 90, &angle);
		double distTraveledMeters = static_cast<int>(ar->getOdometerDistance()/1000);
		double distTraveledMM = static_cast<int>(ar->getOdometerDistance())% 1000;
		double timeFromStart = ar->getOdometerTime();
		double angleFromStart = ar->getOdometerDegrees();
		double PoseX= ar->getX();
		double PoseY= ar->getY();

		std::cout << "Message: Distance to closes object: " << dist << "\n" << std::endl;
		std::cout<<"\nDistance traveled: "<< distTraveledMeters <<" meters "  << distTraveledMM <<" mm in "<< timeFromStart <<" secs " << std::endl;
		std::cout<< "\nRotated " << angleFromStart<< " degrees." <<std::endl;
		std::cout<< "\nAt " << PoseX<< "," << PoseY <<std::endl;
		std::cout<< "\nPress S to Stop" <<std::endl;
		std::cout<< "\nPress H to go home" <<std::endl;

		while(1){
		  while(dist > 100){
			  ar->lock();
			  ar->setVel(200);
			  ar->unlock();
			  ArUtil::sleep(3000);
			  dist = l.currentReadingPolar(-90, 90, &angle);
			  ArLog::log(ArLog::Normal, "following");
			  sleep(1000);
			  if(dist < 60){
				  //obstacleAvoid(20.0,40.0,ar);
			  }
			  else{
				  ar->lock();
				  ar->setRotVel(0);
				  ar->setVel(300);
				  ar->unlock();
			  }
			}
			ar->stop();
		  }
	}

	int main(int argc, char **argv) {
		Aria::init();   //Initialize ARIA library
		ArRobot *robot; //Instantiate robot
		ArArgumentParser parser(&argc, argv);   //Instantiate argument parser
		ArSimpleConnector connector(& parser); 	//Instantiate connector

		/* Connection to robot */
		parser.loadDefaultArguments(); //Load default values
		//if arguments passed in are unknown, exit
		if (!connector.parseArgs()){
			printf("Unknown settings\n");
			Aria::exit(0);
			exit(1);
		}
		//if computer cannot establish a connection with the robot, exit
		if (!connector.connectRobot(&pipMan)){
			printf("Unable to connect\n");
			Aria::exit(0);
			exit(1);
		}
		std::cout<<"init robot accessories";
		/*Initialize a laser and sonar device and link it to the robot */
		robot->addRangeDevice(&sonar);
		robot->addRangeDevice(&pewpew);

		std::cout<<"added range sonar + laser";
		robot->runAsync(true);					//Run in asynchronous mode
		robot->lock(); 							//Lock robot during set up
		robot->comInt(ArCommands::ENABLE, 1); 	//Turn on the motors
		robot->unlock(); 						//Unlock the robot

		// Used to perform actions when keyboard keys are pressed
		// ArRobot contains an exit action for the Escape key. It also
		// stores a pointer to the keyhandler so that other parts of the program can
		// use the same keyhandler.
		ArKeyHandler keyHandler;
		Aria::setKeyHandler(&keyHandler);
		pipMan.attachKeyHandler(&keyHandler);
		printf("You may press escape to exit\n");

		std::cout<<"before follow method runs";
		follow(robot, pewpew);
		std::cout<<"After follow method runs";
		Aria::exit(0);
		return 0;
	}
