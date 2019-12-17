#include "SceneD.h"

//--------------------------------------------------------------
void SceneD::setup() {
	ofBackground(0);

	unsigned int radius = 300;
	sphere = ofSpherePrimitive(radius, 64).getMesh();

	for (unsigned int i = 0; i < 30; i++) {
		float theta = ofMap(ofRandom(0.0, 1.0), 0, 1, 0, TWO_PI);
		float phi = ofMap(ofRandom(0.0, 1.0), 0, 1, 0, PI);
		float x = radius * sin(theta) * cos(phi);
		float y = radius * sin(theta) * sin(phi);
		float z = radius * cos(theta);
		ofVec3f point = ofVec3f(x, y, z);
		points.push_back(point);

		ofVboMesh around = ofSpherePrimitive(ofRandom(0, 100), 32).getMesh();
		float gray = ofRandom(0.0, 1.0);
		for (unsigned int j = 0; j < around.getVertices().size(); j++) {
			around.addColor(ofFloatColor(gray, gray, gray, 1.0));
		}
		aroundSpheres.push_back(around);
	}

	for (unsigned int i = 0; i < sphere.getVertices().size(); i++) {
		sphere.addColor(ofFloatColor(0.2, 0.8, 0.8, 1.0));
	}

	sphereShader.load("shader/SceneD/sphere.vert", "shader/SceneD/sphere.frag");

	ofFbo::Settings rs;
	rs.width = ofGetWidth();
	rs.height = ofGetHeight();
	rs.useDepth = true;
	rs.useStencil = true;
	rs.depthStencilAsTexture = true;
	renderFbo.allocate(rs);
}

//--------------------------------------------------------------
void SceneD::update() {
	time = getSharedData().time;
	lightDir = ofVec3f(0.577, 0.577, -0.577);

	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam.begin();
	glEnable(GL_DEPTH_TEST);
	
	sphereShader.begin();

	ofPushMatrix();
	ofMatrix4x4 model;
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 projection = cam.getProjectionMatrix(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
	ofMatrix4x4 mvpMatrix = model * view * projection;
	ofMatrix4x4 invMatrix = mvpMatrix.getInverse();

	sphereShader.setUniformMatrix4f("mvpMatrix", mvpMatrix);
	sphereShader.setUniformMatrix4f("invMatrix", invMatrix);
	sphereShader.setUniform3f("lightDir", lightDir);

	sphere.draw();

	model.rotate(90, 1, 0, 0);
	mvpMatrix = model * view * projection;
	invMatrix = mvpMatrix.getInverse();

	sphereShader.setUniformMatrix4f("mvpMatrix", mvpMatrix);
	sphereShader.setUniformMatrix4f("invMatrix", invMatrix);

	for (unsigned int i = 0; i < points.size(); i++) {
		model = ofMatrix4x4();
		model.translate(points[i]);
		mvpMatrix = model * view * projection;
		invMatrix = mvpMatrix.getInverse();
		sphereShader.setUniformMatrix4f("mvpMatrix", mvpMatrix);
		sphereShader.setUniformMatrix4f("invMatrix", invMatrix);
		aroundSpheres[i].draw();
	}


	sphereShader.end();
	glDisable(GL_DEPTH_TEST);
	ofPopMatrix();
	cam.end();

	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

//--------------------------------------------------------------
void SceneD::draw() {
	getSharedData().post.draw();
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