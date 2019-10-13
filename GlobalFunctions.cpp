#include "GlobalFunctions.h"

float getScaledValue(float value, float curMin, float curMax, float newMin, float newMax)
{
	float result = (value - curMin) / (curMax - curMin);
	result *= (newMax - newMin);
	result += newMin;

	return result;
}

float getPlaneAngle(int axis, glm::vec3 plane[3])
{
	//Get two vectors on the plane
	glm::vec3 vectorOne = plane[2] - plane[1];
	glm::vec3 vectorTwo = plane[2] - plane[0];

	//Normalize vectors
	vectorOne = glm::normalize(vectorOne);
	vectorTwo = glm::normalize(vectorTwo);

	//Find normal vector to the plane and normalize
	glm::vec3 norm = glm::cross(vectorOne, vectorTwo);
	norm = glm::normalize(norm);

	switch (axis)
	{
	case 0:
		return glm::orientedAngle(norm, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));	//Find angle with X axis; please review and correct if needed
		break;
	case 1:
		return glm::orientedAngle(norm, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));	//Find angle with Y axis; please review and correct if needed
		break;
	case 2:
		return glm::orientedAngle(norm, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));	//Find angle with Z axis; please review and correct if needed
		break;
	default:
		break;
	}
}