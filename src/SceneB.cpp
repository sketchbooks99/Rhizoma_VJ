#include "SceneB.h"

// =========================================================================================
void SceneB::setup() {
	// Post effect settings

	ofFbo::Settings rs; // render settings
	rs.width = ofGetWidth();
	rs.height = ofGetHeight();
	rs.useDepth = true;
	rs.useStencil = true;
	rs.depthStencilAsTexture = true;
	rs.textureTarget = GL_TEXTURE_2D;
	//rs.numSamples = 16;
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

	// Init curl noise buffer
	vector<Lifetime> lifetimes;
	lifetimes.resize(numFish);
	i = 0;
	for (auto& l : lifetimes) {
		l.age = 0.0;
		//l.maxAge = ofRandom();
		/*if (i % 2 == 0) l.maxAge = 1.0;
		else l.maxAge = 1.5;*/
		l.maxAge = ofRandom(1.0, 3.0);
		i++;
	}
	lifeBuffer.allocate(lifetimes, GL_DYNAMIC_DRAW);
	lifeBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 2);

	// Load shader
	forceCompute.loadCompute("shader/SceneB/forceCompute.glsl");
	integrate.loadCompute("shader/SceneB/compute.glsl");
	curlCompute.loadCompute("shader/SceneB/curlNoise.glsl");
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

	// Camera positions
	camRadiuses.push_back(wallSize / 2);
	timeOffsets.push_back(ofVec3f(0.2));
	for(int i = 0; i < 3; i++) {
		ofVec3f radius;
		/*radius.x = ofRandom(wallSize.x / 2, wallSize.x * 2);
		radius.y = ofRandom(wallSize.y / 2, wallSize.y * 2);
		radius.z = ofRandom(wallSize.z / 2, wallSize.z * 2);*/
		radius.x = ofRandom(wallSize.x, wallSize.x * 3);
		radius.y = ofRandom(wallSize.y, wallSize.y * 3);
		radius.z = ofRandom(wallSize.z, wallSize.z * 3);
		camRadiuses.push_back(radius);

		ofVec3f offset;
		offset.x = ofRandom(-0.7, 0.7);
		offset.y = ofRandom(-0.7, 0.7);
		offset.z = ofRandom(-0.7, 0.7);
		timeOffsets.push_back(offset);
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
	getSharedData().bloom->setEnabled(true);
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
	switch(sceneMode) {
	case 0:
		forceBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
		forceBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
		dataBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
		dataBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);

		scene1();

		dataBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 1);
		dataBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
		forceBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
		forceBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
		break;
	case 1:
		dataBuffer.bind(GL_SHADER_STORAGE_BUFFER);
		dataBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);
		lifeBuffer.bind(GL_SHADER_STORAGE_BUFFER);
		lifeBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 2);
		scene2();
		lifeBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 2);
		lifeBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
		dataBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 1);
		dataBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
		break;
	}

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
}

// =========================================================================================
void SceneB::scene1() {
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
	//ofBackgroundGradient(ofColor(30, 0, 50), ofColor(127, 127, 127));
	ofBackground(ofColor(0));

	cam.begin();
	glEnable(GL_DEPTH_TEST);

	ofMatrix4x4 model;
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 projection = cam.getProjectionMatrix();
	ofMatrix4x4 mvpMatrix = model * view * projection;
	ofMatrix4x4 invMatrix = mvpMatrix.getInverse();

	instancingShader.begin();
	instancingShader.setUniformTexture("posTex", posTex, 0);
	instancingShader.setUniform1i("isInvert", getSharedData().invert->getEnabled());
	instancingShader.setUniform3f("scale", ofVec3f(1,1,3) * max(0.7, getSharedData().volume * 10.0));
	instancingShader.setUniform1i("numFish", numFish);
	instancingShader.setUniformMatrix4f("invMatrix", invMatrix);
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

	renderFbo.end();

	// Post effect pass
	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

// =========================================================================================
void SceneB::scene2() {

	// compute curl Noise
	curlCompute.begin();
	curlCompute.setUniform1f("time", time);
	curlCompute.setUniform1f("timestep", 0.01);
	curlCompute.setUniform1f("scale", 0.005);
	curlCompute.dispatchCompute((boids.size() + 1024 - 1) / 1024, 1, 1);
	curlCompute.end();

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	cam.setPosition(
		camRadiuses[camIdx].x * sin(time * timeOffsets[camIdx].x), 
		camRadiuses[camIdx].y * cos(time * timeOffsets[camIdx].y), 
		camRadiuses[camIdx].z * cos(time * timeOffsets[camIdx].z)
	);
	cam.lookAt(ofVec3f(0, 0, 0));

	ofMatrix4x4 model;
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 projection = cam.getProjectionMatrix();
	ofMatrix4x4 mvpMatrix = model * view * projection;
	ofMatrix4x4 invMatrix = mvpMatrix.getInverse();

	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ofBackground(0);
	glEnable(GL_DEPTH_TEST);

	cam.begin();

	instancingShader.begin();
	instancingShader.setUniformTexture("posTex", posTex, 0);
	instancingShader.setUniform1i("isInvert", getSharedData().invert->getEnabled());
	instancingShader.setUniform3f("scale", ofVec3f(1,1,3) * max(1.0, getSharedData().volume * 10.0));
	instancingShader.setUniform1i("numFish", numFish);
	instancingShader.setUniformMatrix4f("invMatrix", invMatrix);
	piramid.drawInstanced(OF_MESH_FILL, numFish);
	instancingShader.end();

	cam.end();
	glDisable(GL_DEPTH_TEST);

	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

// =========================================================================================
void SceneB::scene3() {

}

void SceneB::scene4() {

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
		if (sceneMode == 0) attIdx = (int)ofRandom(0, attractorPoses.size());
		else if (sceneMode == 1) camIdx = (int)ofRandom(0, 4);
		break;
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

// =========================================================================================
string SceneB::getName() {
	return "SceneB";
}