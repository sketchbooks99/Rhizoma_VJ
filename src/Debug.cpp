#include "Debug.h"

using namespace glm;

//------------------------------------------------------------------
void Debug::setup() {
    plane = ofPlanePrimitive(300, 300, 10, 10).getMesh();
	for (int i = 0; i < plane.getVertices().size(); i++) {
		plane.addColor(ofFloatColor(1, 1, 1, 1));
	}
	box = ofBoxPrimitive(100, 100, 100).getMesh();
	for (int i = 0; i < box.getVertices().size(); i++) {
		box.addColor(ofFloatColor(1, 1, 1, 1));
	}

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

	cam.setFarClip(5000);
	//cam.setDistance(1000);
}

//------------------------------------------------------------------
void Debug::update() {


	ofBackground(0);
}

//------------------------------------------------------------------
void Debug::draw() {


	ofPushMatrix();
	ofRotateXDeg(90);
	ofTranslate(0, -10, 0);
	plane.draw(OF_MESH_FILL);
	box.draw(OF_MESH_FILL);
	ofPopMatrix();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	ofPushMatrix();
	ofRotateXDeg(90);
	ofTranslate(0, -10, 0);
	plane.draw(OF_MESH_FILL);
	ofPopMatrix();
	box.draw(OF_MESH_FILL);
	glDisable(GL_CULL_FACE);

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


