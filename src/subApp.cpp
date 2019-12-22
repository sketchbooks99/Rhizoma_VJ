#include "subApp.h"

void subApp::setup() {
	
	fbo.allocate(ofGetWidth(), ofGetHeight());
	//ofSetWindowPosition(ofGetScreenWidth()+1, 0);
	//ofToggleFullscreen();
	/*ofSetWindowPosition(ofGetScreenWidth(), 0);
	ofSetFullscreen(true);*/
}

void subApp::draw() {
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());

	cout << ofGetWindowPositionX() << endl;
	cout << ofGetWindowPositionY() << endl;
}

void subApp::keyPressed(int key) {
	if (key == 'l') {
		cout << ofGetWindowPositionX() << endl;
		cout << ofGetWindowPositionY() << endl;
		ofToggleFullscreen();
	}
}