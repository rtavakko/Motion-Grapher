#include "ProfilePoint.h"

ProfilePoint::ProfilePoint(int ind, int * timeData, float * actuatorData, ProfileFormat pFormat)
{
	index = ind;

	format = pFormat;

	if (time != nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			time[i] = timeData[i];
		}
	}

	data = new float[format.dataCount];

	if (actuatorData != nullptr)
	{
		for (int i = 0; i < format.dataCount; i++)
		{
			data[i] = actuatorData[i];
		}
	}

	//Calculate roll limits for this format 
	RPHLowerLimit[0] = getPlaneAngle(2, format.actuatorPositionsRollLeft);	
	RPHUpperLimit[0] = getPlaneAngle(2, format.actuatorPositionsRollRight);

	//Calculate pitch limits for this format 
	RPHLowerLimit[1] = getPlaneAngle(0, format.actuatorPositionsPitchForward);
	RPHUpperLimit[1] = getPlaneAngle(0, format.actuatorPositionsPitchBack);

	//Calculate heave limits for this format 
	RPHLowerLimit[2] = -1.0f;
	RPHUpperLimit[2] = 1.0f;

	if (data != nullptr && format.actuatorCount == 3)
	{
		//Update actuator positions with current actuator data
		glm::vec3 actuatorPosition[3];
		for (int i = 0; i < format.actuatorCount; i++)
		{
			actuatorPosition[i] = format.actuatorPositionsNeutral[i];
			actuatorPosition[i].y = getScaledValue(data[i], format.actuatorLowerLimit, format.actuatorUpperLimit, -1.0f, 1.0f);
		}

		//Find RP data from current motion base position
		RPHData[0] = getPlaneAngle(2, actuatorPosition);	//Calculate roll (around GL Z axis)
		RPHData[1] = getPlaneAngle(0, actuatorPosition);	//Calculate pitch (around GL X axis)
		RPHData[2] = 0.0f;

		//Find H data from current actuator data
		for (int i = 0; i < format.actuatorCount; i++)
		{
			RPHData[2] = std::max(RPHData[2], data[i]);
			RPHData[2] = getScaledValue(RPHData[2], format.actuatorLowerLimit, format.actuatorUpperLimit, RPHLowerLimit[2], RPHUpperLimit[2]);
		}
	}
}

ProfilePoint::ProfilePoint()
{
	index = -1;

	format = sixDV_Festo;

	for (int i = 0; i < 4; i++)
	{
			time[i] = 0;
	}

	data = new float[format.dataCount];

	for (int i = 0; i < format.dataCount; i++)
	{
		data[i] = 0.0f;
	}

	//Calculate roll limits for this format 
	RPHLowerLimit[0] = getPlaneAngle(2, format.actuatorPositionsRollLeft);
	RPHUpperLimit[0] = getPlaneAngle(2, format.actuatorPositionsRollRight);

	//Calculate pitch limits for this format 
	RPHLowerLimit[1] = getPlaneAngle(0, format.actuatorPositionsPitchForward);
	RPHUpperLimit[1] = getPlaneAngle(0, format.actuatorPositionsPitchBack);

	//Calculate heave limits for this format 
	RPHLowerLimit[2] = -1.0f;
	RPHUpperLimit[2] = 1.0f;

	//Set RPH data to 0
	for (int i = 0; i < 3; i++)
	{
		RPHData[i] = 0.0f;
	}
}

ProfilePoint::ProfilePoint(const ProfilePoint & point)
{
	index = point.index;
	format = point.format;

	if (point.time != nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			time[i] = point.time[i];
		}
	}

	if (point.data != nullptr)
	{
		data = new float[format.dataCount];
		for (int i = 0; i < format.dataCount; i++)
		{
			data[i] = point.data[i];
		}
	}

	if (point.RPHData != nullptr)
	{
		for (int i = 0; i < 3; i++)
		{
			RPHData[i] = point.RPHData[i];

			RPHLowerLimit[i] = point.RPHLowerLimit[i];
			RPHUpperLimit[i] = point.RPHUpperLimit[i];
		}
	}
}

ProfilePoint::~ProfilePoint()
{
	if (data != nullptr)
	{
		delete [] data;
		data = nullptr;
	}
}

ProfilePoint& ProfilePoint::operator=(const ProfilePoint& point)
{
	if (this == &point)
	{
		return *this;
	}
	else
	{
		index = point.index;
		format = point.format;

		if (point.time != nullptr)
		{
			for (int i = 0; i < 4; i++)
			{
				time[i] = point.time[i];
			}
		}

		if (data != nullptr)
		{
			delete[] data;
			data = nullptr;
		}

		if (point.data != nullptr)
		{
			data = new float[format.dataCount];
			for (int i = 0; i < format.dataCount; i++)
			{
				data[i] = point.data[i];
			}
		}

		if (point.RPHData != nullptr)
		{
			for (int i = 0; i < 3; i++)
			{
				RPHData[i] = point.RPHData[i];

				RPHLowerLimit[i] = point.RPHLowerLimit[i];
				RPHUpperLimit[i] = point.RPHUpperLimit[i];
			}
		}
	}
}

void ProfilePoint::updateFormat(ProfileFormat newFormat)
{
	//Convert frame value to new format
	float newFrame = time[3];

	newFrame = getScaledValue(newFrame, 0.0f, format.timeFrameUpperLimit, 0.0f, newFormat.timeFrameUpperLimit);

	time[3] = static_cast<int>(newFrame);

	//Convert data values to new format
	for (int i = 0; i < format.dataCount; i++)
	{
		data[i] = getScaledValue(data[i], format.actuatorLowerLimit, format.actuatorUpperLimit, newFormat.actuatorLowerLimit, newFormat.actuatorUpperLimit);
	}
	
	format = newFormat;

	//Recalculate roll limits for this format 
	RPHLowerLimit[0] = getPlaneAngle(2, format.actuatorPositionsRollLeft);
	RPHUpperLimit[0] = getPlaneAngle(2, format.actuatorPositionsRollRight);

	//Recalculate pitch limits for this format 
	RPHLowerLimit[1] = getPlaneAngle(0, format.actuatorPositionsPitchForward);
	RPHUpperLimit[1] = getPlaneAngle(0, format.actuatorPositionsPitchBack);

	//Recalculate heave limits for this format 
	RPHLowerLimit[2] = -1.0f;
	RPHUpperLimit[2] = 1.0f;
}

void ProfilePoint::updateData(int ind, int* timeData, float* actuatorData)
{
	index = ind;

	if (time != nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			time[i] = timeData[i];
		}
	}

	if (data != nullptr)
	{
		delete[] data;
		data = nullptr;
	}
	data = new float[format.dataCount];

	if (actuatorData != nullptr)
	{
		for (int i = 0; i < format.dataCount; i++)
		{
			data[i] = actuatorData[i];
		}
	}

	if (data != nullptr && format.actuatorCount == 3)
	{
		//Update actuator positions with current actuator data
		glm::vec3 actuatorPosition[3];
		for (int i = 0; i < format.actuatorCount; i++)
		{
			actuatorPosition[i] = format.actuatorPositionsNeutral[i];
			actuatorPosition[i].y = getScaledValue(data[i], format.actuatorLowerLimit, format.actuatorUpperLimit, -1.0f, 1.0f);
		}

		//Find RP data from current motion base position
		RPHData[0] = getPlaneAngle(2, actuatorPosition);	//Calculate roll (around GL Z axis)
		RPHData[1] = getPlaneAngle(0, actuatorPosition);	//Calculate pitch (around GL X axis)
		RPHData[2] = 0.0f;

		//Find H data from current actuator data
		for (int i = 0; i < format.actuatorCount; i++)
		{
			RPHData[2] = std::max(RPHData[2], data[i]);
			RPHData[2] = getScaledValue(RPHData[2], format.actuatorLowerLimit, format.actuatorUpperLimit, RPHLowerLimit[2], RPHUpperLimit[2]);
		}

		//fprintf(stderr, "[%i][%f(%f-%f),%f(%f-%f),%f(%f-%f)]--[%f,%f,%f]\n", index, RPHData[0],RPHLowerLimit[0],RPHUpperLimit[0], RPHData[1], RPHLowerLimit[1], RPHUpperLimit[1], RPHData[2], RPHLowerLimit[2], RPHUpperLimit[2], data[0], data[1], data[2]);
	}
}

void ProfilePoint::updateData(int ind)
{
	index = ind;
}

void ProfilePoint::updateData(float* RPH)
{
	glm::vec3 actuatorPosition[3];

	if (RPH != nullptr && format.actuatorCount == 3)
	{
		for (int i = 0; i < format.actuatorCount; i++)
		{
			//Update RPH data
			RPHData[i] = RPH[i];

			actuatorPosition[i] = format.actuatorPositionsNeutral[i];

			//Update actuator positions based on updated RPH data
			actuatorPosition[i] = glm::rotateZ(actuatorPosition[i], RPHData[0]);
			actuatorPosition[i] = glm::rotateX(actuatorPosition[i], RPHData[1]);
			//TO DO: CALCULATE ACTUATOR DATA OFFSET FROM HEAVE / RPHData[2]

			//Update actuator data based on updated actuator positions
			data[i] = getScaledValue(actuatorPosition[i].y,-1.0f,1.0f,format.actuatorLowerLimit,format.actuatorUpperLimit);
		}
	}
}

void ProfilePoint::updateData(int RPHInd, float RPH)
{
	glm::vec3 actuatorPosition[3];

	//Update RPH data for current axis
	RPHData[RPHInd] = RPH;

	if (format.actuatorCount == 3)
	{
		for (int i = 0; i < format.actuatorCount; i++)
		{
			actuatorPosition[i] = format.actuatorPositionsNeutral[i];

			//Update actuator positions based on updated RPH data
			glm::rotateZ(actuatorPosition[i], RPHData[0]);
			glm::rotateX(actuatorPosition[i], RPHData[1]);

			//Update actuator data based on updated actuator positions
			data[i] = getScaledValue(actuatorPosition[i].y, -1.0f, 1.0f, format.actuatorLowerLimit, format.actuatorUpperLimit);
		}
	}
}

int ProfilePoint::getIndex()
{
	return index;
}

const int * ProfilePoint::getTime()
{
	const int* value = time;
	return value;
}

const float* ProfilePoint::getData()
{
	const float* value = data;
	return value;
}

const float * ProfilePoint::getRPH()
{
	const float* value = RPHData;
	return value;
}

ofPoint ProfilePoint::getRPHLimits(int RPH)
{
	return ofPoint(RPHLowerLimit[RPH],RPHUpperLimit[RPH]);
	//return ofPoint(-3.14f, 3.14f);
}
