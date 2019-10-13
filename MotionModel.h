#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class MotionModel
{
public:
	MotionModel();
	MotionModel(std::string file);
	~MotionModel();

	void rotate(float aX, float aY, float aZ);	//Rotate model in degrees
	void scale(float sX, float sY, float sZ);	//Scale model [0.0f,1.0f]
	void move(float pX, float pY, float pZ);	//Move model
	void draw();	//Draw model

private:
	ofxAssimpModelLoader model;	//OpenGL model 
	ofLight light;	//OpenGL Lighting effect
	ofEasyCam camera;	//OpenGL camera

	std::string fileName;	//Name of 3D model object

	float rotAngX;
	float rotAngY;
	float rotAngZ;

	float scaleX;
	float scaleY;
	float scaleZ;

	float posX;
	float posY;
	float posZ;
};

