#include "SceneA.h"

// =========================================================================================
void SceneA::setup() {
	planeHeight = 0.0;

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
	rs.numSamples = 4;
	renderFbo.allocate(rs);

	// Load shaders
	renderShader.load("shader/SceneA/render.vert", "shader/SceneA/render.frag");
	volumetricShader.load("shader/SceneA/volumetric.vert", "shader/SceneA/volumetric.frag");
	planeShader.load("shader/SceneA/plane.vert", "shader/SceneA/plane.frag", "shader/SceneA/plane.geom");
	sphereShader.load("shader/SceneA/sphere.vert", "shader/SceneA/sphere.frag", "shader/SceneA/sphere.geom");
	pixelizeDancer.load("shader/SceneA/pixelizeDancer.vert", "shader/SceneA/pixelizeDancer.frag", "shader/SceneA/pixelizeDancer.geom");
	wallShader.load("shader/SceneA/wallShader.vert", "shader/SceneA/wallShader.frag");// , "shader/SceneA/wallShader.geom");
	boxShader.load("shader/SceneA/box.vert", "shader/SceneA/box.frag", "shader/SceneA/box.geom");

	// Init custom GUI
	gui.setup();
	gui.setPosition(getSharedData().gui.getWidth() + 10, 10);
	gui.add(density.set("Density", 0.07, 0.0, 1.0));
	gui.add(weight.set("Weight", 1.0, 0.0, 1.0));
	gui.add(decay.set("decay", 0.97, 0.0, 1.0));
	gui.add(exposure.set("exposure", 1.0, 0.0, 1.0));
	gui.add(screenY.set("ScreenY", -10.0, -10.0, 0.0)); 
	gui.add(isPixeled.setup("isPixeled", false));
	gui.add(isColored.setup("isColored", false));
	gui.add(isReactive.setup("isReactive", false));

	// General Settings
	time = getSharedData().time;
	camPoses = { ofVec3f(0, -2000, 0), ofVec3f(0, -1000, 2000) };
	camIdx = 0;
	sceneMode = 0;
	cam.setDistance(150);

	// Geometry settings
	int xRes = 15, yRes = 15;
	plane = ofPlanePrimitive(150, 150, xRes, yRes).getMesh();
	for (int x = 0; x < xRes; x++) {
		for (int y = 0; y < yRes; y++) {
			plane.addTexCoord(ofVec2f(x, y) / 15);
		}
	}
	wall = ofBoxPrimitive(1, 10, 1).getMesh();
	box = ofBoxPrimitive(20, 20, 20, 3, 3, 3).getMesh();

	// Sphere man
	sphere = ofIcoSpherePrimitive(16, 2).getMesh();
}

// =========================================================================================
void SceneA::update() {
	// Scene update
	time = getSharedData().time;

	if (getSharedData().isKicked) planeHeight = 10.0;
	else planeHeight -= 0.1;

	if (sceneMode == 0) {
		if (getSharedData().isKicked) {
			getSharedData().zoomBlur->setEnabled(true);
			getSharedData().zoomBlur->setWeight(0.5);
		}
		else {
			if (getSharedData().zoomBlur->getWeight() > 0.0) getSharedData().zoomBlur->setWeight(getSharedData().zoomBlur->getWeight() - 0.01);
		}
		scene1();
	}
	else if (sceneMode == 1) {
		scene2();
	}
	else if (sceneMode == 2) {
		//scene4();
		scene3();
	}
	else if (sceneMode == 3) {
		isColored = true;
	}
	else if (sceneMode == 4) {
		scene5();
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
void SceneA::scene1() {
	//cam.customSetPosition(ofVec3f(100, 100, 200), ofVec3f(0, 0, 0));
	float radius = 100;
	//cam.setPosition(sin(time * 0.1) * radius, -10, cos(time * 0.2) * radius);
	cam.setPosition(0, -80, 0);
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
	planeShader.setUniform1i("isColored", isColored);
	planeShader.setUniform1f("planeHeight", planeHeight);
	planeShader.setUniformMatrix4f("invMatrix", invMatrix);
	planeShader.setUniform1f("time", time);
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
void SceneA::scene2() {
	float radius = 50;
	cam.setPosition(sin(time * 0.3) * radius, -10, cos(time * 0.3) * radius);
	cam.lookAt(ofVec3f(0, 0, 0));

	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	drawWall();

	ofMatrix4x4 model;
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 projection = cam.getProjectionMatrix();
	/*ofMatrix4x4 mvpMatrix = model * view * projection;
	ofMatrix4x4 invMatrix = mvpMatrix.getInverse();*/

	ofPushMatrix();
	ofRotateXDeg(90);
	plane.draw(OF_MESH_WIREFRAME);

	planeShader.begin();
	planeShader.setUniform1f("intensity", getSharedData().volume);
	planeShader.setUniform1i("isReactive", isReactive);
	planeShader.setUniform1i("isColored", isColored);
	planeShader.setUniform1f("planeHeight", planeHeight);
	planeShader.setUniform1f("time", time);
	plane.draw(OF_MESH_FILL);
	planeShader.end();
	ofPopMatrix();

	glDisable(GL_DEPTH_TEST);
	cam.end();
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

// =========================================================================================
void SceneA::scene3() {
	float radius = 40;
	cam.setPosition(sin(time * 0.3) * radius, -10, cos(time * 0.3) * radius);
	cam.lookAt(ofVec3f(0, 0, 0));

	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	// render scene
	sphereShader.begin();
	sphereShader.setUniform1f("time", time);
	sphere.draw(OF_MESH_FILL);
	sphereShader.end();

	glDisable(GL_DEPTH_TEST);
	cam.end();
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}
//------------------------------------------------------------------------------------------
void SceneA::scene4() {
	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();
	glEnable(GL_DEPTH_TEST);
	
	// render scene
	boxShader.begin();
	ofPushMatrix();
	ofRotate(time * 30.0, 0.4, 0.7, 1.0);
	boxShader.setUniform1f("time", time);
	box.draw(OF_MESH_FILL);
	ofPopMatrix();
	boxShader.end();

	glDisable(GL_DEPTH_TEST);
	cam.end();
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

//------------------------------------------------------------------------------------------
void SceneA::drawWall() {
	if (getSharedData().isKicked) wallOffset = 5.0;
	else {
		if (wallOffset > 1.0) wallOffset -= 0.1;
		else if (wallOffset < 1.0) wallOffset = 1.0;
	}
	for (int i = 0; i < getSharedData().left.size(); i++) {
		ofPushMatrix();
		wallShader.setUniform1i("index", i);
		float wallHeight = ofNoise(time * 0.2 + i * 0.1) * 5.0;
		ofScale(1, wallHeight * wallOffset, 1);
		float theta = ((float)i / getSharedData().left.size()) * TWO_PI;
		ofTranslate(sin(theta) * 100, 0, cos(theta) * 100);
		wall.draw(OF_MESH_FILL);
		ofPopMatrix();
	}
}

// =========================================================================================
void SceneA::keyPressed(int key) {
	int NUM_BRIGHT = 0;
	switch (key) {
	case 'a': // camera change
		break;
	case 's':
		isColored = !isColored;
		break;
	// Change child scene
	case 'd':
		isReactive = !isReactive;
		break;
	case 'z': // simple 
		sceneMode = 0;
		isColored = false;
		break;
	case 'x': // colored start scene
		sceneMode = 1;
		isColored = true;
		break;
	case 'c': // only sphere scene
		sceneMode = 2;
		isColored = false;
		break;
	case 'v':  // pop up cube
		sceneMode = 3;
		isColored = false;
		break;
	case 'b':  // room
		sceneMode = 4;
		break;
	case 'n': // geometry in cube
		sceneMode = 5;
		break;
	case 'm': 
		sceneMode = 6;
		break;
	}
}

// =========================================================================================
string SceneA::getName() {
	return "SceneA";
}

// =========================================================================================
// circle Burst
void SceneA::sceneBurst() {
	cam.customSetPosition(camPoses[camIdx], ofVec3f(0, 0, 0));

	// Occluding Rendering Pass
	occludeFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	ofPushMatrix();
	ofRotateXDeg(90);

	renderShader.begin();
	// ----------------------------------------
	// render bright geometry
	// ----------------------------------------

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
	
	// ----------------------------------------
	// render normal geometry
	// ----------------------------------------

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