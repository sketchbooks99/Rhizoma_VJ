#pragma once

#include "ofxState.h"
#include "SharedData.h"

class SceneC : public itg::ofxState<SharedData> {
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	string getName();

	void scene1();
	void scene2();
	void scene3();
	void scene4();
	void scene5();

	// Custom Functions
	void createGBuffer();
	void createRenderBuffer();

	struct Particle {
		ofVec3f pos;
		ofVec3f vel;
	};

	// Shaders & fbo
	ofShader rayShader, postShader, lightingShader, gBufferShader, gRenderShader;
	//ofShader ray2, ray3, ray4;
	ofShader ray[5];
	ofFbo renderFbo, gFbo, lightingFbo;
	ofVboMesh quad;
	ofVec3f lightPos;

	vector<ofVec3f> camRadiuses;
	vector<ofVec3f> timeOffsets;

	ofxPanel gui;
	ofxToggle isNormal, isDepth, isPosition, isRender, isColored, isShade;
	ofxToggle showTex;

	bool sceneArray[5][5];

	// GPU Particles
	int numParticles, textureRes;
	ofShader compute, renderParticle;
	ofBufferObject particleBuffer;
	ofVboMesh particle;

	// General parameters
	float time;
	int sceneMode;
	myCamera cam;

	// Geometry
	ofVboMesh box;
};