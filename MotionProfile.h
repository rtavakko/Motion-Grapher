#pragma once

#include "ofMain.h"
#include "ofEvent.h"

#include "MotionGraph.h"

#include "GlobalFunctions.h"

#include <iosfwd>
#include <iostream>
#include <fstream>
#include <sstream>

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

class MotionProfile
{
public:
	MotionProfile(std::string file, ProfileFormat pFormat);
	MotionProfile(ProfileFormat pFormat);
	~MotionProfile();

	void connectGraph(MotionGraph* pGraph);	//Connect and existing graph

	void updateFileName(std::string file);	//Update name of input file
	void updateAutoSaveFileName(std::string file);	//Update name of autosave file

	bool read();	//Read current input file, parse raw actuator data and store in connected graph; NOTE: create equivalent for SimEx master file
	bool write(std::string file);	//Write data from connected graph to a file; NOTE: create equivalent for SimEx master file 
	bool save();	//Write data from connected graph to current input file
	bool saveAs(std::string file, ProfileFormat pFormat);	//Convert data from a connected graph to a new format and write to a file

	void record(float* RPH, int axisCount, float lowerLimit, float upperLimit); //Receive data array with specified axis count and limits and store in connected graph

	bool convert(ProfileFormat newFormat); //Convert data from a connected graph to a new format

	void updateIndex(int ind); //Update current profile index

	void graphPointsUpdated(std::vector<ProfilePoint>& points); //Event method called by connected graph

	ProfilePoint& insertGraphPoint();	//Insert a new profile point or return an existing one from connected graph

	//Update profile point data of a given profile point
	void updateProfilePointData(ProfilePoint& point, int posInd, int* time, float* data);
	void updateProfilePointData(ProfilePoint& point, float* RPH);

	//Get data of the profile point at current index of connected graph
	const float* getRPHData();
	const float* getActuatorData();

	int getSize();	//Get size of connected graph
	int* getLength();	//Get time stamp of last data point in connected graph

	ProfileFormat getFormat();	//Get current profile format

private:
	bool parse(); //Parse raw actuator data stored in stringstream, NOTE: create equivalent for SimEx master file

	void updateGraph();	//Call the update method of connected graph; used after updates are made to graph

	void splitTimeString(std::string timeString, int* time);	//Split a time string into its HMSF components and place in a give array
	bool splitDataString(std::string dataString, float* data, int dataCount); //Split an actuator data string into its components and place in a give array

private:
	std::string fileName;	//Name of current input file
	std::string autoSaveFileName;	//Name of autosavefile
	ProfileFormat format;	//Format of the profile

	std::fstream fileHandle;	//File IO object
	std::stringstream rawData;	//String buffer storing the raw data from file

	MotionGraph* graph;	//Pointer to graph object

	int currentIndex;	//Current index of profile

	int length[4];	//Current length of profile
};

