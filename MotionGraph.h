#pragma once

#include "ofMain.h"
#include "ProfilePoint.h"

class MotionGraph
{
public:
	MotionGraph(float sX, float sY, float gWidth, float gHeight, int gAxes);
	~MotionGraph();

	ProfilePoint& insertPoint();	//Insert new point if graph index is past data capacity
	ProfilePoint& getPoint();		//Return existing point if graph index is within data capacity

	std::vector<ProfilePoint>& getPoints();	//Pointer to data container

	int getIndex();	//Return current index of graph 
	int getSize();	//Return current data size
	int getNumDisplayPoints();	//Return the number of points displayed on the graph
	int getFirstDisplayPoint();	//Return the index of the first point displayed on the graph

	void updateCurrentCurve(int ind);	//Set current working axis Roll:0,Pitch:1,Heave:2
	void updateCurveVisibility(int ind, bool visible);	//Set visibility of current working axis
	void updateCurveColor(int ind, float red, float green, float blue);	//Set color of current working axis
	void updateNumPoints(int number);	//Set number of points displayed on the graph
	void updateFirstDisplayPoint(int first);	//Set the index of the first point displayed on the graph
	void updateGridSpacing(float gridX, float gridY); //Set the horizontal and vertical grid distances

	void updateIndex(int posInd);	//Set the current index of the graph

	void updateToGross(bool modifyGross);	//Set working axis to either gross motion or texture

	void update();	//Update the curves drawn by the graph; only the subset of points specified by firstDisplayPointIndex and numDisplayPoints are drawn
	void draw();	//Draw graph, grid, cursor, etc.

	//Mouse events to allow data manipulation
	void mousePressed(ofMouseEventArgs & mouse);	
	void mouseDragged(ofMouseEventArgs & mouse);
	void mouseReleased(ofMouseEventArgs & mouse);

	ofEvent<std::vector<ProfilePoint>&> pointsUpdated;	//Event to notify any profile or object the graph is connected to 

private:
	void drawBoundingBox();	//The area containing the graph background, axes, grid, curves etc.
	void drawGrid();	//Draw graph grid
	void drawAxes();	//Draw graph axes
	void drawCursor();	//Draw graph cursor (based on index)
	void drawCurves();	//Draw all curves
	void drawModBox();	//Draw modification rectangle for data manipulation

	void copy(int startIndex, int endIndex);	//Copy range of data points into buffer 
	void paste(int startIndex);	//Paste buffer data into data 

	void getQuadBezierCurve(int startIndex, int midIndex, int endIndex);	//Populate current working data and curve with a quadratic bezier curve using modPoint.y
	void getCosineCurve(int startIndex, int endIndex, float frequency, float amplitude);	//Populate current working data and curve with cosine curve
	void getSineCurve(int startIndex, int endIndex, float frequency, float amplitude);	//Populate current working data and curve with sine curve

private:
	int index;	//Current graph index

	int currentAxis;	//Current working axis

	int numAxes;	//Total number of axes

	int numDisplayPoints;	//Total number of data points displayed on the graph
	int firstDisplayPointIndex;	//Index of first point displayed on the graph

	float pointSpacing;	//Screen distance between displayed points equal to the total width of the graph divided by the number of points

	float gridSpacingX;	//Screen width of the horizontal grid
	float gridSpacingY;	//Screen height of the vertical grid

	ofPoint start;	//Overall graph starting point

	float width;	//Overall width of graph 
	float height;	//Overall height of graph

	float frameMargin;	//Screen distance between the axes and the edges of the graph

	ofPoint origin;	//Origin of working area of graph
	ofPoint destination;	//End of working area of graph
	
	float curveWidth;	//Width of graph working area
	float curveHeight;	//Height of graph working area

	bool modBoxSelected;	//Flag set when an area is selected for modification
	bool modifying;	//Flag set when modifying points in modification rectangle

	ofPoint modBoxOrigin;	//Origin point of the modification rectangle
	ofPoint modBoxDestination;	//End point of the modification rectangle

	ofPoint modPoint;	//Bezier mid point selected in the modification rectangle used to generate bezier curves

	int modIndexStart;	//Index of start point in the data range being modified
	int modIndexEnd;	//Index of end point in the data range being modified
	int modIndexMid;	//Index of mid point in the data range being modified

	bool grossMotion;	//Flag set when gross data and curve are being modified

	bool* displayGross;	//Flags for visibility of the gross curve for each axis
	ofPolyline* grossCurve;	//Gross curves for each axis
	ofColor* grossColors;	//Color of the gross curve for each axis

	bool* displayTexture;	//Flags for visibility of the texture curve for each axis
	ofPolyline* textureCurve;	//Texture curves for each axis
	ofColor* textureColors;	//Color of the texture curve for each axis

	std::vector<ProfilePoint> grossPoints;	//Gross data
	std::vector<ProfilePoint> texturePoints;	//Texture data

	std::vector<ProfilePoint> buffer;	//Buffer to store temporary data
};

