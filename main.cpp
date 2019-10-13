#include "ofMain.h"
#include "ofApp.h"
#include "sideApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1920,1080,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

	/*
	ofGLWindowSettings settings;

	settings.setSize(640, 360);
	settings.setPosition(ofVec2f(200.0f, 200.0f));
	auto mainWindow = ofCreateWindow(settings);

	settings.setPosition(ofVec2f(300.0f, 300.0f));
	auto sideWindow = ofCreateWindow(settings);

	auto mApp = make_shared<ofApp>();
	auto sApp = make_shared<sideApp>();

	ofRunApp(mainWindow, mApp);
	ofRunApp(sideWindow, sApp);

	*/

	//ofRunMainLoop();
}
