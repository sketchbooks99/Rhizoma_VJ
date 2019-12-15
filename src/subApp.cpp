#include "subApp.h"

void subApp::setup() {
	
	fbo.allocate(ofGetWidth(), ofGetHeight());
	//ofSetWindowPosition(ofGetScreenWidth()+1, 0);
	//ofToggleFullscreen();
	//ofSetFullscreen(true);
}

void subApp::draw() {
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
}

void subApp::keyPressed(int key) {
	if (key == 'l') {
		cout << ofGetWindowPositionX() << endl;
		cout << ofGetWindowPositionY() << endl;
		ofToggleFullscreen();
	}
}