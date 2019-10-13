#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "MotionProfile.h"
#include "MotionControl.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(ofMouseEventArgs& args);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void readProfile();
		void writeProfile();

		void recordProfile();
		void playProfile();
		void stopProfile();

		void displayXGross(bool& toggled);
		void displayYGross(bool& toggled);
		void displayZGross(bool& toggled);

		void displayXTexture(bool& toggled);
		void displayYTexture(bool& toggled);
		void displayZTexture(bool& toggled);

		void modifyXGross(bool& toggled);
		void modifyYGross(bool& toggled);
		void modifyZGross(bool& toggled);

		void modifyXTexture(bool& toggled);
		void modifyYTexture(bool& toggled);
		void modifyZTexture(bool& toggled);

		void updateNumPoints(int& value);
		void updateTimeline(int& value);

		void updateInputFile(std::string& value);
		void updateOutputFile(std::string& value);

		MotionControl controller;
		MotionModel model;
		MotionProfile* profile;
		MotionGraph* graph;
		
		float btnWidth;
		float btnHeight;

		float sldrWidth;
		float sldrHeight;

		float grphX;
		float grphY;

		float grphWidth;
		float grphHeight;

		float hMargin;
		float vMargin;

		ofxButton btnRead;
		ofxButton btnWrite;

		ofxTextField txtRead;
		ofxTextField txtWrite;

		ofxToggle dispXGross;
		ofxToggle dispYGross;
		ofxToggle dispZGross;

		ofxToggle dispXTexture;
		ofxToggle dispYTexture;
		ofxToggle dispZTexture;

		ofxToggle modXGross;
		ofxToggle modYGross;
		ofxToggle modZGross;

		ofxToggle modXTexture;
		ofxToggle modYTexture;
		ofxToggle modZTexture;

		ofxButton btnRecord;
		ofxButton btnPlay;
		ofxButton btnStop;

		ofxIntSlider sldrNumPoints;
		ofxIntSlider sldrTimeline;

		bool recording = false;
		bool playing = false;

		std::string inputFileName;
		std::string outputFileName;
};
