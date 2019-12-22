#include "SceneD.h"

//--------------------------------------------------------------
void SceneD::setup() {
	ofFbo::Settings rs;
	rs.width = ofGetWidth();
	rs.height = ofGetHeight();
	rs.useDepth = true;
	rs.useStencil = true;
	rs.depthStencilAsTexture = true;
	rs.textureTarget = GL_TEXTURE_2D;
	renderFbo.allocate(rs);

	boxel = ofIcoSpherePrimitive(200, 5).getMesh();

	boxelShader.load("shader/SceneD/boxel.vert", "shader/SceneD/boxel.frag", "shader/SceneD/boxel.geom");
	
	// trail setup
	size = 100.0;

	sceneMode = 0;

	plane = ofPlanePrimitive(10000, 10000, 300, 300).getMesh();
	//sky = ofPlanePrimitive(10000, 10000, 500, 500).getMesh();
	box = ofBoxPrimitive(4000, 4000, 4000 , 100, 100, 100).getMesh();

	manSphere = ofIcoSpherePrimitive(2, 5).getMesh();
	string filename = "bvh01-09/05/MMD_Lamb.bvh";
	bvh = ofxBvh(filename);
	//bvh.play();

	// Camera positions
	ofVec3f wallSize = ofVec3f(1000.0);
	camRadiuses.push_back(wallSize / 2);
	timeOffsets.push_back(ofVec3f(0.2));
	for (int i = 0; i < 3; i++) {
		ofVec3f radius;
		radius.x = ofRandom(wallSize.x, wallSize.x * 3);
		radius.y = ofRandom(wallSize.y, wallSize.y * 3);
		radius.z = ofRandom(wallSize.z, wallSize.z * 3);
		camRadiuses.push_back(radius);

		ofVec3f offset;
		offset.x = ofRandom(-0.7, 0.7);
		offset.y = ofRandom(-0.7, 0.7);
		offset.z = ofRandom(-0.7, 0.7);
		timeOffsets.push_back(offset);
	}
}

//--------------------------------------------------------------
void SceneD::update() {
	time = getSharedData().time;

	switch (sceneMode) {
	case 0:
		scene1();
		break;
	case 1:
		scene2();
		break;
	case 2:
		scene3();
		break;
	case 3:
		scene4();
		break;
	}
	getSharedData().fbo.begin();
	getSharedData().post.draw();
	getSharedData().fbo.end();
}

//--------------------------------------------------------------
void SceneD::draw() {
	getSharedData().fbo.draw(0, 0);
	getSharedData().gui.draw();
}

//--------------------------------------------------------------
void SceneD::scene1() {
	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	ofPushMatrix();
	ofRotate(time * 30, 0.4, 0.7, 1.0);
	boxelShader.begin();
	boxelShader.setUniform1f("scale", 4.0 + getSharedData().volume * 10.0);
	boxelShader.setUniform3f("inColor", .4, .2, .6);
	boxelShader.setUniform3f("outColor", .7, .2, .1);
	boxelShader.setUniform3f("spColor",.2,.2,.2);
	boxelShader.setUniform1f("time", ofGetElapsedTimef());
	boxelShader.setUniform1i("isShade", false);
	boxel.draw();
	boxelShader.end();
	ofPopMatrix();
	

	glDisable(GL_DEPTH_TEST);
	cam.end();
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

//--------------------------------------------------------------
void SceneD::scene2() {

	cam.setPosition(
		camRadiuses[camIdx].x * sin(time * timeOffsets[camIdx].x),
		camRadiuses[camIdx].y * cos(time * timeOffsets[camIdx].y),
		camRadiuses[camIdx].z * cos(time * timeOffsets[camIdx].z)
	);
	cam.lookAt(ofVec3f(0, 0, 0));

	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//ofBackground(ofFloatColor(0.3, 1.0));
	cam.begin();
	glEnable(GL_DEPTH_TEST);
	
	boxelShader.begin();
	boxelShader.setUniform1f("scale", 50.0 + getSharedData().volume * 10.0);
	boxelShader.setUniform3f("inColor", .4, .2, .6);
	boxelShader.setUniform3f("outColor", .7, .2, .1);
	boxelShader.setUniform3f("spColor", .2, .2, .2);
	boxelShader.setUniform1f("time", ofGetElapsedTimef());
	boxelShader.setUniform1i("isShade", false);
	box.draw();

	ofPushMatrix();
	boxelShader.setUniform1f("scale", 10.0);
	ofTranslate(0, 500, 0);
	ofRotateXDeg(90);
	plane.draw();
	ofPopMatrix();
	boxelShader.end();

	glDisable(GL_DEPTH_TEST);
	cam.end();
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}
//--------------------------------------------------------------
void SceneD::scene3() {
	cam.setPosition(
		camRadiuses[camIdx].x * sin(time * timeOffsets[camIdx].x),
		camRadiuses[camIdx].y * cos(time * timeOffsets[camIdx].y),
		camRadiuses[camIdx].z * cos(time * timeOffsets[camIdx].z)
	);
	cam.lookAt(ofVec3f(0, 0, 0));
	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//ofBackground(ofFloatColor(0.3, 1.0));
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	boxelShader.begin();
	boxelShader.setUniform1f("scale", 50.0);
	boxelShader.setUniform3f("inColor", .7,.7,.7);
	boxelShader.setUniform3f("outColor", .7,.7,.7);
	boxelShader.setUniform3f("spColor", .2, .4, .8);
	boxelShader.setUniform1f("time", ofGetElapsedTimef());
	boxelShader.setUniform1i("isShade", true);
	
	ofPushMatrix();
	ofTranslate(0, -1000, 0);
	ofRotateXDeg(90);
	plane.draw();
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(0, 1000, 0);
	ofRotateXDeg(90);
	plane.draw();
	ofPopMatrix();

	boxelShader.end();

	glDisable(GL_DEPTH_TEST);
	cam.end();
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

void SceneD::scene4() {
	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//ofBackground(ofFloatColor(0.3, 1.0));
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	boxelShader.begin();
	boxelShader.setUniform1f("scale", 1. + getSharedData().volume * 10.0);
	boxelShader.setUniform3f("inColor", .4, .2, .6);
	boxelShader.setUniform3f("outColor", .7, .2, .1);
	boxelShader.setUniform3f("spColor", .2, .2, .2);
	boxelShader.setUniform1f("time", ofGetElapsedTimef());
	boxelShader.setUniform1i("isShade", false);
	manSphere.draw();

	ofPushMatrix();
	boxelShader.setUniform1f("scale", 10.0);
	ofTranslate(0, 500, 0);
	ofRotateXDeg(90);
	plane.draw();
	ofPopMatrix();
	boxelShader.end();

	glDisable(GL_DEPTH_TEST);
	cam.end();
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}
//--------------------------------------------------------------
void SceneD::keyPressed(int key) {
	switch (key) {
	case 'a':
		camIdx = (int)ofRandom(0, camRadiuses.size());
		break;
		// Local Scene change
	case 'z':
		sceneMode = 0;
		break;
	case 'x':
		sceneMode = 1;
		break;
	case 'c':
		sceneMode = 2;
		break;
	case 'v':
		sceneMode = 3;
		break;
	case 'b':
		sceneMode = 4;
		break;
	}
}

//--------------------------------------------------------------
string SceneD::getName() {
	return "SceneD";
}