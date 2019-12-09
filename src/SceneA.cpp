#include "SceneA.h"

void SceneA::setup() {
	ofFbo::Settings rs; // render settings
	rs.width = ofGetWidth();
	rs.height = ofGetHeight();
	rs.useDepth = true;
	rs.useStencil = true;
	rs.depthStencilAsTexture = true;
	renderFbo.allocate(rs);

	for (unsigned int i = 0; i < 100; i++) {
		unsigned int res = int(ofRandom(3, 20));
		unsigned int size = int(ofRandom(20, 100));
		ofVec3f pos = ofVec3f(ofRandom(-ofGetWidth() / 2, ofGetWidth() / 2), ofRandom(-ofGetHeight() / 2, ofGetHeight() / 2), ofRandom(-20, 20));
		bool isBright = false;
		ofColor color = ofColor(30, 200, 200, 100);
		Circle circle = Circle(res, size, pos, color, isBright);
		circles.push_back(circle);
	}
	ofSetLineWidth(20);

}

void SceneA::update() {
	renderFbo.begin();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam.begin();
	glEnable(GL_DEPTH_TEST);

	ofEnableBlendMode(OF_BLENDMODE_ADD);
	for (int i = 0; i < circles.size(); i++) {
		circles[i].draw();
	}

	ofDisableAlphaBlending();

	glDisable(GL_DEPTH_TEST);
	cam.end();

	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();

}

void SceneA::draw() {
	getSharedData().post.draw(0, 0);
}

string SceneA::getName() {
	return "SceneA";
}