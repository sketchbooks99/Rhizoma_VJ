#include "Debug.h"

using namespace glm;

//------------------------------------------------------------------
void Debug::setup() {
    plane = ofPlanePrimitive(1,1, 10, 10).getMesh();
    box = ofBoxPrimitive(1,1,1).getMesh();

    createGBuffer();

	ofFbo::Settings ds;
	ds.width = ofGetWidth();
	ds.height = ofGetHeight();
	//ds.internalformat = GL_RGB;
	ds.internalformat = GL_RGBA;
	ds.textureTarget = GL_TEXTURE_2D;
	ds.useDepth = true;
	ds.depthStencilAsTexture = true;
	depthFbo.allocate(ds);

	depthShader.load("shader/Debug/depth.vert", "shader/Debug/depth.frag");
	depthRender.load("shader/Debug/passthru.vert", "shader/Debug/depthRender.frag");
	quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	quad.addVertex(ofVec3f(1.0, 1.0, 0.0)); // top-right
	quad.addTexCoord(ofVec2f(1.0, 1.0));
	quad.addVertex(ofVec3f(1.0, -1.0, 0.0)); //bottom-right
	quad.addTexCoord(ofVec2f(1.0, 0.0f));
	quad.addVertex(ofVec3f(-1.0, -1.0, 0.0)); //bottom-left
	quad.addTexCoord(ofVec2f(0.0f, 0.0f));
	quad.addVertex(ofVec3f(-1.0, 1.0, 0.0)); //top-left
	quad.addTexCoord(ofVec2f(0.0f, 1.0));

}

//------------------------------------------------------------------
void Debug::update() {
	vec3 lightInvDir = vec3(0.5,2,2);
	//vec3 lightInvDir = vec3(100, 200, 200);
	//mat4 depthProjectionMatrix = ortho(-10, 10, -10, 10, -10, 20);
	//mat4 depthProjectionMatrix = ortho<float>(-ofGetWidth() / 2, ofGetWidth() / 2, -ofGetHeight() / 2, ofGetHeight()/2, 0.1, 7.5);
	mat4 depthProjectionMatrix = ortho<float>(-10, 10, -10, 10, -10, 20);
	mat4 depthViewMatrix = glm::lookAt(lightInvDir, vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 depthSpaceMatrix = depthProjectionMatrix * depthViewMatrix;
	ofMatrix4x4 model;

	depthFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);

	//cam.begin();
	
	depthShader.begin();

	// Draw plane
	model.rotate(90, 1, 0, 0);
	//model.translate(0, 0, 0);
	depthShader.setUniformMatrix4f("depthSpaceMatrix", depthSpaceMatrix);
	depthShader.setUniformMatrix4f("model", model);
	plane.draw(OF_MESH_FILL);

	// Draw Box
	model = ofMatrix4x4();
	depthShader.setUniformMatrix4f("model", model);
	box.draw(OF_MESH_FILL);

	depthShader.end();

	//cam.end();
	depthFbo.end();
	//glDisable(GL_DEPTH_TEST);
}

//------------------------------------------------------------------
void Debug::draw() {
	depthRender.begin();
	depthRender.setUniformTexture("depth", depthFbo.getDepthTexture(), 0);
	quad.draw(OF_MESH_FILL);
	depthRender.end();

	getSharedData().gui.draw();

}

//------------------------------------------------------------------
void Debug::keyPressed(int key) {

}

//------------------------------------------------------------------
void Debug::scene1() {

}

//------------------------------------------------------------------
void Debug::scene2() {

}

//------------------------------------------------------------------
void Debug::scene3() {

}

//------------------------------------------------------------------
void Debug::scene4() {

}

//-----------------------------------------------------------------
void Debug::drawPrimitives() {
}

//-----------------------------------------------------------------
void Debug::createGBuffer() {

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

//------------------------------------------------------------------
string Debug::getName() {
    return "Debug";
}


