#include "SceneF.h"

//-------------------------------------------------------------------------
void SceneF::setup() {
	int xRes = 100, yRes = 100;
	terrain = ofPlanePrimitive(2000, 2000, xRes, yRes).getMesh();
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
		float z = (2000 * i) / 5;
		FCircle circle = FCircle(res, 400, ofVec3f(0, 0, z), ofVec3f(0, 0, -1));
		circle.setSpeed(10.0);
		circles.push_back(circle);
	}

	cam.setDistance(300.0);
}

//-------------------------------------------------------------------------
void SceneF::update() {
	cam.setPosition(0, -600, -500);
	cam.lookAt(ofVec3f(0, 0, 0));
	time = getSharedData().time;
	
	scene1();

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
	//ofBackgroundGradient(ofFloatColor(fmod(time, 1.0), 0.0, 0.0, 1.0), ofFloatColor(0.0, 0.0, 1.0, 1.0));
	ofBackgroundGradient(ofFloatColor(fmod(time, 1.0), 1.0), ofFloatColor(0.0));
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	ofPushMatrix();
	ofRotateXDeg(90);
	terrainShader.begin();
	if (sceneMode == 1) {
		terrainShader.setUniform1i("isBoxel", true);
	}
	else {
		terrainShader.setUniform1i("isBoxel", false);
	}
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
	case 'z':
		sceneMode = 0;
		break;
	case 'x':
		sceneMode = 1;
		break;
	case 's':
		for (int i = 0; i < circles.size(); i++) {
			circles[i].setRotate(ofRandom(-5.0, 5.0));
		}
		break;
	}
}

//-------------------------------------------------------------------------
string SceneF::getName() {
    return "SceneF";
}