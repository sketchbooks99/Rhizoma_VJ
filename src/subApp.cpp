#include "subApp.h"

void subApp::setup() {
	fbo.allocate(1280, 720);
}

void subApp::draw() {
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
}

void subApp::keyPressed(int key) {
}