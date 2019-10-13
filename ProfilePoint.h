#pragma once

#include "ofMain.h"

#include "ProfileFormat.h"
#include "SimexFormats.h"

#include "GlobalFunctions.h"

class ProfilePoint
{
public:
	ProfilePoint(int ind, int* timeData, float* actuatorData, ProfileFormat pFormat);
	ProfilePoint();
	ProfilePoint(const ProfilePoint &point);	//Copy consturctor
	~ProfilePoint();

	ProfilePoint& operator=(const ProfilePoint& point); //Assignment operator

	void updateFormat(ProfileFormat newFormat);	//Used to convert actuator data from one format to another (e.g. Qiyou to Festo)

	//Overloaded functions to update point data 
	void updateData(int ind, int* timeData, float* actuatorData);
	void updateData(int ind);
	void updateData(float* RPH);
	void updateData(int RPHInd, float RPH);

	int getIndex();	//Get profile index of point

	const int* getTime();	//Get time data of point
	const float* getData();	//Get actuator data of point
	const float* getRPH();	//Get Roll Pitch Heave data of point

	ofPoint getRPHLimits(int RPHInd);	//Get Roll Pitch Heave data limits of point (index: RPH -> 0,1,2)

private:

private:
	int index;	//Index of point as specified by the profile / graph containing it

	ProfileFormat format;

	int time[4];	//Time stamp of point

	float* data;	//Actuator data

	float RPHData[3];	//RPH data (3DOF only)

	//RPH data limits (3DOF only)
	float RPHLowerLimit[3];	
	float RPHUpperLimit[3];
};

