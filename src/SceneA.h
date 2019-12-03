#pragma once

#include "ofxState.h"
#include "SharedData.h"

class SceneA : public itg::ofxState<SharedData> {
	void setup();
	void update();
	void draw();
	string getName();

	int plWidth, plHeight;
	ofVboMesh vboMesh, planeMesh;

	ofEasyCam cam;
	int radius;
	float time;

	vector<ofVec3f> points;
	vector<ofVec3f> speeds; 
	vector<ofVec4f> spawnP;
	vector<float> spawnVelZ;

	ofVec3f translate;
};