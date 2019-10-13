#pragma once

#include <iosfwd>
#include <iostream>
#include <fstream>
#include <sstream>

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>

#include "MotionModel.h"
#include "MotionProfile.h"

#include <GLFW/glfw3.h>

class MotionControl
{
public:
	MotionControl();
	~MotionControl();

	void connectModel(MotionModel* nModel);	//Connect controller to motion base 3D model
	void connectProfile(MotionProfile* nProfile);	//Connect a profile object

	void findJoystick();	//Find the first available joystick
	int getJoystickAxisCount();	//Return the number of joystick axes

	void setMaxModelRotations(float maX, float maY, float maZ);	//Set maximum rotation angles in degrees for motion base model 

	void play();	//Play motion data from profile or joystick 
	void stop();	//Stop playback / recording
	void record();	//Record data to motion profile

private:
	const float* getAxisDataFromJoystick();	//Get joystick data [-1.0f,1.0f] 
	const float* getAxisDataFromProfile();	//Get roll, pitch, heave data from profile (range will depend on profile)

	const float* getAxisData();	//Get data either from joystick or profile 

private:
	MotionModel* model;	//Pointer to motion base model object
	MotionProfile* profile;	//Pointer to motion profile object

	float modelRotAngX;	//Motion base model rotation angle around X axis in degrees
	float modelRotAngY;	//Motion base model rotation angle around Y axis in degrees
	float modelRotAngZ;	//Motion base model rotation angle around Z axis in degrees

	float maxModelRotAngX;	//Motion base model max rotation angle around X axis in degrees
	float maxModelRotAngY;	//Motion base model max rotation angle around Y axis in degrees
	float maxModelRotAngZ;	//Motion base model max rotation angle around Z axis in degrees

	int joystickID;	//GLFW ID of current joystick [0-16)
	int joystickAxes;	//Number of data axes provided by current joystick (populated when getting data from a connected joystick)

	bool dataFromJoystick;	//Flag set to determine if axis data comes from joystick or profile
};

