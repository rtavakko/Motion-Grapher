#include "MotionGraph.h"

MotionGraph::MotionGraph(float sX, float sY, float gWidth, float gHeight, int gAxes)
{
	index = 0;

	currentAxis = 0;

	numAxes = gAxes;

	numDisplayPoints = 20;
	firstDisplayPointIndex = 0;

	pointSpacing = gWidth / numDisplayPoints;

	gridSpacingX = gWidth / 10.0f;
	gridSpacingY = gHeight / 10.0f;

	start = ofPoint(sX, sY);

	width = gWidth;
	height = gHeight;

	frameMargin = 20.0f;

	origin = ofPoint(start.x + frameMargin, start.y + height - frameMargin);
	destination = ofPoint(start.x + width - frameMargin, start.y + frameMargin);
	
	curveWidth = width - 2*frameMargin;
	curveHeight = height -2*frameMargin;

	modBoxSelected = false;
	modifying = false;

	modBoxOrigin = ofPoint(0.0f,0.0f);
	modBoxDestination = ofPoint(0.0f, 0.0f);
	
	modPoint = ofPoint(0.0f, 0.0f);

	modIndexStart = 0;
	modIndexEnd = 0;
	modIndexMid = 0;

	grossMotion = true;

	displayGross = new bool[gAxes];
	grossCurve = new ofPolyline[gAxes];
	grossColors = new ofColor[gAxes];

	displayTexture = new bool[gAxes];
	textureCurve = new ofPolyline[gAxes];
	textureColors = new ofColor[gAxes];

	//Add graph methods as mouse event listeners
	ofAddListener(ofEvents().mousePressed, this, &MotionGraph::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &MotionGraph::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &MotionGraph::mouseReleased);
}

MotionGraph::~MotionGraph()
{
	if (displayGross != nullptr)
	{
		delete[] displayGross;
		displayGross = nullptr;
	}

	if (grossCurve != nullptr)
	{
		delete[] grossCurve;
		grossCurve = nullptr;
	}

	if (grossColors != nullptr)
	{
		delete[] grossColors;
		grossColors = nullptr;
	}

	if (displayTexture != nullptr)
	{
		delete[] displayTexture;
		displayTexture = nullptr;
	}

	if (textureCurve != nullptr)
	{
		delete[] textureCurve;
		textureCurve = nullptr;
	}

	if (textureColors != nullptr)
	{
		delete[] textureColors;
		textureColors = nullptr;
	}

	//Clear data and swap with an empty vector to deallocate memory
	grossPoints.clear();
	std::vector<ProfilePoint>().swap(grossPoints);	

	texturePoints.clear();
	std::vector<ProfilePoint>().swap(texturePoints);

	buffer.clear();
	std::vector<ProfilePoint>().swap(buffer);

	//Remove graph methods as mouse event listeners
	ofRemoveListener(ofEvents().mousePressed, this, &MotionGraph::mousePressed);
	ofRemoveListener(ofEvents().mouseDragged, this, &MotionGraph::mouseDragged);
	ofRemoveListener(ofEvents().mouseReleased, this, &MotionGraph::mouseReleased);
}

ProfilePoint & MotionGraph::insertPoint()
{
	if (index < grossPoints.size())
	{
		return getPoint();	//Point exists, return it
	}
	else
	{
		grossPoints.push_back(ProfilePoint());	//Insert a new point for gross motion
		texturePoints.push_back(ProfilePoint());	//Insert a new point for texture motion
		
		//Increment graph index and adjust start of display window
		index++;
		firstDisplayPointIndex = (index > firstDisplayPointIndex + numDisplayPoints - 1) ? (firstDisplayPointIndex + 1) : firstDisplayPointIndex;

		ProfilePoint& point = (grossMotion == true) ? (grossPoints.back()) : (texturePoints.back());

		return point;
	}
}

ProfilePoint & MotionGraph::getPoint()
{
	if (index < grossPoints.size())
	{
		ProfilePoint& point = (grossMotion == true) ? (grossPoints[index]) : (texturePoints[index]);
		
		index++;
		firstDisplayPointIndex = (index > firstDisplayPointIndex + numDisplayPoints - 1) ? (firstDisplayPointIndex + 1) : firstDisplayPointIndex;

		return point;
	}
}

std::vector<ProfilePoint>& MotionGraph::getPoints()
{
	return grossPoints;
}

int MotionGraph::getIndex()
{
	return index;
}

int MotionGraph::getSize()
{
	return grossPoints.size();
}

int MotionGraph::getNumDisplayPoints()
{
	return numDisplayPoints;
}

int MotionGraph::getFirstDisplayPoint()
{
	return firstDisplayPointIndex;
}

void MotionGraph::updateCurrentCurve(int ind)
{
	currentAxis = ind;
}

void MotionGraph::updateCurveVisibility(int ind, bool visible)
{
	bool& currFlag = (grossMotion == true) ? (displayGross[ind]) : (displayTexture[ind]);
	currFlag = visible;
}

void MotionGraph::updateCurveColor(int ind, float red, float green, float blue)
{
	ofColor& currColor = (grossMotion == true) ? (grossColors[ind]) : (textureColors[ind]);
	currColor.set(red, green, blue);
}

void MotionGraph::updateNumPoints(int number)
{
	if (firstDisplayPointIndex + number < getSize())
	{
		numDisplayPoints = number;

		//Adjust graph index if outside of display window
		index = ((index < firstDisplayPointIndex) || (index > firstDisplayPointIndex + numDisplayPoints - 1)) ? (firstDisplayPointIndex) : index;

		update();
	}
}

void MotionGraph::updateFirstDisplayPoint(int first)
{
	if (first + numDisplayPoints < getSize())
	{
		firstDisplayPointIndex = first;

		//Adjust graph index if outside of display window
		index = ((index < firstDisplayPointIndex) || (index > firstDisplayPointIndex + numDisplayPoints - 1)) ? (firstDisplayPointIndex) : index;

		update();
	}
}

void MotionGraph::updateGridSpacing(float gridX, float gridY)
{
	gridSpacingX = gridX;
	gridSpacingY = gridY;
}

void MotionGraph::updateIndex(int posInd)
{
	if (posInd < grossPoints.size())
	{
		index = posInd;
	}
	else
	{
		index = 0;
	}
}

void MotionGraph::updateToGross(bool modifyGross)
{
	grossMotion = modifyGross;
}

void MotionGraph::update()
{
	//fprintf(stderr, "%i\n", getSize());
	ofPoint tmpPoint;

	const float* RPHData;
	ofPoint RPHLimits;

	float scaledRPHData;

	//Ensure that display window is within range of actual data
	float lastDisplayPointIndex = (firstDisplayPointIndex + numDisplayPoints > getSize()) ? (getSize()):(firstDisplayPointIndex + numDisplayPoints);

	//Calculate distance between points
	pointSpacing = curveWidth / (lastDisplayPointIndex - firstDisplayPointIndex);

	//Update all curves
	for (int i = 0; i < numAxes; i++)
	{
		grossCurve[i].clear();
		grossCurve[i].begin();

		textureCurve[i].clear();
		textureCurve[i].begin();

		for (int j = firstDisplayPointIndex; j < lastDisplayPointIndex; j++)
		{
			//Get gross RPH data and corresponding limits
			RPHData = grossPoints[j].getRPH();
			RPHLimits = grossPoints[j].getRPHLimits(i);

			//Scale to height of graph working area
			scaledRPHData = getScaledValue(RPHData[i], RPHLimits.x, RPHLimits.y, 0.0f, curveHeight);
			
			//Adjust data index to get curve index and calculate curve coordinates
			tmpPoint.x = (j - firstDisplayPointIndex) * pointSpacing;
			tmpPoint.y = scaledRPHData;

			grossCurve[i].lineTo(tmpPoint.x, tmpPoint.y, 0);

			//Repeat for textures
			RPHData = texturePoints[j].getRPH();
			RPHLimits = texturePoints[j].getRPHLimits(i);

			scaledRPHData = getScaledValue(RPHData[i], RPHLimits.x, RPHLimits.y, 0.0f, curveHeight);

			tmpPoint.x = (j - firstDisplayPointIndex) * pointSpacing;
			tmpPoint.y = scaledRPHData;

			textureCurve[i].lineTo(tmpPoint.x, tmpPoint.y, 0);
		}
		textureCurve[i].end();

		grossCurve[i].end();
	}
}

void MotionGraph::draw()
{
	ofPushMatrix();
	ofRotateX(180.0f);	//Flip default window coordinate system upside down (default: TOP LEFT: (0,0), BOTTOM RIGHT: (X,Y) -> BOTTOM LEFT: (0,0), TOP RIGHT: (X,Y))
	ofTranslate(0, -height);	//Bring coordinate system back to visible window area; top left of graph is now at top left of window
	ofTranslate(start.x, -start.y);	//Translate to the starting point of graph; top left of graph is now at (sX,sY)

	drawBoundingBox();	

	ofPushMatrix();
	ofTranslate(frameMargin,frameMargin);	//Translate to the origin of the graph; this is the graph's (0,0)

	drawGrid();	//Draw all curves exactly as they are without any need to flip them
	drawCurves();	
	drawCursor();
	drawAxes();

	ofPopMatrix();	//Return to starting point of graph (sX,sY)
	ofPopMatrix();	//Return to default coordinates

	drawModBox();	//Draw mod box without translations since it uses mouse (default) coordinates
}

void MotionGraph::mousePressed(ofMouseEventArgs & mouse)
{
	//Get the underlying data vector of the current working curve
	std::vector<glm::vec3>& currCurve = (grossMotion == true)?(grossCurve[currentAxis].getVertices()):(textureCurve[currentAxis].getVertices());

	//Check if within graph working area
	if (mouse.x > origin.x && mouse.x < destination.x && mouse.y > destination.y && mouse.y < origin.y)
	{
		//Check if started drawing mod box
		if (modBoxSelected)
		{
			//Check if within mod box coordinates 
			if (mouse.x > modBoxOrigin.x && mouse.x < modBoxDestination.x && mouse.y > modBoxDestination.y && mouse.y < modBoxOrigin.y)
			{
				//Start modifying curve as mouse is dragged
				modifying = true;
			}
			else
			{
				//Deselect mod box and move cursor / index
				modBoxSelected = false;
				modifying = false;

				index = getScaledValue(mouse.x, origin.x, destination.x, firstDisplayPointIndex, firstDisplayPointIndex + numDisplayPoints);
			}
		}
		else
		{	
			//Move cursor 
			index = getScaledValue(mouse.x, origin.x, destination.x, firstDisplayPointIndex, firstDisplayPointIndex + numDisplayPoints);
		}
	}
}

void MotionGraph::mouseDragged(ofMouseEventArgs & mouse)
{
	//Get the underlying data vector of the current working curve
	std::vector<glm::vec3>& currCurve = (grossMotion == true) ? (grossCurve[currentAxis].getVertices()) : (textureCurve[currentAxis].getVertices());

	//Check if within graph working area
	if (mouse.x > origin.x && mouse.x < destination.x && mouse.y > destination.y && mouse.y < origin.y)
	{
		//Check if started drawing mod box
		if (modBoxSelected)
		{
			//Check if started modifying curve
			if (modifying)
			{
				//Check if within mod box coordinates
				if (mouse.x > modBoxOrigin.x && mouse.x < modBoxDestination.x && mouse.y > modBoxDestination.y && mouse.y < modBoxOrigin.y)
				{
					//Find mid index using start and end indices selected previously using mod box
					modIndexMid = getScaledValue(mouse.x, modBoxOrigin.x, modBoxDestination.x, modIndexStart, modIndexEnd);

					modPoint.y = getScaledValue(mouse.y, modBoxDestination.y, modBoxOrigin.y, 0.0f, curveHeight);
 
					//Generate bezier curve using mod points
					getQuadBezierCurve(modIndexStart, modIndexMid, modIndexEnd);
				}
			}
			else
			{
				//If mod box is being drawn but we are not modifying then get the end index / ending coordinates of mod box
				modBoxDestination.x = mouse.x;
				modBoxDestination.y = destination.y;

				modIndexEnd = getScaledValue(mouse.x, origin.x, destination.x, firstDisplayPointIndex, firstDisplayPointIndex + numDisplayPoints);
			}
		}
		else
		{
			//If not drawing mod box, get the start index / starting coordinates of mod box and start drawing it
			modBoxSelected = true;

			modBoxOrigin.x = mouse.x;
			modBoxOrigin.y = origin.y;

			modIndexStart = getScaledValue(mouse.x, origin.x, destination.x, firstDisplayPointIndex, firstDisplayPointIndex + numDisplayPoints);
		}
	}
}

void MotionGraph::mouseReleased(ofMouseEventArgs & mouse)
{
}

void MotionGraph::drawBoundingBox()
{
	ofSetColor(0);
	ofDrawRectangle(0, 0, width, height);
}

void MotionGraph::drawGrid()
{
	ofPoint tmpPoint;

	ofSetColor(255,85);
	ofFill();

	if (!grossPoints.empty())
	{
		//Determine how many horizontal grids to draw
		int numLevelsX = curveWidth / gridSpacingX;
		for (int i = 0; i < numLevelsX; i++)
		{
			tmpPoint.x = float(i * gridSpacingX);

			ofDrawLine(tmpPoint.x, 0.0f, tmpPoint.x, curveHeight);
		}

		//Determine how many vertical grids to draw
		float numLevelsY = curveHeight / gridSpacingY;
		for (int i = 0; i < numLevelsY; i++)
		{
			tmpPoint.y = float(i * gridSpacingY);

			ofDrawLine(0.0f, tmpPoint.y, curveWidth, tmpPoint.y);
		}
	}
}

void MotionGraph::drawAxes()
{
	ofSetColor(255);
	ofFill();
	ofSetLineWidth(1.0f);

	ofDrawLine(0.0f, 0.0f, curveWidth, 0.0f);
	ofDrawLine(0.0f, 0.0f, 0.0f, curveHeight);
}

void MotionGraph::drawCursor()
{
	ofSetColor(255,255,50);
	ofSetLineWidth(5.0f);

	ofFill();

	ofDrawLine((index - firstDisplayPointIndex) * pointSpacing, 0.0f, (index - firstDisplayPointIndex) * pointSpacing, curveHeight);
}

void MotionGraph::drawCurves()
{
	for (int i = 0; i < numAxes; i++)
	{
		if (displayGross[i])
		{
			ofSetColor(grossColors[i]);
			grossCurve[i].draw();
		}
		if (displayTexture[i])
		{
			ofSetColor(textureColors[i]);
			textureCurve[i].draw();
		}
	}
}

void MotionGraph::drawModBox()
{
	if (modBoxSelected)
	{
		ofSetColor(255, 255, 0, 50);
		ofFill();
		ofSetLineWidth(1.0f);

		ofDrawRectangle(modBoxOrigin.x,modBoxOrigin.y, modBoxDestination.x - modBoxOrigin.x, modBoxDestination.y - modBoxOrigin.y);
	}
}

void MotionGraph::copy(int startIndex, int endIndex)
{
	buffer.clear();
	std::copy(grossPoints.begin() + startIndex, grossPoints.end() + endIndex, buffer.begin());
}

void MotionGraph::paste(int startIndex)
{
	std::copy(buffer.begin(), buffer.end(),grossPoints.begin()+startIndex);
}

void MotionGraph::getQuadBezierCurve(int startIndex, int midIndex, int endIndex)
{
	//Get the underlying data vector of the current working curve as well as the data of the current axis
	std::vector<glm::vec3>& currCurve = (grossMotion == true) ? (grossCurve[currentAxis].getVertices()) : (textureCurve[currentAxis].getVertices());
	std::vector<ProfilePoint>& currData = (grossMotion == true) ? (grossPoints) : (texturePoints);

	//Adjust data indices to get curve indices and define bezier points
	ofPoint P0 = ofPoint(startIndex - firstDisplayPointIndex, currCurve[startIndex - firstDisplayPointIndex].y);
	ofPoint P1 = ofPoint(midIndex - firstDisplayPointIndex, modPoint.y);
	ofPoint P2 = ofPoint(endIndex - firstDisplayPointIndex, currCurve[endIndex - firstDisplayPointIndex].y);

	float t = 0.0f;	//Bezier parameter

	float currRPHData;	//RPH data of working axis

	//Gross and texture RPH data for current axis
	float grossRPHData;	
	float textureRPHData;	

	ofPoint currRPHDataLimits;	//RPH data limits for working axis

	//Gross and texture RPH data limits for current axis
	ofPoint grossRPHDataLimits;
	ofPoint textureRPHDataLimits;

	for (int i = startIndex; i < endIndex; i++)
	{
		t = getScaledValue(i, startIndex, endIndex, 0.0f, 1.0f);	//Find current bezier parameter

		//Current curve and data only (could be gross or texture); note that the index is adjusted to be able to access curve data
		currCurve[i - firstDisplayPointIndex].y = std::pow(1.0f - t, 2.0f)*P0.y + 2.0f*(1.0f - t)*t*P1.y + std::pow(t, 2.0f)*P2.y;
		
		currRPHDataLimits = currData[i].getRPHLimits(currentAxis);

		currRPHData = getScaledValue(currCurve[i - firstDisplayPointIndex].y, 0.0f, curveHeight, currRPHDataLimits.x, currRPHDataLimits.y);

		currData[i].updateData(currentAxis, currRPHData);

		//Recalculate gross curve and data with texture added
		float grossCurveY = grossCurve[currentAxis].getVertices()[i - firstDisplayPointIndex].y;
		float textureCurveY = textureCurve[currentAxis].getVertices()[i - firstDisplayPointIndex].y;

		grossRPHDataLimits = grossPoints[i].getRPHLimits(currentAxis);
		textureRPHDataLimits = texturePoints[i].getRPHLimits(currentAxis);

		grossRPHData = getScaledValue(grossCurveY, 0.0f, curveHeight, grossRPHDataLimits.x, grossRPHDataLimits.y);	//Calculate gross RPH data from gross curve data; this allows for addition and subtraction of texture data from gross data
		textureRPHData = getScaledValue(textureCurveY, 0.0f, curveHeight, textureRPHDataLimits.x, textureRPHDataLimits.y);	////Calculate texture RPH data from texture curve data; this allows for addition and subtraction of texture data from gross data

		grossRPHData += textureRPHData;	//Update gross RPH data with texture RPH data added
		grossRPHData = std::max(grossRPHDataLimits.x, std::min(grossRPHData, grossRPHDataLimits.y));	//Clip gross RPH data if out of range

		grossPoints[i].updateData(currentAxis, grossRPHData);	//Store updated gross RPH data

		grossCurveY = getScaledValue(grossRPHData, grossRPHDataLimits.x, grossRPHDataLimits.y, 0.0f, curveHeight);	//Update gross curve data based on gross RPH data
		grossCurve[currentAxis].getVertices()[i - firstDisplayPointIndex].y = grossCurveY;	//Store updated gross curve data
	}
}

void MotionGraph::getCosineCurve(int startIndex, int endIndex, float frequency, float amplitude)
{
	//TODO
}

void MotionGraph::getSineCurve(int startIndex, int endIndex, float frequency, float amplitude)
{
	//TODO
}

