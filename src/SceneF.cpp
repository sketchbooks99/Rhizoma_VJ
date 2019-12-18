#include "SceneF.h"

//-------------------------------------------------------------------------
void SceneF::setup() {
	int xRes = 20, yRes = 20;
	terrain = ofPlanePrimitive(1000, 1000, xRes, yRes).getMesh();
	terrainShader.load("shader/SceneF/terrain.vert", "shader/SceneF/terrain.frag", "shader/SceneF/terrain.geom");
	for (int i = 0; i < xRes; i++) {
		for (int j = 0; j < yRes; j++) {
			terrain.addTexCoord(ofVec2f(i, j) / ofVec2f(xRes, yRes));
		}
	}

	ofFbo::Settings rs;
	rs.width = ofGetWidth();
	rs.height = ofGetHeight();
	rs.useDepth = true;
	rs.useStencil = true;
	rs.depthStencilAsTexture = true;
	rs.textureTarget = GL_TEXTURE_2D;
	renderFbo.allocate(rs);

	sceneMode = 0;

	for (int i = 0; i < 5; i++) {
		unsigned int res = (int)ofRandom(3, 7);
		float z = ofMap(i, 0, 4, -300, 500);
		FCircle circle = FCircle(res, 400, ofVec3f(0, 0, z), ofVec3f(0, 0, -1));
		circle.setSpeed(10.0);
		circles.push_back(circle);
	}

	cam.setDistance(300.0);
}

//-------------------------------------------------------------------------
void SceneF::update() {
	cam.setPosition(0, -200, -500);
	cam.lookAt(ofVec3f(0, 0, 0));
	time = getSharedData().time;
	switch (sceneMode) {
	case 0:
		scene1();
		break;
	}

	getSharedData().fbo.begin();
	getSharedData().post.draw();
	getSharedData().fbo.end();

}

//-------------------------------------------------------------------------
void SceneF::draw() {
	getSharedData().fbo.draw(0, 0);
	getSharedData().post.draw();
	getSharedData().gui.draw();
}

//-------------------------------------------------------------------------
void SceneF::scene1() {
	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ofBackgroundGradient(ofFloatColor(fmod(time, 1.0), 0.0, 0.0, 1.0), ofFloatColor(0.0, 0.0, 1.0, 1.0));
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < circles.size(); i++) {
		circles[i].update();
		circles[i].draw(time);
		if (circles[i].getPosition().z < -500) circles[i].setPosition(ofVec3f(0, 0, 500));
	}

	ofPushMatrix();
	ofRotateXDeg(90);
	terrainShader.begin();
	terrainShader.setUniform1f("time", time);
	terrainShader.setUniform3f("lightDir", -0.577, -0.577, 0.577);
	terrain.draw(OF_MESH_FILL);
	terrainShader.end();
	ofPopMatrix();

	glDisable(GL_DEPTH_TEST);
	cam.end();
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

//-------------------------------------------------------------------------
void SceneF::keyPressed(int key) {
	switch (key) {
	case 'a':
		sceneMode = 0;
		break;
	}
}

//-------------------------------------------------------------------------
string SceneF::getName() {
    return "SceneF";
}