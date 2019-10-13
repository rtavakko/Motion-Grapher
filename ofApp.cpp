#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	btnWidth = 120.0f;
	btnHeight = 50.f;

	sldrWidth = 60.0f;
	sldrHeight = 25.0f;

	grphX = 800.0f;
	grphY = 100.0f;

	grphWidth = 1000.0f;
	grphHeight = 800.0f;

	hMargin = 10.0f;
	vMargin = 10.0f;

	inputFileName = "Agila.dat";
	outputFileName = "Output.dat";

	btnRead.setup("Read", btnWidth, btnHeight);
	btnWrite.setup("Write", btnWidth, btnHeight);

	btnRead.setPosition(hMargin, vMargin);
	btnWrite.setPosition(hMargin, 2.0f*vMargin + btnHeight);

	btnRead.addListener(this, &ofApp::readProfile);
	btnWrite.addListener(this, &ofApp::writeProfile);

	txtRead.setup("", inputFileName, "", "", 2.0f*btnWidth, btnHeight);
	txtWrite.setup("", outputFileName, "", "", 2.0f*btnWidth, btnHeight);

	txtRead.setPosition(2.0f*hMargin + btnWidth, vMargin);
	txtWrite.setPosition(2.0f*hMargin + btnWidth, 2.0f*vMargin + btnHeight);

	dispXGross.setup("S-XG", true, btnWidth, btnHeight);
	dispYGross.setup("S-YG", true, btnWidth, btnHeight);
	dispZGross.setup("S-ZG", true, btnWidth, btnHeight);

	dispXTexture.setup("S-XT", true, btnWidth, btnHeight);
	dispYTexture.setup("S-YT", true, btnWidth, btnHeight);
	dispZTexture.setup("S-ZT", true, btnWidth, btnHeight);

	dispXGross.setPosition(hMargin, 5.0f*vMargin + 4.0f*btnHeight);
	dispYGross.setPosition(hMargin, 6.0f*vMargin + 5.0f*btnHeight);
	dispZGross.setPosition(hMargin, 7.0f*vMargin + 6.0f*btnHeight);

	dispXTexture.setPosition(btnWidth + 2.0f*hMargin, 5.0f*vMargin + 4.0f*btnHeight);
	dispYTexture.setPosition(btnWidth + 2.0f*hMargin, 6.0f*vMargin + 5.0f*btnHeight);
	dispZTexture.setPosition(btnWidth + 2.0f*hMargin, 7.0f*vMargin + 6.0f*btnHeight);

	dispXGross.addListener(this, &ofApp::displayXGross);
	dispYGross.addListener(this, &ofApp::displayYGross);
	dispZGross.addListener(this, &ofApp::displayZGross);

	dispXTexture.addListener(this, &ofApp::displayXTexture);
	dispYTexture.addListener(this, &ofApp::displayYTexture);
	dispZTexture.addListener(this, &ofApp::displayZTexture);

	modXGross.setup("M-XG", true, btnWidth, btnHeight);
	modYGross.setup("M-YG", false, btnWidth, btnHeight);
	modZGross.setup("M-ZG", false, btnWidth, btnHeight);

	modXTexture.setup("M-XT", false, btnWidth, btnHeight);
	modYTexture.setup("M-YT", false, btnWidth, btnHeight);
	modZTexture.setup("M-ZT", false, btnWidth, btnHeight);

	modXGross.setPosition(3.0f*btnWidth + 4.0f*hMargin, 5.0f*vMargin + 4.0f*btnHeight);
	modYGross.setPosition(3.0f*btnWidth + 4.0f*hMargin, 6.0f*vMargin + 5.0f*btnHeight);
	modZGross.setPosition(3.0f*btnWidth + 4.0f*hMargin, 7.0f*vMargin + 6.0f*btnHeight);

	modXTexture.setPosition(4.0f*btnWidth + 5.0f*hMargin, 5.0f*vMargin + 4.0f*btnHeight);
	modYTexture.setPosition(4.0f*btnWidth + 5.0f*hMargin, 6.0f*vMargin + 5.0f*btnHeight);
	modZTexture.setPosition(4.0f*btnWidth + 5.0f*hMargin, 7.0f*vMargin + 6.0f*btnHeight);

	modXGross.addListener(this, &ofApp::modifyXGross);
	modYGross.addListener(this, &ofApp::modifyYGross);
	modZGross.addListener(this, &ofApp::modifyZGross);

	modXTexture.addListener(this, &ofApp::modifyXTexture);
	modYTexture.addListener(this, &ofApp::modifyYTexture);
	modZTexture.addListener(this, &ofApp::modifyZTexture);

	btnRecord.setup("Record", btnWidth, btnHeight);
	btnPlay.setup("Play", btnWidth, btnHeight);
	btnStop.setup("Stop", btnWidth, btnHeight);

	btnRecord.setPosition(hMargin, 12.0f*vMargin + 11.0f*btnHeight);
	btnPlay.setPosition(hMargin, 13.0f*vMargin + 12.0f*btnHeight);
	btnStop.setPosition(hMargin, 14.0f*vMargin + 13.0f*btnHeight);

	btnRecord.addListener(this, &ofApp::recordProfile);
	btnPlay.addListener(this, &ofApp::playProfile);
	btnStop.addListener(this, &ofApp::stopProfile);

	sldrNumPoints.setup("N", 0, 0, 100, grphWidth, sldrHeight);
	sldrTimeline.setup("", 0, 0, 100, grphWidth, sldrHeight);

	sldrNumPoints.setPosition(grphX, grphY + grphHeight + sldrHeight + 2.0f*vMargin);
	sldrTimeline.setPosition(grphX, grphY + grphHeight + vMargin);

	sldrNumPoints.addListener(this, &ofApp::updateNumPoints);
	sldrTimeline.addListener(this, &ofApp::updateTimeline);

	txtRead.addListener(this, &ofApp::updateInputFile);
	txtWrite.addListener(this, &ofApp::updateOutputFile);

	graph = new MotionGraph(grphX, grphY, grphWidth, grphHeight, 3);
	graph->updateGridSpacing(grphWidth / 20.0f, grphHeight / 20.0f);

	graph->updateToGross(false);

	graph->updateCurveColor(0, 255, 0, 255);
	graph->updateCurveColor(1, 255, 255, 0);
	graph->updateCurveColor(2, 0, 255, 255);

	graph->updateCurveVisibility(0, true);
	graph->updateCurveVisibility(1, true);
	graph->updateCurveVisibility(2, true);

	graph->updateToGross(true);

	graph->updateCurveColor(0, 255, 0, 0);
	graph->updateCurveColor(1, 0, 255, 0);
	graph->updateCurveColor(2, 0, 0, 255);

	graph->updateCurveVisibility(0, true);
	graph->updateCurveVisibility(1, true);
	graph->updateCurveVisibility(2, true);

	profile = new MotionProfile("Agila.dat", sixDV_Festo);
	profile->connectGraph(graph);

	controller.setMaxModelRotations(22.5f, 22.5f, 22.5f);
	controller.connectModel(&model);
	controller.connectProfile(profile);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	//model.draw();

	btnRead.draw();
	btnWrite.draw();

	txtRead.draw();
	txtWrite.draw();

	dispXGross.draw();
	dispYGross.draw();
	dispZGross.draw();

	dispXTexture.draw();
	dispYTexture.draw();
	dispZTexture.draw();

	modXGross.draw();
	modYGross.draw();
	modZGross.draw();

	modXTexture.draw();
	modYTexture.draw();
	modZTexture.draw();

	btnRecord.draw();
	btnPlay.draw();
	btnStop.draw();

	sldrNumPoints.draw();
	sldrTimeline.draw();

	graph->draw();

	if (recording)
	{
		controller.record();
	}
	if (playing)
	{
		controller.play();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs& argsn){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

void ofApp::readProfile()
{
	profile->updateFileName(inputFileName);
	profile->read();
}

void ofApp::writeProfile()
{
	profile->write(outputFileName);
}

void ofApp::recordProfile()
{
	recording = true;
}

void ofApp::playProfile()
{
	playing = true;
}

void ofApp::stopProfile()
{
	recording = false;
	playing = false;
}

void ofApp::displayXGross(bool& toggled)
{
	graph->updateToGross(true);
	graph->updateCurveVisibility(0, toggled);
}

void ofApp::displayYGross(bool& toggled)
{
	graph->updateToGross(true);
	graph->updateCurveVisibility(1, toggled);
}

void ofApp::displayZGross(bool& toggled)
{
	graph->updateToGross(true);
	graph->updateCurveVisibility(2, toggled);
}

void ofApp::displayXTexture(bool& toggled)
{
	graph->updateToGross(false);
	graph->updateCurveVisibility(0, toggled);
}

void ofApp::displayYTexture(bool& toggled)
{
	graph->updateToGross(false);
	graph->updateCurveVisibility(1, toggled);
}

void ofApp::displayZTexture(bool& toggled)
{
	graph->updateToGross(false);
	graph->updateCurveVisibility(2, toggled);
}

void ofApp::modifyXGross(bool & toggled)
{
	if (toggled)
	{
		graph->updateToGross(true);
		graph->updateCurrentCurve(0);

		modYGross = false;
		modZGross = false;

		modXTexture = false;
		modYTexture = false;
		modZTexture = false;
	}
}

void ofApp::modifyYGross(bool & toggled)
{
	if (toggled)
	{
		graph->updateToGross(true);
		graph->updateCurrentCurve(1);

		modXGross = false;
		modZGross = false;

		modXTexture = false;
		modYTexture = false;
		modZTexture = false;
	}
}

void ofApp::modifyZGross(bool & toggled)
{
	if (toggled)
	{
		graph->updateToGross(true);
		graph->updateCurrentCurve(2);

		modXGross = false;
		modYGross = false;

		modXTexture = false;
		modYTexture = false;
		modZTexture = false;
	}
}

void ofApp::modifyXTexture(bool & toggled)
{
	if (toggled)
	{
		graph->updateToGross(false);
		graph->updateCurrentCurve(0);

		modXGross = false;
		modYGross = false;
		modZGross = false;

		modYTexture = false;
		modZTexture = false;
	}
}

void ofApp::modifyYTexture(bool & toggled)
{
	if (toggled)
	{
		graph->updateToGross(false);
		graph->updateCurrentCurve(1);

		modXGross = false;
		modYGross = false;
		modZGross = false;

		modXTexture = false;
		modZTexture = false;
	}
}

void ofApp::modifyZTexture(bool & toggled)
{
	if (toggled)
	{
		graph->updateToGross(false);
		graph->updateCurrentCurve(2);

		modXGross = false;
		modYGross = false;
		modZGross = false;

		modXTexture = false;
		modYTexture = false;
	}
}

void ofApp::updateNumPoints(int& value)
{
	int maxPoints = graph->getSize();

	int num = getScaledValue(value,0,100,0,maxPoints);

	graph->updateNumPoints(num);
}

void ofApp::updateTimeline(int& value)
{
	int index = getScaledValue(value, 0, 100, 0, graph->getSize());
	graph->updateFirstDisplayPoint(index);
}

void ofApp::updateInputFile(std::string & value)
{
	inputFileName = value;
}

void ofApp::updateOutputFile(std::string & value)
{
	outputFileName = value;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
}
