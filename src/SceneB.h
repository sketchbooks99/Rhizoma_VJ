#pragma once

#include "ofxState.h"
#include "SharedData.h"
#include "ofxGui.h"

class SceneB : public itg::ofxState<SharedData> {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void scene1(); // Boids
	void scene2(); // curlNoise
	void scene3(); // Boids + GPU particles
	void scene4(); // GPU trail
	ofVboMesh createPiramid(float scale);
	string getName();

	struct Boids {
		ofVec3f pos;
		ofVec3f vel;
	};

	struct Lifetime {
		float age;
		float maxAge;
	};

private:
	// Camera
	myCamera cam;
	vector<ofVec3f> camRadiuses;
	vector<ofVec3f> timeOffsets;
	int camIdx = 0;
	
	// General Settings
	ofVec3f lightDir;
	float time;
	ofFbo renderFbo;
	int sceneMode = 0;

	// boid
	ofVboMesh piramid, box;
	ofShader integrate, forceCompute, instancingShader, curlCompute;
	ofTexture posTex, velTex, forceTex;
	ofBufferObject forceBuffer, dataBuffer, lifeBuffer;
	vector<Boids> boids;
	int numFish, texRes;
	vector<ofVec3f> attractorPoses;
	unsigned int attIdx;
	ofVec3f wallSize;
	ofSpherePrimitive attractor;

	// Gui & Paramters
	ofxPanel gui;
	ofParameter<float> separateRadius, alignmentRadius, cohesionRadius;
	ofParameter<float> separateWeight, alignmentWeight, cohesionWeight;
	ofParameter<float> maxSpeed, maxForce, avoidWallWeight;
	ofParameter<float> attractWeight;
	ofParameter<float> fps;
	ofParameterGroup shaderUniforms;
};