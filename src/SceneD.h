#pragma once

#include "ofxState.h"
#include "SharedData.h"

class SceneD : public itg::ofxState<SharedData> {
public:
	void setup();
	void update();
	void draw();
	string getName();

private:
	ofVboMesh sphere;
	vector<ofVec3f> points;
	vector<ofVboMesh> aroundSpheres;
	ofEasyCam cam;
	ofShader sphereShader;
	ofVec3f lightDir;
	float time;
	ofFbo renderFbo;
};