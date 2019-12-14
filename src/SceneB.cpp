#include "SceneB.h"

// =========================================================================================
void SceneB::setup() {
	ofFbo::Settings rs; // render settings
	rs.width = ofGetWidth();
	rs.height = ofGetHeight();
	rs.useDepth = true;
	rs.useStencil = true;
	rs.depthStencilAsTexture = true;
	rs.textureTarget = GL_TEXTURE_2D;
	rs.numSamples = 16;
	renderFbo.allocate(rs);


	// ===== Boid settings =====
	wallSize = ofVec3f(128, 128, 128);
	attIdx = 0;
	box = ofBoxPrimitive(wallSize.x, wallSize.y, wallSize.z).getMesh();
	attractor = ofSpherePrimitive(1, 16);

	numFish = 1024 * 8;
	// texRes = int(sqrt(float(numFish)));
	piramid = createPiramid(ofRandom(1, 3));
	 //piramid = ofBoxPrimitive(5, 5, 5).getMesh();
	for (unsigned int i = 0; i < piramid.getVertices().size(); i++) {
		piramid.addColor(ofFloatColor(1, 1, 1, 0.5));
	}
	// Init boids position & vector 
	vector<ofVec3f> forces;
	forces.resize(numFish);
	boids.resize(numFish);
	int i = 0;
	for (auto& b : boids) {
		b.pos.x = ofRandom(-wallSize.x / 2, wallSize.x / 2);
		b.pos.y = ofRandom(-wallSize.y / 2, wallSize.y / 2);
		b.pos.z = ofRandom(-wallSize.z / 2, wallSize.z / 2);
		/*b.pos.x = ofRandom(-1, 1);
		b.pos.y = ofRandom(-1, 1);
		b.pos.z = ofRandom(-1, 1);*/
		b.vel.set(0, 0, 0);
	}
	for (auto& f : forces) {
		f.set(0, 0, 0);
	}
	// Create Buffer Object
	forceBuffer.allocate(forces, GL_DYNAMIC_DRAW);
	forceBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	dataBuffer.allocate(boids, GL_DYNAMIC_DRAW);
	dataBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);

	// Load shader
	forceCompute.loadCompute("shader/SceneB/forceCompute.glsl");
	integrate.loadCompute("shader/SceneB/compute.glsl");
	//instancingShader.load("shader/SceneB/instancing.vert", "shader/SceneB/instancing.frag");
	instancingShader.load("shader/SceneB/instancing.vert", "shader/SceneB/instancing.frag", "shader/SceneB/instancing.geom");

	ofEnableArbTex();
	posTex.allocate(numFish, 1, GL_RGBA32F, true);
	posTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	posTex.bindAsImage(0, GL_READ_WRITE);
	velTex.allocate(numFish, 1, GL_RGBA32F, true);
	velTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	velTex.bindAsImage(1, GL_READ_WRITE);
	forceTex.allocate(numFish, 1, GL_RGBA32F, true);
	forceTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	forceTex.bindAsImage(2, GL_READ_WRITE);
	ofDisableArbTex();

	// Init position of attractor
	for (int i = 0; i < 10; i++) {
		float x = ofRandom(-wallSize.x / 2, wallSize.x / 2);
		float y = ofRandom(-wallSize.y / 2, wallSize.y / 2);
		float z = ofRandom(-wallSize.z / 2, wallSize.z / 2);
		ofVec3f attPos = ofVec3f(x, y, z);
		attractorPoses.push_back(attPos);
	}

	// ===== GUI Settings ===== 
	gui.setup();
	gui.setPosition(getSharedData().gui.getWidth() + 10, 10);
	shaderUniforms.setName("shader params");
	shaderUniforms.add(separateRadius.set("separateRadius", 6.0, 1.0, 10));
	shaderUniforms.add(alignmentRadius.set("alignmentRadius", 2.0, 1.0, 10));
	shaderUniforms.add(cohesionRadius.set("cohesionRadius", 3.0, 1.0, 10));
	shaderUniforms.add(separateWeight.set("separateWeight", 3.0, 1.0, 10.0));
	shaderUniforms.add(alignmentWeight.set("alignmentWeight", 1.0, 1.0, 10.0));
	shaderUniforms.add(cohesionWeight.set("cohesionWeight", 1.0, 1.0, 10.0));
	shaderUniforms.add(maxSpeed.set("maxSpeed", 30, 0.1, 30));
	shaderUniforms.add(maxForce.set("maxForce", 5, 0.1, 30));
	shaderUniforms.add(avoidWallWeight.set("avoidWallWeight", 10.0, 1.0, 50.0));
	shaderUniforms.add(attractWeight.set("attractWeight", 10.0, 1.0, 30.0));
	gui.add(shaderUniforms);
	gui.add(fps.set("fps", 60, 0, 200));

	// Bloom Enable
	getSharedData().post[0]->setEnabled(true);
}

// =========================================================================================
void SceneB::update() {
	time = getSharedData().time;
	fps = ofGetFrameRate();

	/*if (getSharedData().volume > 0.7) {
		int ev = (int)ofRandom(0, 10);
		switch (ev) {
		case 0:
			attIdx = (int)ofRandom(0, 10);
			break;
	}*/
	if (getSharedData().volume > 0.7) {
		attIdx = (int)ofRandom(0, attractorPoses.size());
	}

	forceCompute.begin();
	forceCompute.setUniform1f("separateRadius", separateRadius);
	forceCompute.setUniform1f("alignmentRadius", alignmentRadius);
	forceCompute.setUniform1f("cohesionRadius", cohesionRadius);
	forceCompute.setUniform1f("separateWeight", separateWeight);
	forceCompute.setUniform1f("alignmentWeight", alignmentWeight);
	forceCompute.setUniform1f("cohesionWeight", cohesionWeight);
	forceCompute.setUniform1f("maxSpeed", maxSpeed);
	forceCompute.setUniform1f("maxForce", maxForce);
	forceCompute.setUniform1f("deltaTime", ofGetLastFrameTime());
	forceCompute.setUniform3f("wallSize", wallSize);
	forceCompute.setUniform3f("center", 0, 0, 0);
	forceCompute.setUniform1f("avoidWallWeight", avoidWallWeight);
	forceCompute.setUniform1i("maxNum", boids.size());
	forceCompute.setUniform3f("attractorPos", attractorPoses[attIdx]);
	forceCompute.setUniform1f("attractWeight", attractWeight);
	forceCompute.dispatchCompute((boids.size() + 1024 - 1) / 1024, 1, 1);
	forceCompute.end();

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	// Integrate Position & Velocity pass
	integrate.begin();
	integrate.setUniform1f("separateRadius", separateRadius);
	integrate.setUniform1f("alignmentRadius", alignmentRadius);
	integrate.setUniform1f("cohesionRadius", cohesionRadius);
	integrate.setUniform1f("separateWeight", separateWeight);
	integrate.setUniform1f("alignmentWeight", alignmentWeight);
	integrate.setUniform1f("cohesionWeight", cohesionWeight);
	integrate.setUniform1f("maxSpeed", maxSpeed);
	integrate.setUniform1f("maxForce", maxForce);
	integrate.setUniform1f("deltaTime", ofGetLastFrameTime());
	integrate.setUniform3f("wallSize", wallSize);
	integrate.setUniform3f("center", 0, 0, 0);
	integrate.setUniform1i("maxNum", boids.size());
	integrate.dispatchCompute((boids.size() + 1024 - 1) / 1024, 1, 1);
	integrate.end();

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	
	// Change camera position
	cam.setPosition(sin(time * 0.2) * wallSize.x * 0.5, cos(time * 0.4) * wallSize.y * 0.5, cos(time * 0.3) * wallSize.z * 0.5);
	//cam.lookAt(ofVec3f(0, 0, 0));
	cam.lookAt(attractorPoses[attIdx]);

	// render pass
	renderFbo.begin();

	// Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ofBackgroundGradient(ofColor(30, 0, 50), ofColor(127, 127, 127));

	cam.begin();
	glEnable(GL_DEPTH_TEST);

	instancingShader.begin();
	instancingShader.setUniformTexture("posTex", posTex, 0);
	instancingShader.setUniform3f("scale", 1, 1, 3);
	piramid.drawInstanced(OF_MESH_FILL, numFish);
	instancingShader.end();

	ofSetColor(255);
	box.drawWireframe();

	// Draw Attractor
	ofSetColor(255, 150);
	attractor.setScale(ofRandom(2, 8));
	attractor.setPosition(attractorPoses[attIdx]);
	attractor.draw();

	glDisable(GL_DEPTH_TEST);
	cam.end();
	// posTex.draw(100, 100, posTex.getWidth(), posTex.getHeight());
	// velTex.draw(200, 100, velTex.getWidth(), velTex.getHeight());
	// forceTex.draw(300, 100, forceTex.getWidth(), forceTex.getHeight());

	renderFbo.end();

	// Post effect pass
	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();

	// Render to Global fbo
	getSharedData().fbo.begin();
	getSharedData().post.draw(0, 0);
	getSharedData().fbo.end();
}

// =========================================================================================
void SceneB::draw() {
	getSharedData().fbo.draw(0, 0);
	getSharedData().gui.draw();
	gui.draw();
	/*posTex.draw(300, 100, 100, 100);
	velTex.draw(400, 100, 100, 100);
	forceTex.draw(500, 100, 100, 100);*/
}

// =========================================================================================
ofVboMesh SceneB::createPiramid(float scale) {
	ofVec3f v0 = ofVec3f(0, 0, 1) * scale; // front
	ofVec3f v1 = ofVec3f(sin(TWO_PI), cos(TWO_PI), 0) * scale;
	ofVec3f v2 = ofVec3f(sin(TWO_PI / 3), cos(TWO_PI / 3), 0) * scale;
	ofVec3f v3 = ofVec3f(sin(TWO_PI * 2 / 3), cos(TWO_PI * 2 / 3), 0) * scale;

	ofVboMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	mesh.addVertex(v0);
	mesh.addVertex(v1);
	mesh.addVertex(v2);

	mesh.addVertex(v0);
	mesh.addVertex(v3);
	mesh.addVertex(v1);

	mesh.addVertex(v0);
	mesh.addVertex(v2);
	mesh.addVertex(v3);

	mesh.addVertex(v1);
	mesh.addVertex(v3);
	mesh.addVertex(v2);

	return mesh;
}

// =========================================================================================
void SceneB::keyPressed(int key) {
	switch (key) {
	case 'a':
		attIdx = (int)ofRandom(0, attractorPoses.size());
	}
}

// =========================================================================================
string SceneB::getName() {
	return "SceneB";
}