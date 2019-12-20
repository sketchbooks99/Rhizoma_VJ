#pragma once

#include "ofxState.h"
#include "SharedData.h"
#include "Trail.hpp"

class SceneD : public itg::ofxState<SharedData> {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	string getName();

	void scene1();
	void scene2();
	void scene3();

private:
	ofVboMesh boxel, plane, box;
	ofFbo renderFbo;
	ofShader boxelShader;

	int sceneMode;
	float time;

	myCamera cam;

	ofxPanel gui;
	ofxToggle wireFrame;

	// trail setting
	Trail trail;
	int trailLength, numTrail; 
	ofVboMesh trailSphere;
	float speed, size, trailWidth;
};