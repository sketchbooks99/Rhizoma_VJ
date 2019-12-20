#pragma once

#include "ofxState.h"
#include "SharedData.h"
#include "Trail.hpp"
#include "ofxBvh.h"

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
	void scene4();

private:
	ofVboMesh boxel, plane, box, sky;
	ofFbo renderFbo;
	ofShader boxelShader;

	int sceneMode;
	float time;

	int sceneIndex[4];

	myCamera cam;
	vector<ofVec3f> camRadiuses;
	vector<ofVec3f> timeOffsets;
	int camIdx = 0;

	ofxPanel gui;
	ofxToggle wireFrame;

	// trail setting
	Trail trail;
	int trailLength, numTrail; 
	ofVboMesh trailSphere;
	float speed, size, trailWidth;
	
	ofxBvh bvh;
	ofVboMesh manSphere;
};