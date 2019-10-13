#include "MotionControl.h"



MotionControl::MotionControl()
{
	MotionModel* model = nullptr;
	MotionProfile* profile = nullptr;

	joystickID = 0;
	joystickAxes = 0;

	dataFromJoystick = true;

	findJoystick();
}

MotionControl::~MotionControl()
{
}

void MotionControl::connectModel(MotionModel * nModel)
{
	if (nModel != nullptr)
	{
		model = nModel;
	}
}

void MotionControl::connectProfile(MotionProfile * nProfile)
{
	if (nProfile != nullptr)
	{
		profile = nProfile;
	}
}

void MotionControl::findJoystick()
{
	for (int i = 0; i < 16; i++)
	{
		if (glfwJoystickPresent(i))
		{
			joystickID = i;
			break;
		}
	}
}

int MotionControl::getJoystickAxisCount()
{
	return joystickAxes;
}

void MotionControl::setMaxModelRotations(float maX, float maY, float maZ)
{
	if (model != nullptr)
	{
		maxModelRotAngX = maX;
		maxModelRotAngY = maY;
		maxModelRotAngZ = maZ;
	}
}

void MotionControl::play()
{
	//Implement ofTimer and use to call this function for correct timing and FPS
	dataFromJoystick = false;

	const float* axisData;

	axisData = getAxisData();

	if (axisData != nullptr && model != nullptr && profile != nullptr)
	{
		model->rotate(modelRotAngX,modelRotAngY,modelRotAngZ);
	}
}

void MotionControl::stop()
{
	dataFromJoystick = true;
}

void MotionControl::record()
{
	//Implement ofTimer and use to call this function for correct timing and FPS
	dataFromJoystick = true;

	const float* axisData;
	float RPHData[3];

	axisData = getAxisData();

	if (axisData != nullptr && model != nullptr && profile != nullptr && joystickAxes == 3)
	{
		modelRotAngZ = maxModelRotAngZ * axisData[0];
		modelRotAngY = maxModelRotAngY * axisData[2];
		modelRotAngX = maxModelRotAngX * axisData[1];

		model->rotate(modelRotAngX, modelRotAngY, 180.0f + modelRotAngZ);	//The FESTO model is upside down so it needs a 180.0f rotation around GL Z 

		RPHData[0] = axisData[0];
		RPHData[1] = axisData[1];
		RPHData[2] = axisData[2];

		profile->record(RPHData, joystickAxes, -1.0f, 1.0f);
	}
}

const float * MotionControl::getAxisDataFromJoystick()
{
	return glfwGetJoystickAxes(joystickAxes, &joystickAxes);
}

const float * MotionControl::getAxisDataFromProfile()
{
	return profile->getRPHData();
}

const float * MotionControl::getAxisData()
{
	if (dataFromJoystick)
	{
		return getAxisDataFromJoystick();
	}
	return getAxisDataFromProfile();
}

