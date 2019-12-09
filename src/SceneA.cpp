#include "SceneA.h"

void SceneA::setup() {
	ofBackground(0);
	ofSetCircleResolution(64);
	//ofSetFrameRate(60);

	ofSetSmoothLighting(true);

	radius = 100;
	plWidth = 1000; plHeight = 1000;

	vboMesh = ofSpherePrimitive(radius, 64).getMesh();

	planeMesh = ofPlanePrimitive(plWidth, plHeight,10, 10).getMesh();
	for (int i = 0; i < planeMesh.getVertices().size(); i++) {
		int x = i % 10;
		int y = i / 10;
		ofVec3f pos = planeMesh.getVertex(i);
		pos.z += ofNoise(x, y) * 50.0;

		float delight = ofRandom(1.0);
		planeMesh.setVertex(i, pos);

		ofVec4f spawn = ofVec4f(pos.x, pos.y, pos.z, ofRandom(1, 3));
		spawnP.push_back(spawn);

		float velZ = ofRandom(3.0, 6.0);
		spawnVelZ.push_back(velZ);
	}

	translate = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, 0);

	glPointSize(3.0f);


}

void SceneA::update() {
	time = getSharedData().time;

	for (int i = 0; i < planeMesh.getVertices().size(); i++) {
		ofVec3f pos = planeMesh.getVertex(i);
		float newZ = pos.z + sin(time + i) * 0.3;
		planeMesh.setVertex(i, ofVec3f(pos.x, pos.y, newZ));

		if (spawnP[i].w < 0) {
			spawnP[i] = ofVec4f(pos.x, pos.y, pos.z, ofRandom(1, 2));
			spawnVelZ[i] = ofRandom(3, 6);
		}
		spawnP[i].w -= 0.01;
		spawnVelZ[i] *= 0.99;
		spawnP[i].z += spawnVelZ[i];
	}

	getSharedData().post.begin(cam);

	glEnable(GL_DEPTH_TEST);

	ofPushMatrix();
	ofTranslate(0, 100, 0);

	vboMesh.draw();

	ofPopMatrix();

	ofPushMatrix();
	ofScale(1, -1, 1);
	ofRotateXDeg(90.0);
	ofTranslate(0, 0, -200);
	ofSetColor(255);
	planeMesh.drawWireframe();
	ofSetColor(255, 50);
	planeMesh.draw();
	for (int i = 0; i < planeMesh.getVertices().size(); i++) {
		ofVec3f pos = planeMesh.getVertex(i);
		ofSetColor(100, 200, 200, 200);
		ofDrawSphere(pos, 5);
		ofDrawSphere(spawnP[i].x, spawnP[i].y, spawnP[i].z, 5);
	}

	ofPopMatrix;

	glDisable(GL_DEPTH_TEST);

	getSharedData().post.end();
}

void SceneA::draw() {
	getSharedData().post.draw(0, 0);
	getSharedData().gui.draw();
}

string SceneA::getName() {
	return "SceneA";
}