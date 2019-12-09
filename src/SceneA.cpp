#include "SceneA.h"

void SceneA::setup() {
	ofFbo::Settings rs; // render settings
	rs.width = ofGetWidth();
	rs.height = ofGetHeight();
	rs.useDepth = true;
	rs.useStencil = true;
	rs.depthStencilAsTexture = true;
	rs.textureTarget = GL_TEXTURE_2D;
	renderFbo.allocate(rs);
	occludeFbo.allocate(rs);
	volumetricFbo.allocate(rs);

	for (unsigned int i = 0; i < 100; i++) {
		unsigned int res = int(ofRandom(3, 20));
		unsigned int size = int(ofRandom(100, 200));
		ofVec3f pos = ofVec3f(ofRandom(-ofGetWidth(), ofGetWidth()), ofRandom(-ofGetWidth(), ofGetWidth()), 0);
		bool isBright = i % 10 == 0 ? true : false;
		ofColor color = ofColor(30, 200, 200, 100);
		Circle circle = Circle(res, size, pos, color, isBright);
		circles.push_back(circle);
	}
	ofSetLineWidth(20);

	renderShader.load("shader/SceneA/render.vert", "shader/SceneA/render.frag");
	volumetricShader.load("shader/SceneA/volumetric.vert", "shader/SceneA/volumetric.frag");

	gui.setup();
	gui.setPosition(10, 10);
	gui.add(density.set("Density", 1.0, 0.0, 1.0));
	gui.add(weight.set("Weight", 1.0, 0.0, 1.0));
	gui.add(decay.set("decay", 1.0, 0.0, 1.0));
	gui.add(exposure.set("exposure", 1.0, 0.0, 1.0));
	gui.add(screenY.set("ScreenY", -1.0, -10.0, 0.0));
}

void SceneA::update() {
	float time = getSharedData().time;
	/*float x = 3000 * sin(time);
	float y = 500;
	float z = 3000 * cos(time);*/
	float x = 0;
	float y = 500;
	float z = 3000;
	cam.setPosition(x, y, z);
	cam.lookAt(ofVec3f(0, 0, 0));

	occludeFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	ofPushMatrix();
	ofRotateXDeg(90);

	renderShader.begin();
	for(int i = 0; i < circles.size(); i++) {
		renderShader.setUniform1i("isBright", circles[i].isBright);
		circles[i].draw();
	}
	renderShader.end();

	ofPopMatrix();

	glDisable(GL_DEPTH_TEST);
	cam.end();
	occludeFbo.end();
	
	renderFbo.begin();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam.begin();
	glEnable(GL_DEPTH_TEST);

	ofPushMatrix();
	ofRotateXDeg(90);

	ofEnableBlendMode(OF_BLENDMODE_ADD);
	for (int i = 0; i < circles.size(); i++) {
		circles[i].draw();
	}

	ofPopMatrix();

	ofDisableAlphaBlending();

	glDisable(GL_DEPTH_TEST);
	cam.end();

	renderFbo.end();

	volumetricFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	volumetricShader.begin();
	volumetricShader.setUniformTexture("occludeTex", occludeFbo.getTexture(), 0);
	volumetricShader.setUniformTexture("colorTex", renderFbo.getTexture(), 1);
	volumetricShader.setUniform3f("screenLightPos", 0.5, screenY, 0.0);
	volumetricShader.setUniform1f("density", density);
	volumetricShader.setUniform1f("weight",weight);
	volumetricShader.setUniform1f("decay", decay);
	volumetricShader.setUniform1f("exposure", exposure);

	//renderFbo.draw(0, 0);
	occludeFbo.draw(0, 0);

	volumetricShader.end();

	volumetricFbo.end();

	getSharedData().post.begin();
	volumetricFbo.draw(0, 0);
	//occludeFbo.draw(0, 0);
	//renderFbo.draw(0, 0);
	getSharedData().post.end();

}

void SceneA::draw() {
	getSharedData().post.draw(0, 0);
	gui.draw();
}

string SceneA::getName() {
	return "SceneA";
}