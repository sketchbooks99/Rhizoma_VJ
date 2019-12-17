#include "SceneA.h"

// =========================================================================================
void SceneA::setup() {

	ofEnableAntiAliasing();

	// Render settings
	ofFbo::Settings rs; 
	rs.width = ofGetWidth();
	rs.height = ofGetHeight();
	rs.useDepth = true;
	rs.useStencil = true;
	rs.depthStencilAsTexture = true;
	rs.textureTarget = GL_TEXTURE_2D;
	normalFbo.allocate(rs);
	occludeFbo.allocate(rs);
	volumetricFbo.allocate(rs);
	//rs.numSamples = 16;
	/*rs.useDepth = false;
	rs.depthStencilAsTexture = false;
	rs.useStencil = false;
	rs.numSamples = 4;
	renderFbo.allocate(rs);*/

	// Init circles
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

	// Load shaders
	renderShader.load("shader/SceneA/render.vert", "shader/SceneA/render.frag");
	volumetricShader.load("shader/SceneA/volumetric.vert", "shader/SceneA/volumetric.frag");
	planeShader.load("shader/SceneA/plane.vert", "shader/SceneA/plane.frag", "shader/SceneA/plane.geom");
	dancerShader.load("shader/SceneA/dancer.vert", "shader/SceneA/dancer.frag", "shader/SceneA/dancer.geom");
	pixelizeDancer.load("shader/SceneA/pixelizeDancer.vert", "shader/SceneA/pixelizeDancer.frag", "shader/SceneA/pixelizeDancer.geom");

	// Init custom GUI
	gui.setup();
	gui.setPosition(getSharedData().gui.getWidth() + 10, 10);
	gui.add(density.set("Density", 0.07, 0.0, 1.0));
	gui.add(weight.set("Weight", 1.0, 0.0, 1.0));
	gui.add(decay.set("decay", 0.97, 0.0, 1.0));
	gui.add(exposure.set("exposure", 1.0, 0.0, 1.0));
	gui.add(screenY.set("ScreenY", -10.0, -10.0, 0.0));

	// General Settings
	time = getSharedData().time;
	camPoses = { ofVec3f(0, -2000, 0), ofVec3f(0, -1000, 2000) };
	camIdx = 0;
	sceneMode = 0;
	cam.setDistance(150);

	// Geometry settings
	plane = ofPlanePrimitive(100, 100, 30, 30).getMesh();
	for (int x = 0; x < 30; x++) {
		for (int y = 0; y < 30; y++) {
			plane.addTexCoord(ofVec2f(x, y) / 30);
		}
	}

	// Sphere man
	sphere = ofSpherePrimitive(2, 16).getMesh();
	filename = "bvh01-09/05/05_05.bvh";
	bvh = ofxBvh(filename);
	bvh.setLoop(true);
	bvh.play();
}

// =========================================================================================
void SceneA::update() {
	// Bloom set
	getSharedData().bloom->setStrength(1.0);
	getSharedData().bloom->setEnabled(false);
	// Scene update
	bvh.update();
	time = getSharedData().time;

	if (getSharedData().isKicked) planeHeight = 10.0;
	else planeHeight -= 0.1;

	if (sceneMode == 0) {
		scene1();
	}
	else if (sceneMode == 1) {
		scene4();
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
// rotating circle
void SceneA::scene3() {
	cam.customSetPosition(ofVec3f(0, -2000, 0), ofVec3f(0, 0, 0));

	// rendering pass
	renderFbo.begin();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam.begin();
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
	cam.end();

	renderFbo.end();


	// post effect pass
	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

// =========================================================================================
// circle Burst
void SceneA::scene2() {
	cam.customSetPosition(camPoses[camIdx], ofVec3f(0, 0, 0));

	// Occluding Rendering Pass
	occludeFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();
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
	cam.end();
	occludeFbo.end();

	// Normal Rendering Pass
	normalFbo.begin();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam.begin();
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
	cam.end();

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
void SceneA::scene1() {
	//cam.customSetPosition(ofVec3f(100, 100, 200), ofVec3f(0, 0, 0));
	float radius = 100;
	cam.setPosition(sin(time * 0.1) * radius, -10, cos(time * 0.2) * radius);
	cam.lookAt(ofVec3f(0, 0, 0));

	// Occluding rendering pass

	// rendering pass
	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	cam.begin();

	ofMatrix4x4 model;
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 projection = cam.getProjectionMatrix();
	ofMatrix4x4 mvpMatrix = model * view * projection;
	ofMatrix4x4 invMatrix = mvpMatrix.getInverse();

	ofPushMatrix();

	ofRotateXDeg(90);

	plane.draw(OF_MESH_WIREFRAME);

	planeShader.begin();
	planeShader.setUniform1f("intensity", getSharedData().volume);
	planeShader.setUniform1i("isWhite", false);
	planeShader.setUniform1f("planeHeight", planeHeight);
	planeShader.setUniformMatrix4f("invMatrix", invMatrix);
	plane.draw(OF_MESH_FILL);
	planeShader.end();

	ofPopMatrix();

	cam.end();

	glDisable(GL_DEPTH_TEST);
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}
// =========================================================================================
void SceneA::scene4() {
	float radius = 100;
	cam.setPosition(sin(time * 0.1) * radius, -10, cos(time * 0.2) * radius);
	cam.lookAt(ofVec3f(0, 0, 0));

	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	// Draw Floor
	ofMatrix4x4 model;
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 projection = cam.getProjectionMatrix();
	ofMatrix4x4 mvpMatrix = model * view * projection;
	ofMatrix4x4 invMatrix = mvpMatrix.getInverse();

	ofPushMatrix();

	ofRotateXDeg(90);

	plane.draw(OF_MESH_WIREFRAME);

	planeShader.begin();
	planeShader.setUniform1f("intensity", getSharedData().volume);
	planeShader.setUniform1i("isWhite", false);
	planeShader.setUniform1f("planeHeight", planeHeight);
	planeShader.setUniformMatrix4f("invMatrix", invMatrix);
	planeShader.setUniform1f("time", time);
	plane.draw(OF_MESH_FILL);
	planeShader.end();

	ofPopMatrix();

	dancerShader.begin();
	for (int i = 0; i < bvh.getJoints().size(); i++) {
		//ofMatrix4x4 m = bvh.getJoints()[i]->localMat;
		//ofPushMatrix();
		/*float angle;
		ofVec3f axis;
		m.getRotate().getRotate(angle, axis);
		ofScale(1, -1, 1);*/

		model = ofMatrix4x4();
		model.translate(bvh.getJoints()[i]->getPosition() * 0.2);
		mvpMatrix = model * view * projection;
		invMatrix = mvpMatrix.getInverse();

		dancerShader.setUniformMatrix4f("mvpMatrix", mvpMatrix);
		dancerShader.setUniformMatrix4f("invMatrix", invMatrix);
		//ofRotate(angle, axis.x, axis.y, axis.z);
		sphere.draw(OF_MESH_FILL);
		//ofPopMatrix();
	}
	dancerShader.end();

	glDisable(GL_DEPTH_TEST);
	cam.end();
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
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
		cam.setShakeTime(0.1);
		//cam.startThread();
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