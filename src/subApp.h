#pragma once

#include "ofMain.h"
//#include "ofApp.h"
#include "ofxGui.h"

class subApp : public ofBaseApp {
public:
	void setup();
	void draw();
	void keyPressed(int key);
	ofVec2f getWindowSize() { return ofVec2f(ofGetWidth(), ofGetHeight()); }
	ofxPanel gui;

	ofFbo fbo;
};