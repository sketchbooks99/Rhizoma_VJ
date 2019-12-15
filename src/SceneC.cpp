#include "SceneC.h"

// =============================================================
void SceneC::setup() {
	gBufferShader.load("shader/SceneC/gBuffer.vert", "shader/SceneC/gBuffer.frag");
	//gRenderShader.load("shader/SceneC/gRender.vert", "shader/SceneC/gRender.frag");
	rayShader.load("shader/SceneC/ray.vert", "shader/SceneC/ray.frag");
	//lightingShader.load("shader/SceneC/lighting.vert", "shader/SceneC/lighting.frag");

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
}

// =============================================================
void SceneC::update() {
	time = getSharedData().time;

	glEnable(GL_DEPTH_TEST);

	// Render Geometry to G-Buffer
	gFbo.begin();
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
	gFbo.end();

	// Render ray marching
	gFbo.begin();
	gFbo.activateAllDrawBuffers();
	cam.begin();

	rayShader.begin();
	rayShader.setUniform1f("time", time);
	rayShader.setUniformMatrix4f("view", view);
	rayShader.setUniform2f("resolution", gFbo.getWidth(), gFbo.getHeight());
	rayShader.setUniformMatrix4f("projection", projection);
	rayShader.setUniform3f("camPos", cam.getGlobalPosition());
	rayShader.setUniform3f("camUp", cam.getUpDir());
	rayShader.setUniform1f("fov", cam.getFov());
	rayShader.setUniform1f("farClip", cam.getFarClip());
	rayShader.setUniform1f("nearClip", cam.getNearClip());

	quad.draw(OF_MESH_FILL);

	rayShader.end();
	cam.end();
	gFbo.end();

	glDisable(GL_DEPTH_TEST);

	getSharedData().post.begin();
	gFbo.getTexture(2).draw(0, 0);
	getSharedData().post.end();

	getSharedData().fbo.begin();
	getSharedData().post.draw();
	getSharedData().fbo.end();
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
string SceneC::getName() {
	return "SceneC";
}