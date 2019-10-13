#include "MotionModel.h"



MotionModel::MotionModel()
{
	fileName = "FESTO.stl";
	model.loadModel(fileName);

	rotAngX = 0.0f;
	rotAngY = 0.0f;
	rotAngZ = 0.0f;

	scaleX = 1.0f;
	scaleY = 1.0f;
	scaleZ = 1.0f;

	posX = 0.0f;
	posY = 0.0f;
	posZ = -500.0f;
}

MotionModel::MotionModel(std::string file)
{
	fileName = file;
	model.loadModel(fileName);

	rotAngX = 0.0f;
	rotAngY = 0.0f;
	rotAngZ = 0.0f;

	scaleX = 1.0f;
	scaleY = 1.0f;
	scaleZ = 1.0f;

	posX = 0.0f;
	posY = 0.0f;
	posZ = -500.0f;
}

MotionModel::~MotionModel()
{
}

void MotionModel::rotate(float aX, float aY, float aZ)
{
	rotAngX = aX;
	rotAngY = aY;
	rotAngZ = aZ;
}

void MotionModel::scale(float sX, float sY, float sZ)
{
	scaleX = sX;
	scaleY = sY;
	scaleZ = sZ;
}

void MotionModel::move(float pX, float pY, float pZ)
{
	posX = pX;
	posY = pY;
	posZ = pZ;
}

void MotionModel::draw()
{
	ofBackground(200,200,200,255);
	ofSetColor(255, 255, 255, 255);

	light.enable();

	camera.begin();

	model.setPosition(posX, posY, posZ);
	model.setScale(scaleX, scaleY, scaleZ);

	model.setRotation(0, rotAngX, 1, 0, 0);
	model.setRotation(1, rotAngY, 0, 1, 0);
	model.setRotation(2, rotAngZ, 0, 0, 1);

	model.drawFaces();

	camera.end();

	light.disable();
}
