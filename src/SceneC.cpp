#include "SceneC.h"

// =============================================================
void SceneC::setup() {
	gBufferShader.load("shader/SceneC/gBuffer.vert", "shader/SceneC/gBuffer.frag");
	//gRenderShader.load("shader/SceneC/gRender.vert", "shader/SceneC/gRender.frag");
	ray[0].load("shader/SceneC/ray.vert", "shader/SceneC/ray0.frag");
	ray[1].load("shader/SceneC/ray.vert", "shader/SceneC/ray1.frag");
	ray[2].load("shader/SceneC/ray.vert", "shader/SceneC/ray2.frag");
	ray[3].load("shader/SceneC/ray.vert", "shader/SceneC/ray3.frag");
	ray[4].load("shader/SceneC/ray.vert", "shader/SceneC/ray4.frag");
	lightingShader.load("shader/SceneC/lighting.vert", "shader/SceneC/lighting.frag");
	sphereShader.load("shader/SceneC/sphere.vert", "shader/SceneC/sphere.frag", "shader/SceneC/sphere.geom");

	createGBuffer();
	createRenderBuffer();

	cam.setNearClip(0.1);
	cam.setFarClip(5000);
	cam.setFov(70);
	cam.setDistance(5.0);

	// Camera positions
	camRadiuses.push_back(ofVec3f(5,0,5));
	timeOffsets.push_back(ofVec3f(0.4));
	for (int i = 0; i < 3; i++) {
		ofVec3f radius;
		radius.x = ofRandom(3.0, 10.0);
		radius.y = ofRandom(3.0, 10.0);
		radius.z = ofRandom(3.0, 10.0);
		camRadiuses.push_back(radius);

		ofVec3f offset;
		offset.x = ofRandom(-0.7, 0.7);
		offset.y = ofRandom(-0.7, 0.7);
		offset.z = ofRandom(-0.7, 0.7);
		timeOffsets.push_back(offset);
	}

	// Plane to render ray marching
	quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	quad.addVertex(ofVec3f(1.0, 1.0, 0.0)); // top-right
	quad.addTexCoord(ofVec2f(1.0, 1.0));
	quad.addVertex(ofVec3f(1.0, -1.0, 0.0)); //bottom-right
	quad.addTexCoord(ofVec2f(1.0, 0.0f));
	quad.addVertex(ofVec3f(-1.0, -1.0, 0.0)); //bottom-left
	quad.addTexCoord(ofVec2f(0.0f, 0.0f));
	quad.addVertex(ofVec3f(-1.0, 1.0, 0.0)); //top-left
	quad.addTexCoord(ofVec2f(0.0f, 1.0));

	// Geometry settings
	box = ofBoxPrimitive(4,4,4).getMesh();
	for(int i = 0; i < box.getVertices().size(); i++) {
		box.addColor(ofFloatColor(1, 0, 0, 1));
	}

	// Custom GUI Settings
	gui.setup();
	gui.setPosition(getSharedData().gui.getWidth() + 10, 10);
	gui.add(showTex.setup("showTex", false));
	gui.add(isColored.setup("isColored", false));
	gui.add(isGeometry.setup("isGeometry", false));
	gui.add(isShade.setup("isShade", true));

	sceneMode = 0;
	camIdx = 0;

	// lighting Fbo allocate
	ofFbo::Settings s;
	s.width = ofGetWidth();
	s.height = ofGetHeight();
	s.useDepth = true;
	s.useStencil = true;
	s.depthStencilAsTexture = true;
	s.textureTarget = GL_TEXTURE_2D;
	lightingFbo.allocate(s);

	for (int i = 0; i < 5; i++) sceneArray[i] = 0;

	// setup geometry
	sphere = ofIcoSpherePrimitive(1.5, 2).getMesh();

	rayCams.push_back(ofVec3f(0, 0, 10));
	rayCams.push_back(ofVec3f(-1, 0, -10));
	rayCams.push_back(ofVec3f(2, 2, 10));

	sceneMode = 0;
}

// =============================================================
void SceneC::update() {
	time = getSharedData().time;
	/*switch (sceneMode) {
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
	case 4:
		scene5();
		break;
	}*/
	scene1();
}

// =============================================================
void SceneC::draw() {
	getSharedData().fbo.draw(0, 0);
	getSharedData().gui.draw();
	gui.draw();
	if (showTex) {
		gFbo.getTexture(0).draw(10, getSharedData().gui.getHeight() + 10, 100, 100);
		gFbo.getTexture(1).draw(110, getSharedData().gui.getHeight() + 10, 100, 100);
		gFbo.getTexture(2).draw(210, getSharedData().gui.getHeight() + 10, 100, 100);
	}
}

//--------------------------------------------------------------
void SceneC::scene1() {
	float camY = 0;
	switch (sceneMode) {
	case 0:
		if (sceneArray[0] == 0 || sceneArray[0] == 1) {
			cam.setPosition(
				camRadiuses[camIdx].x * sin(time * timeOffsets[camIdx].x),
				camY,
				camRadiuses[camIdx].z * cos(time * timeOffsets[camIdx].z)
			);
		}
		else if(sceneArray[0] == 2 || sceneArray[0] == 3) {
			cam.setPosition(
				camRadiuses[camIdx].x * sin(time * timeOffsets[camIdx].x),
				camRadiuses[camIdx].z* cos(time * timeOffsets[camIdx].y),
				camRadiuses[camIdx].z * cos(time * timeOffsets[camIdx].z)
			);
		}

		cam.lookAt(ofVec3f(0, 0, 0));
		break;
	case 1:
	case 2:
		if (camIdx <= 2) cam.setPosition(rayCams[camIdx]);
		else cam.setPosition(ofVec3f(sin(time * 0.33), cos(time * 0.45), 10));
		cam.lookAt(ofVec3f(0, 0, 0));
		break;
	}
	lightPos = ofVec3f(-10, -10, 10);
	glEnable(GL_DEPTH_TEST);

	ofMatrix4x4 projection, model, view, mvpMatrix;

	if(isGeometry && sceneMode > 0) {
		// Render Geometry to G-Buffer
		gFbo.begin();
		gFbo.activateAllDrawBuffers();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam.begin();
		projection = cam.getProjectionMatrix();
		model.rotate(time * 50.0, 1.0, 0.7, 0.4);
		view = ofGetCurrentViewMatrix();
		mvpMatrix = model * view * projection;
		sphereShader.begin();
		sphereShader.setUniformMatrix4f("model", model);
		sphereShader.setUniformMatrix4f("view", view);
		sphereShader.setUniformMatrix4f("peojection", projection);
		sphereShader.setUniformMatrix4f("mvpMatrix", mvpMatrix);
		sphereShader.setUniform1f("time", time);
		sphereShader.setUniform1f("volume", getSharedData().volume);
		sphere.draw(OF_MESH_FILL);
		sphereShader.end();

		cam.end();
		gFbo.end();
	}

	// Render ray marching
	gFbo.begin();
	gFbo.activateAllDrawBuffers();

	if (!isGeometry || sceneMode == 0) {
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	cam.begin();

	/*ofMatrix4x4 projection = cam.getProjectionMatrix();
	ofMatrix4x4 view = ofGetCurrentViewMatrix();*/

	ray[sceneMode].begin();
	ray[sceneMode].setUniform1f("time", time);
	ray[sceneMode].setUniformMatrix4f("view", view);
	ray[sceneMode].setUniform1f("volume", getSharedData().volume);
	ray[sceneMode].setUniform1i("sceneMode", sceneArray[sceneMode]);
	ray[sceneMode].setUniform2f("resolution", gFbo.getWidth(), gFbo.getHeight());
	ray[sceneMode].setUniformMatrix4f("projection", projection);
	ray[sceneMode].setUniform3f("camPos", cam.getGlobalPosition());
	ray[sceneMode].setUniform3f("camUp", cam.getUpDir());
	ray[sceneMode].setUniform1i("isColored", isColored);
	ray[sceneMode].setUniform1f("fov", cam.getFov());
	ray[sceneMode].setUniform1f("farClip", cam.getFarClip());
	ray[sceneMode].setUniform1f("nearClip", cam.getNearClip());
	ray[sceneMode].setUniform1i("sceneMode", sceneArray[sceneMode]);

	quad.draw(OF_MESH_FILL);

	ray[sceneMode].end();
	cam.end();
	gFbo.end();

	glDisable(GL_DEPTH_TEST);

	lightingFbo.begin();
	lightingShader.begin();
	lightingShader.setUniformTexture("gPosition", gFbo.getTexture(0), 0);
	lightingShader.setUniformTexture("gNormal", gFbo.getTexture(1), 1);
	lightingShader.setUniformTexture("gColor", gFbo.getTexture(2), 2);
	lightingShader.setUniform3f("camPos", cam.getPosition());
	lightingShader.setUniform1i("isShade",isShade);
	lightingShader.setUniform3f("lightPos", lightPos);
	quad.draw(OF_MESH_FILL);
	lightingShader.end();
	lightingFbo.end();

	getSharedData().post.begin();
	//gFbo.getTexture(2).draw(0, 0);
	lightingFbo.draw(0, 0);
	getSharedData().post.end();

	getSharedData().fbo.begin();
	getSharedData().post.draw();
	getSharedData().fbo.end();
}

//--------------------------------------------------------------
void SceneC::scene2() {

}

//--------------------------------------------------------------
void SceneC::scene3() { // simple scene
}
//--------------------------------------------------------------
void SceneC::scene4() { // fucking good
}
//--------------------------------------------------------------
void SceneC::scene5() { // ray & sphere
}

// =============================================================
void SceneC::createGBuffer() {

	ofFbo::Settings gSettings;
	gSettings.textureTarget = GL_TEXTURE_2D;
	gSettings.width = ofGetWidth();
	gSettings.height = ofGetHeight();
	gSettings.internalformat = GL_RGB32F;
	gSettings.useDepth = true;
	gSettings.useStencil = true;
	gSettings.depthStencilAsTexture = true;
	gSettings.depthStencilInternalFormat = GL_DEPTH_COMPONENT;
	gSettings.maxFilter = GL_NEAREST;
	gSettings.minFilter = GL_NEAREST;
	gSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
	gSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	gFbo.allocate(gSettings);

	gFbo.createAndAttachTexture(GL_RGB32F, 1);
	gFbo.createAndAttachTexture(GL_RGB32F, 2);
	gFbo.createAndAttachTexture(GL_RGB32F, 3);
	gFbo.checkStatus();
}

//--------------------------------------------------------------
void SceneC::createRenderBuffer() {

	ofFbo::Settings rSettings;
	rSettings.textureTarget = GL_TEXTURE_2D;
	rSettings.width = ofGetWidth();
	rSettings.height = ofGetHeight();
	rSettings.useDepth = true;
	rSettings.useStencil = true;
	rSettings.internalformat = GL_RGB32F;
	rSettings.depthStencilAsTexture = true;
	rSettings.depthStencilInternalFormat = GL_DEPTH_COMPONENT;
	rSettings.maxFilter = GL_NEAREST;
	rSettings.minFilter = GL_NEAREST;

	rSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	rSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
	renderFbo.allocate(rSettings);

	renderFbo.createAndAttachTexture(GL_RGB32F, 1);
}
//--------------------------------------------------------------
void SceneC::keyPressed(int key) {
	switch (key) {
	// Camera change
	case 'a':
		camIdx = (int)ofRandom(0, camRadiuses.size());
		break;
	// Local Scene change
	case 's':
		isColored = !isColored;
		break;
	case 'd':
		isGeometry = !isGeometry;
		break;
	case 'f':
		isShade = !isShade;
		break;
	case 'z':
		sceneMode = 0;
		isShade = true;
		break;
	case 'x':
		sceneMode = 1;
		isShade = false;
		break;
	case 'c':
		sceneMode = 2;
		isShade = false;
		break;
	case 'v':
		sceneMode = 3;
		break;
	case 'b':
		sceneMode = 4;
		break;
	case 'g':
		sceneArray[sceneMode] = 0;
		break;
	case 'h':
		sceneArray[sceneMode] = 1;
		break;
	case 'j':
		sceneArray[sceneMode] = 2;
		break;
	case 'k':
		sceneArray[sceneMode] = 3;
		break;
	case 'l':
		sceneArray[sceneMode] = 4;
		break;
	}
}

//--------------------------------------------------------------
string SceneC::getName() {
	return "SceneC";
}

