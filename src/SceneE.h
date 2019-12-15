#pragma once

#include "ofxState.h"
#include "SharedData.h"

class SceneC : public itg::ofxState<SharedData> {
	void setup();
	void update();
	void draw();
	string getName();

	// Custom Functions
	void createGBuffer();
	void createRenderBuffer();

	struct Particle {
		ofVec3f pos;
		ofVec3f vel;
	};

	// Shaders & fbo
	ofShader rayShader, postShader, lightingShader, gBufferShader, gRenderShader;
	ofFbo renderFbo, gFbo;
	ofVboMesh quad;

	ofxPanel gui;
	ofxToggle isNormal, isDepth, isPosition, isRender;
	ofxToggle showTex;

	// GPU Particles
	int numParticles, textureRes;
	ofShader compute, renderParticle;
	ofBufferObject particleBuffer;
	ofVboMesh particle;

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