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

	createGBuffer();
	createRenderBuffer();

	cam.setNearClip(0.1);
	cam.setFarClip(5000);
	cam.setFov(70);
	cam.setDistance(5.0);


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

	sceneMode = 0;

	// lighting Fbo allocate
	ofFbo::Settings s;
	s.width = ofGetWidth();
	s.height = ofGetHeight();
	s.useDepth = true;
	s.useStencil = true;
	s.depthStencilAsTexture = true;
	s.textureTarget = GL_TEXTURE_2D;
	lightingFbo.allocate(s);
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
	lightPos = ofVec3f(10, 10, 10);
	glEnable(GL_DEPTH_TEST);

	// Render Geometry to G-Buffer
	/*gFbo.begin();
	gFbo.activateAllDrawBuffers();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam.begin();
	ofMatrix4x4 projection = cam.getProjectionMatrix();
	ofMatrix4x4 model;
	model.rotate(time * 50.0, 1.0, 0.7, 0.4);
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 mvpMatrix = model * view * projection;
	ofMatrix4x4 invMatrix = mvpMatrix.getInverse();

	gBufferShader.begin();
	gBufferShader.setUniformMatrix4f("model", model);
	gBufferShader.setUniformMatrix4f("view", view);
	gBufferShader.setUniformMatrix4f("peojection", projection);
	gBufferShader.setUniformMatrix4f("mvpMatrix", mvpMatrix);
	box.draw(OF_MESH_FILL);
	gBufferShader.end();

	cam.end();
	gFbo.end();*/

	// Render ray marching
	gFbo.begin();
	gFbo.activateAllDrawBuffers();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();

	ofMatrix4x4 projection = cam.getProjectionMatrix();
	ofMatrix4x4 view = ofGetCurrentViewMatrix();

	ray[sceneMode].begin();
	ray[sceneMode].setUniform1f("time", time);
	ray[sceneMode].setUniformMatrix4f("view", view);
	ray[sceneMode].setUniform2f("resolution", gFbo.getWidth(), gFbo.getHeight());
	ray[sceneMode].setUniformMatrix4f("projection", projection);
	ray[sceneMode].setUniform3f("camPos", cam.getGlobalPosition());
	ray[sceneMode].setUniform3f("camUp", cam.getUpDir());
	ray[sceneMode].setUniform1i("isColored", isColored);
	ray[sceneMode].setUniform1f("fov", cam.getFov());
	ray[sceneMode].setUniform1f("farClip", cam.getFarClip());
	ray[sceneMode].setUniform1f("nearClip", cam.getNearClip());

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
	case 's':
		isColored = !isColored;
	}
}

//--------------------------------------------------------------
string SceneC::getName() {
	return "SceneC";
}

