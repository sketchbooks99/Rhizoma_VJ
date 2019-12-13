#pragma once

#include "ofxState.h"
#include "SharedData.h"
#include "ofxGui.h"

class SceneB : public itg::ofxState<SharedData> {
public:
	void setup();
	void update();
	void draw();
	ofVboMesh createPiramid(float scale);
	string getName();

	struct Boids {
		ofVec3f pos;
		ofVec3f vel;
	};

private:
	myCamera cam;
	ofVec3f lightDir;
	float time;
	ofFbo renderFbo;

	// boid
	ofVboMesh piramid, box;
	ofShader integrate, forceCompute, instancingShader;
	ofTexture posTex, velTex, forceTex;
	ofBufferObject forceBuffer, dataBuffer;
	vector<Boids> boids;
	int numFish, texRes;

	// Gui & Paramters
	ofxPanel gui;
	ofParameter<float> separateRadius, alignmentRadius, cohesionRadius;
	ofParameter<float> separateWeight, alignmentWeight, cohesionWeight;
	ofParameter<float> maxSpeed, maxForce, avoidWallWeight;
	ofParameter<float> fps;
	ofParameterGroup shaderUniforms;
};