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
	ofEnableArbTex();
	numTrail = 4096;
	trailLength = 20;
	trail.init(numTrail, trailLength);
	trail.createMesh(trail.getLength());
	trail.createBuffers(trail.getNum(), trail.getLength());
	trail.loadVelocityShader("shader/Trail/passthru.vert", "shader/Trail/velUpdate.frag");
	trail.loadPositionShader("shader/Trail/passthru.vert", "shader/Trail/posUpdate.frag");
	trail.loadRenderShader("shader/Trail/createTrail.vert", "shader/Trail/createTrail.frag", "shader/Trail/createTrail.geom");
	//ofDisableArbTex();

	trailSphere = ofSpherePrimitive(300, 32).getMesh();
	for (int i = 0; i < trailSphere.getVertices().size(); i++) {
		trailSphere.addColor(ofFloatColor(0.0, 0.0, 0.0, 1.0));
	}
	size = 100.0;

	sceneMode = 0;
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

	boxelShader.begin();
	boxelShader.setUniform1f("scale", 4.0);
	boxelShader.setUniform3f("inColor", .4, .2, .6);
	boxelShader.setUniform3f("outColor", .7, .2, .1);
	boxelShader.setUniform3f("spColor", .0, .0, .0);
	boxelShader.setUniform1f("time", ofGetElapsedTimef());
	boxel.draw();
	boxelShader.end();

	glDisable(GL_DEPTH_TEST);
	cam.end();
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

//--------------------------------------------------------------
void SceneD::scene2() {
	trail.setWidth(7.0);
	trail.update();

	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ofBackground(ofFloatColor(0.3, 1.0));
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	trail.setUniforms(cam.getGlobalPosition(), 10.0, size);
	trail.draw(OF_MESH_FILL);

	trailSphere.draw();

	glDisable(GL_DEPTH_TEST);
	cam.end();

	trail.debugTexture(100, 100);
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}
//--------------------------------------------------------------
void SceneD::keyPressed(int key) {
	switch (key) {
	case 'a':
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
	case 'n':
		sceneMode = 5;
		break;
	case 'm':
		sceneMode = 6;
		break;
	}
}

//--------------------------------------------------------------
string SceneD::getName() {
	return "SceneD";
}