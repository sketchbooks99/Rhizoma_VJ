#pragma once

#include "ofxState.h"
#include "SharedData.h"

class SceneC : public itg::ofxState<SharedData> {
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
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

	// General parameters
	float time;
	int sceneIdx;
	myCamera cam;

	// Geometry
	ofVboMesh box;
};