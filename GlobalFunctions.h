#pragma once

#include "ofMain.h"
#include "ProfilePoint.h"

float getScaledValue(float value, float curMin, float curMax, float newMin, float newMax);	//Scale a value from one range to another: [currMin-currMax]->[newMin-newMax]

float getPlaneAngle(int axis, glm::vec3 plane[3]);	//Get angle of a plane (radians) made with a given axis (axis index: X:0,Y:1,Z:2)