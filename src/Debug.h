#pragma once

#include "ofxState.h"
#include "SharedData.h"
#include "ofxGui.h"

using namespace glm;

class Debug : public itg::ofxState<SharedData> {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void scene1(); // Boids
	void scene2(); // curlNoise
	void scene3(); // Boids + GPU particles
	void scene4(); // GPU trail
	void drawPrimitives();
    void createGBuffer();
	ofVboMesh createPiramid(float scale);
	string getName();

private:
    ofFbo renderFbo, gFbo, depthFbo;
	ofShader depthShader, depthRender;

    ofVboMesh plane, box, quad;
    ofEasyCam cam;

	// Gui & Paramters
	ofxPanel gui;
	ofParameter<int> debugMode;
	ofParameterGroup shaderUniforms;
	mat4 lightProjection, lightView, lightSpaceMatrix;
};