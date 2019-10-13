#include "MotionProfile.h"

MotionProfile::MotionProfile(std::string file, ProfileFormat pFormat)
{
	fileName = file;
	autoSaveFileName = fileName;
	format = pFormat;

	graph = nullptr;

	currentIndex = 0;

	for (int i = 0; i < 4; i++)
	{
		length[i] = 0;
	}
}

MotionProfile::MotionProfile(ProfileFormat pFormat)
{
	fileName = "test.dat";
	autoSaveFileName = fileName;
	format = pFormat;

	graph = nullptr;

	currentIndex = 0;

	for (int i = 0; i < 4; i++)
	{
		length[i] = 0;
	}
}

MotionProfile::~MotionProfile()
{
	if (graph != nullptr)
	{
		ofRemoveListener(graph->pointsUpdated, this, &MotionProfile::graphPointsUpdated);	//Remove profile method as graph event listener
	}
}

void MotionProfile::connectGraph(MotionGraph* pGraph)
{
	if (pGraph != nullptr)
	{
		graph = pGraph;
		ofAddListener(graph->pointsUpdated, this, &MotionProfile::graphPointsUpdated);	//Add profile method as graph event listener
	}
}

void MotionProfile::updateFileName(std::string file)
{
	fileName = file;
}

void MotionProfile::updateAutoSaveFileName(std::string file)
{
	autoSaveFileName = file;
}

bool MotionProfile::read()
{
	fileHandle.open(fileName, std::ifstream::binary || std::ifstream::in);	//Open file

	if (fileHandle.is_open())
	{
		fprintf(stderr, "READING %s...\n", fileName.c_str());

		rawData << fileHandle.rdbuf();	//Read file data

		fileHandle.close();	//Close file

		return parse();	//Parse data in buffer
	}
	else
	{
		fprintf(stderr, "FAILED TO READ\n");
		return false;
	}
}

bool MotionProfile::parse()
{
	std::string dataLine;

	size_t dataIndex = 0;
	size_t delimiterIndex = 0;

	std::string timeString = "";
	std::string dataString = "";

	int actCount = format.actuatorCount;

	int* timeData;
	float* actData;

	if (rawData.str() != "")
	{
		currentIndex = 0;

		timeData = new int[4];
		actData = new float[actCount];

		while (!rawData.eof())
		{
			getline(rawData, dataLine, format.eolDelimiterLast);

			delimiterIndex = dataLine.find(format.spacerDelimiter);	//Find the position of the spacer delimiter in current line

			//Split current line into a time string and a data string using the spacer delimiter
			timeString = dataLine.substr(dataIndex, delimiterIndex);
			dataString = dataLine.substr(delimiterIndex + 1, dataLine.length() - 1);

			splitTimeString(timeString, timeData);
			bool completed = splitDataString(dataString, actData, actCount);	//Use the return flag from this method to check if data values are all ""

			//If flag is set to false, we have reached the end of the file; other wise the extracted data is valid
			if (completed) 
			{
				updateProfilePointData(insertGraphPoint(), currentIndex, timeData, actData);
				currentIndex++;
			}
		}

		updateGraph();

		delete[] timeData;
		delete[] actData;

		return true;
	}
	else
	{
		return false;
	}
}

bool MotionProfile::write(std::string file)
{
	int profileSize = getSize();

	if (graph != nullptr)
	{
		fileHandle.open(file, std::ifstream::binary | std::ifstream::out);

		if (fileHandle.is_open())
		{
			fprintf(stderr, "WRITING TO %s...\n", file.c_str());

			//Reset profile and connected graph indices
			currentIndex = 0;
			graph->updateIndex(currentIndex);

			for (int i = 0; i < profileSize; i++)
			{
				//Get current graph point and read its data
				ProfilePoint& point = graph->getPoint();
				const int* time = point.getTime();
				const float* data = point.getData();

				std::string timeString = "";
				std::string dataString = "";

				for (int t = 4 - format.timeCount; t < 4; t++)
				{
					if (t < 2)
					{
						//HMS time data
						timeString = (time[t] < 10) ? ("0" + std::to_string(time[t])) : (std::to_string(time[t]));
						fileHandle << timeString << format.timeDelimiterHMS;
						//fprintf(stderr, "%i,", time[t]);
					}
					else if (t < 3)
					{
						//Frame time data
						timeString = (time[t] < 10) ? ("0" + std::to_string(time[t])) : (std::to_string(time[t]));
						fileHandle << timeString << format.timeDelimiterF;
						//frintf(stderr, "%i,", time[t]);
					}
					else
					{
						//Spacer delimiter
						timeString = (time[t] < 10) ? ("0" + std::to_string(time[t])) : (std::to_string(time[t]));
						fileHandle << timeString << format.spacerDelimiter;
						//frintf(stderr, "%i,", time[t]);
					}
				}
				//fprintf(stderr, ",");

				for (int d = 0; d < format.dataCount; d++)
				{
					//Check if we are currently storing the last data value
					if (d < format.dataCount - 1)
					{
						if (d < format.actuatorCount)
						{
							//Actuator data that was read and processed (stored in the actuator arrangement specified by the format)
							dataString = std::to_string(static_cast<int>(data[format.actuatorIndices[d]]));
							fileHandle << dataString << format.dataDelimiter;
							//frintf(stderr, "%f,", data[d]);
						}
						else
						{
							//Other data not read or processed (assumed to have been 0)
							dataString = std::to_string(0);
							fileHandle << dataString << format.dataDelimiter;
							//frintf(stderr, "%f,", data[d]);
						}
					}
					//Place last value and the end of line delimiters
					else
					{
						if (d < format.actuatorCount)
						{
							//Actuator data that was read and processed (stored in the actuator arrangement specified by the format)
							dataString = std::to_string(static_cast<int>(data[format.actuatorIndices[d]]));
							fileHandle << dataString << format.eolDelimiterFirst << format.eolDelimiterLast;
							//frintf(stderr, "%f,", data[d]);
						}
						else
						{
							//Other data not read or processed (assumed to have been 0)
							dataString = std::to_string(0);
							fileHandle << dataString << format.eolDelimiterFirst << format.eolDelimiterLast;
							//frintf(stderr, "%f,", data[d]);
						}
					}
				}
				//frintf(stderr, "\n");
			}
			fprintf(stderr, "FINISHED\n");

			fileHandle.flush();
			fileHandle.close();

			return true;
		}
		else
		{
			fprintf(stderr, "FAILED TO WRITE\n");
			return false;
		}
	}
	return false;
}

bool MotionProfile::save()
{
	return write(fileName);
}

bool MotionProfile::saveAs(std::string file, ProfileFormat pFormat)
{
	convert(pFormat);
	return write(file);
}

void MotionProfile::record(float * RPH, int axisCount, float lowerLimit, float upperLimit)
{
	//Currently designed for 3DOF data only; reimplement if needed
	ProfilePoint& point = insertGraphPoint();

	float RPHScaled[3];

	ofPoint RPHLimits;

	if (graph != nullptr && RPH != nullptr && axisCount == 3)
	{
		point.updateFormat(format);	//Set format of current point if not already set
		for (int i = 0; i < 3; i++)
		{
			RPHLimits = point.getRPHLimits(i);	
			RPHScaled[i] = getScaledValue(RPH[i],lowerLimit,upperLimit, RPHLimits.x, RPHLimits.y);	//Scale raw RPH data to fit profile format
		}
		updateProfilePointData(point, RPHScaled);	//Update current point with scaled RPH data
		currentIndex = graph->getIndex();	//Get updated index of connected graph 

		updateGraph();
	}
}

bool MotionProfile::convert(ProfileFormat newFormat)
{
	if (format != newFormat)
	{
		format = newFormat;

		if (graph != nullptr)
		{
			currentIndex = 0;
			graph->updateIndex(currentIndex);

			for (int i = 0; i < graph->getSize(); i++)
			{
				graph->getPoint().updateFormat(format);
			}
			return true;
		}
		return false;
	}
	return false;
}

void MotionProfile::updateIndex(int ind)
{
	currentIndex = ind;
}

void MotionProfile::graphPointsUpdated(std::vector<ProfilePoint>& points)
{
	//Perform any operations needed if graph points are updated
}

ProfilePoint& MotionProfile::insertGraphPoint()
{
	if (graph != nullptr)
	{
		return graph->insertPoint();
	}
}

void MotionProfile::updateProfilePointData(ProfilePoint& point, int posInd, int* time, float* data)
{
	if (graph != nullptr)
	{
		point.updateFormat(format);
		point.updateData(posInd, time, data);
	}
}

void MotionProfile::updateProfilePointData(ProfilePoint& point, float * RPH)
{
	if (graph != nullptr)
	{ 
		point.updateData(RPH);
	}
}

const float * MotionProfile::getRPHData()
{
	if (graph != nullptr)
	{
		return graph->getPoint().getRPH();
	}
	return nullptr;
}

const float * MotionProfile::getActuatorData()
{
	if (graph != nullptr)
	{
		return graph->getPoint().getData();
	}
	return nullptr;
}

int MotionProfile::getSize()
{
	if (graph != nullptr)
	{
		return graph->getSize();
	}
	return -1;
}

int* MotionProfile::getLength()
{
	int currSize = getSize();
	const int* currLength;

	if (graph != nullptr)
	{
		if (currSize != 0)
		{
			currLength = graph->getPoints()[currSize - 1].getTime();

			for (int i = 0; i < 4; i++)
			{
				length[i] = currLength[i];
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				length[i] = 0;
			}
		}
	}
	return length;
}

ProfileFormat MotionProfile::getFormat()
{
	return format;
}

void MotionProfile::updateGraph()
{
	if (graph != nullptr)
	{
		graph->update();
	}
}

void MotionProfile::splitTimeString(std::string timeString, int* time)
{
	std::string updatedString = "";

	const int count = 4;
	std::string hmsf[count];	//String array storing split time data

	int hmsfIndex = count - 1;	//Set string array index to the last element (frame)

	for (int i = 0; i < count; i++)
	{
		hmsf[i] = "";	//Initialize time data 
	}

	//String positions for start of substring and target delimiter
	size_t startIndex = 0;	
	size_t delimiterIndex = 0;

	delimiterIndex = timeString.find_last_of(format.timeDelimiterF); //Look for frame value delimiter starting from the right

	updatedString = timeString.substr(startIndex, delimiterIndex);	//Store HMS time values

	hmsf[hmsfIndex] = timeString.substr(delimiterIndex + 1, timeString.length() - 1);	//Store frame value

	timeString = updatedString;	//Update time string

	hmsfIndex--;

	while (true)
	{
		delimiterIndex = timeString.find_last_of(format.timeDelimiterHMS);	//Look for HMS value delimiter starting from the right

		//If there are no more HMS delimiters we have the last time value
		if (delimiterIndex == std::string::npos)
		{
			hmsf[hmsfIndex] = timeString;
			break;
		}
		//If an HMS delimiter is found, split the time string, store the value to its right and keep searching
		else
		{
			hmsf[hmsfIndex] = timeString.substr(delimiterIndex + 1, timeString.length() - 1);	//Store current HMS value

			//Update time string to look for remaining HMS values
			updatedString = timeString.substr(startIndex, delimiterIndex);	

			timeString = updatedString;
			hmsfIndex--;
		}
	}

	//Look through split time strings and set them to 00 if they are empty
	for (int i = 0; i < count; i++)
	{
		if (hmsf[i] == "")
		{
			hmsf[i] = "00";
		}
		time[i] = std::stoi(hmsf[i]);
	}
}

bool MotionProfile::splitDataString(std::string dataString, float* data, int dataCount)
{
	std::string updatedString = "";

	std::string* splitData = new std::string[dataCount];	//String array storing split actuator data

	int dataIndex = 0;	//Set string array index to the first element (actuator 1)

	//String positions for start of substring and target delimiter
	size_t startIndex = 0;
	size_t delimiterIndex = 0;

	while (true)
	{
		delimiterIndex = dataString.find(format.dataDelimiter);	//Look for data value delimiter starting from the left
		
		//If there are no more data delimiters we have the last data value
		if (delimiterIndex == std::string::npos)
		{
			splitData[dataIndex] = dataString;
			break;
		}
		//If a data delimiter is found, split the data string, store the value to its left and keep searching
		else
		{
			//Only store valid actuator values as specified by the current profile format
			if (dataIndex < format.actuatorCount)
			{
				splitData[dataIndex] = dataString.substr(startIndex, delimiterIndex);

				updatedString = dataString.substr(delimiterIndex + 1, dataString.length() - 1);	//Store current data value

				//Update data string to look for remaining data values
				dataString = updatedString;
				dataIndex++;
			}
			//Discard any additional actuator or unknown values
			else
			{
				break;
			}
		}
	}

	//Look through split data strings and check if any of them is empty; if this is the case, the current line is invalid and we are at the end of the file
	for (int i = 0; i < format.actuatorCount; i++)
	{
		if (splitData[i] == "")
		{
			return false;
		}

		//Store actuator data based on the actuator arrangement for this format
		data[i] = std::stof(splitData[format.actuatorIndices[i]], NULL);
	}
	delete[] splitData;
	splitData = nullptr;

	return true;
}
