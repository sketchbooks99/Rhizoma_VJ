#pragma once

#include "ofMain.h"
//#include "ofApp.h"
#include "ofxGui.h"

class subApp : public ofBaseApp {
public:
	void setup();
	void draw();
	void keyPressed(int key);
	ofxPanel gui;
	ofxToggle isBloom, isEdge, isDof, isGodray, isNoiseWarp, isPixelate, isRGBShift, isRimHighlight, isZoomBlur, isInvert;

	ofFbo fbo;
	float fps;
	//shared_ptr<ofApp> mainApp;
};