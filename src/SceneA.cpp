#include "SceneA.h"

// =========================================================================================
void SceneA::setup() {
	ofFbo::Settings rs; // render settings
	rs.width = ofGetWidth();
	rs.height = ofGetHeight();
	rs.useDepth = true;
	rs.useStencil = true;
	rs.depthStencilAsTexture = true;
	rs.textureTarget = GL_TEXTURE_2D;
	//rs.numSamples = 16;
	renderFbo.allocate(rs);
	normalFbo.allocate(rs);
	occludeFbo.allocate(rs);
	volumetricFbo.allocate(rs);

	for (unsigned int i = 0; i < 100; i++) {
		unsigned int res = int(ofRandom(3, 20));
		unsigned int size = int(ofRandom(100, 200));
		ofVec3f pos = ofVec3f(ofRandom(-ofGetWidth(), ofGetWidth()) * 1.5, ofRandom(-ofGetWidth(), ofGetWidth()) * 1.5, ofRandom(-30, 30));
		//bool isBright = i % 10 == 0 ? true : false;
		bool isBright = false;

		ofColor color = ofColor(200, 255, 255, 64);
		Circle circle = Circle(res, size, pos, color, isBright);
		circles.push_back(circle);
	}
	ofSetLineWidth(20);

	renderShader.load("shader/SceneA/render.vert", "shader/SceneA/render.frag");
	volumetricShader.load("shader/SceneA/volumetric.vert", "shader/SceneA/volumetric.frag");

	gui.setup();
	gui.setPosition(getSharedData().gui.getWidth() + 10, 10);
	gui.add(density.set("Density", 0.07, 0.0, 1.0));
	gui.add(weight.set("Weight", 1.0, 0.0, 1.0));
	gui.add(decay.set("decay", 0.97, 0.0, 1.0));
	gui.add(exposure.set("exposure", 1.0, 0.0, 1.0));
	gui.add(screenY.set("ScreenY", -10.0, -10.0, 0.0));

	time = getSharedData().time;
	camPoses = { ofVec3f(0, -2000, 0), ofVec3f(0, -1000, 2000) };
	camIdx = 0;

	sceneMode = 0;
}

// =========================================================================================
void SceneA::update() {
	time = getSharedData().time;

	if (sceneMode == 0) {
		scene1();
	}
	else if (sceneMode == 1) {
		scene2();
	}

	getSharedData().fbo.begin();
	getSharedData().post.draw(0, 0);
	getSharedData().fbo.end();

}

// =========================================================================================
void SceneA::draw() {
	//getSharedData().post.draw(0, 0);
	getSharedData().fbo.draw(0, 0);
	getSharedData().gui.draw();
	gui.draw();
}

// =========================================================================================
Circle SceneA::spawnCircle(float xRange, float yRange, ofVec2f sizeRange, ofColor color, bool isBright = false) {
	float x = ofRandom(-xRange, xRange);
	float y = ofRandom(-yRange, yRange);
	float z = ofRandom(-30, 30);
	float size = ofRandom(sizeRange.x, sizeRange.y);
	int res = ofRandom(3);

	return Circle(res, size, ofVec3f(x, y, z), color, isBright);
}

// =========================================================================================
void SceneA::removeCircle(int index) {

}


// =========================================================================================
// rotating circle
void SceneA::scene1() {
	myCam.customSetPosition(ofVec3f(0, -2000, 0), ofVec3f(0, 0, 0));

	// rendering pass
	renderFbo.begin();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myCam.begin();
	glEnable(GL_DEPTH_TEST);

	ofPushMatrix();
	ofRotateXDeg(90);

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	for (int i = 0; i < circles.size(); i++) {
		circles[i].update(time);
		circles[i].draw();
	}

	ofPopMatrix();

	ofDisableAlphaBlending();

	glDisable(GL_DEPTH_TEST);
	myCam.end();

	renderFbo.end();


	// post effect pass
	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

// =========================================================================================
// circle Burst
void SceneA::scene2() {
	myCam.customSetPosition(camPoses[camIdx], ofVec3f(0, 0, 0));

	// Occluding Rendering Pass
	occludeFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	myCam.begin();
	glEnable(GL_DEPTH_TEST);

	ofPushMatrix();
	ofRotateXDeg(90);

	renderShader.begin();
	for (int i = 0; i < circles.size(); i++) {
		if (circles[i].getIntensity() > 0.0f) circles[i].setIntensity(circles[i].getIntensity() - 0.01);
		else if (circles[i].getIntensity() <= 0.0f) circles[i].setBright(false);
		circles[i].update(time);
		renderShader.setUniform1f("intensity", circles[i].getIntensity());
		renderShader.setUniform1i("isBright", circles[i].getBright());
		circles[i].draw();
	}
	renderShader.end();

	ofPopMatrix();

	glDisable(GL_DEPTH_TEST);
	myCam.end();
	occludeFbo.end();

	// Normal Rendering Pass
	normalFbo.begin();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myCam.begin();
	glEnable(GL_DEPTH_TEST);

	ofPushMatrix();
	ofRotateXDeg(90);

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	for (int i = 0; i < circles.size(); i++) {
		circles[i].draw();
	}

	ofPopMatrix();

	ofDisableAlphaBlending();

	glDisable(GL_DEPTH_TEST);
	myCam.end();

	normalFbo.end();

	// Volumetric Pass
	volumetricFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	volumetricShader.begin();
	volumetricShader.setUniformTexture("occludeTex", occludeFbo.getTexture(), 0);
	volumetricShader.setUniformTexture("colorTex", normalFbo.getTexture(), 1);
	volumetricShader.setUniform3f("screenLightPos", 0.5, screenY, 0.0);
	volumetricShader.setUniform1f("density", density);
	volumetricShader.setUniform1f("weight", weight);
	volumetricShader.setUniform1f("decay", decay);
	volumetricShader.setUniform1f("exposure", exposure);

	//renderFbo.draw(0, 0);
	occludeFbo.draw(0, 0);

	volumetricShader.end();

	volumetricFbo.end();

	// Post effect Pass
	//getSharedData().post.begin();
	renderFbo.begin();
	volumetricFbo.draw(0, 0);
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
	//getSharedData().post.end();
}
// =========================================================================================
void SceneA::keyPressed(int key) {
	int NUM_BRIGHT = 0;
	switch (key) {
	case 'a':
		switch (sceneMode) {
		case 0:
			break;
		case 1:
			NUM_BRIGHT = 5;
			break;
		case 2:
			NUM_BRIGHT = 20;
			break;
		case 3:
			NUM_BRIGHT = 50;
			break;
		case 4:
			NUM_BRIGHT = 100;
			break;
		}
		myCam.setShakeTime(0.1);
		//myCam.startThread();
		for (int i = 0; i < NUM_BRIGHT; i++) {
			int idx = int(ofRandom(0, circles.size()));
			circles[idx].setIntensity(0.4f);
			circles[idx].setBright(true);
		}
		break;
	case 's':
		sceneMode = (sceneMode + 1) % 2;
		camIdx = (camIdx + 1) % 2;
		break;
	case 'd':
		for (int i = 0; i < circles.size(); i++) {
			circles[i].speedUp(0.1f);
		}
		break;
	}
}

// =========================================================================================
string SceneA::getName() {
	return "SceneA";
}