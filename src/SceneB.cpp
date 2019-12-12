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
	int numFish = 4096;
	int texRes = int(sqrt(float(numFish)));
	piramid = createPiramid(10);
	for (unsigned int i = 0; i < piramid.getVertices().size(); i++) {
		piramid.addColor(ofFloatColor(1, 1, 1, 0.5));
	}
	// Init boids position & vector 
	vector<ofVec3f> forces;
	forces.resize(numFish);
	boids.resize(numFish);
	int i = 0;
	for (auto& b : boids) {
		b.pos.x = ofRandom(-ofGetWidth(), ofGetWidth());
		b.pos.y = ofRandom(-ofGetHeight(), ofGetHeight());
		b.pos.z = ofRandom(-ofGetHeight(), ofGetHeight());
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
	instancingShader.load("shader/SceneB/instancing.vert", "shader/SceneB/instancing.frag");

	// Init textures for instancing boids
	posTex.allocate(texRes, texRes, GL_RGBA32F);
	posTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	posTex.bindAsImage(0, GL_READ_WRITE);
	velTex.allocate(texRes, texRes, GL_RGBA32F);
	velTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	velTex.bindAsImage(1, GL_READ_WRITE);
	forceTex.allocate(texRes, texRes, GL_RGBA32F);
	forceTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	forceTex.bindAsImage(2, GL_READ_WRITE);

	// ===== GUI Settings ===== 
	gui.setup();
	gui.setPosition(10, 10);
	shaderUniforms.setName("shader params");
	shaderUniforms.add(separateRadius.set("separateRadius", 2.0, 1.0, 10.0));
	shaderUniforms.add(alignmentRadius.set("alignmentRadius", 2.0, 1.0, 10.0));
	shaderUniforms.add(cohesionRadius.set("cohesionRadius", 1.0, 1.0, 10.0));
	shaderUniforms.add(separateWeight.set("separateWeight", 1.0, 1.0, 10.0));
	shaderUniforms.add(alignmentWeight.set("alignmentWeight", 1.0, 1.0, 10.0));
	shaderUniforms.add(cohesionWeight.set("cohesionWeight", 3.0, 1.0, 10.0));
	shaderUniforms.add(maxSpeed.set("maxSpeed", 5.0, 0.1, 10.0));
	shaderUniforms.add(maxForce.set("maxForce", 0.5, 0.1, 10.0));
	shaderUniforms.add(avoidWallWeight.set("avoidWallWeight", 10.0, 1.0, 30.0));
	gui.add(shaderUniforms);
	gui.add(fps.set("fps", 60, 0, 200));
}

// =========================================================================================
void SceneB::update() {
	fps = ofGetFrameRate();
	// Computing Force pass
	/*posTex.bindAsImage(0, GL_READ_WRITE);
	velTex.bindAsImage(1, GL_READ_WRITE);
	forceTex.bindAsImage(2, GL_READ_WRITE);
	forceBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	forceBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	dataBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	dataBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);*/

	forceCompute.begin();
	forceCompute.setUniforms(shaderUniforms);
	forceCompute.setUniform1f("separateRadius", separateRadius);
	forceCompute.setUniform1f("alignmentRadius", alignmentRadius);
	forceCompute.setUniform1f("cohesionRadius", cohesionRadius);
	forceCompute.setUniform1f("separateWeight", separateWeight);
	forceCompute.setUniform1f("alignmentWeight", alignmentWeight);
	forceCompute.setUniform1f("cohesionWeight", cohesionWeight);
	forceCompute.setUniform1f("maxSpeed", maxSpeed);
	forceCompute.setUniform1f("maxForce", maxForce);
	forceCompute.setUniform1f("deltaTime", ofGetLastFrameTime());
	forceCompute.setUniform3f("wallSize", ofGetWidth() * 2.0, ofGetHeight() * 2.0, ofGetHeight() * 2.0);
	forceCompute.setUniform3f("center", 0, 0, 0);
	forceCompute.setUniform1i("maxNum", boids.size());
	forceCompute.dispatchCompute((boids.size() + 1024 - 1) / 1024, 1, 1);
	forceCompute.end();

	// Integrate Position & Velocity pass
	/*integrate.begin();
	integrate.setUniform1f("separateRadius", separateRadius);
	integrate.setUniform1f("alignmentRadius", alignmentRadius);
	integrate.setUniform1f("cohesionRadius", cohesionRadius);
	integrate.setUniform1f("separateWeight", separateWeight);
	integrate.setUniform1f("alignmentWeight", alignmentWeight);
	integrate.setUniform1f("cohesionWeight", cohesionWeight);
	integrate.setUniform1f("maxSpeed", maxSpeed);
	integrate.setUniform1f("maxForce", maxForce);
	integrate.setUniform1f("deltaTime", ofGetLastFrameTime());
	integrate.setUniform3f("wallSize", ofGetWidth() * 2.0, ofGetHeight() * 2.0, ofGetHeight() * 2.0);
	integrate.setUniform3f("center", 0, 0, 0);
	integrate.setUniform1i("maxNum", boids.size());
	integrate.dispatchCompute((boids.size() + 1024 - 1) / 1024, 1, 1);
	integrate.end();*/

	/*posTex.unbind();
	velTex.unbind();
	forceTex.unbind();
	dataBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 1);
	dataBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
	forceBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
	forceBuffer.unbind(GL_SHADER_STORAGE_BUFFER);*/


	// render pass
	renderFbo.begin();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam.begin();
	glEnable(GL_DEPTH_TEST);

	instancingShader.begin();
	instancingShader.setUniformTexture("posTex", posTex, 0);
	piramid.drawInstanced(OF_MESH_FILL, boids.size());
	instancingShader.end();

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
}

// =========================================================================================
void SceneB::draw() {
	getSharedData().post.draw();
	gui.draw();
	posTex.draw(300, 100, posTex.getWidth(), posTex.getHeight());
	velTex.draw(400, 100, velTex.getWidth(), velTex.getHeight());
	forceTex.draw(500, 100, forceTex.getWidth(), forceTex.getHeight());
}

// =========================================================================================
ofVboMesh SceneB::createPiramid(float scale) {
	ofVec3f v0 = ofVec3f(0, -2, 0) * scale; // top
	ofVec3f v1 = ofVec3f(sin(TWO_PI), 0, cos(TWO_PI)) * scale;
	ofVec3f v2 = ofVec3f(sin(TWO_PI / 3), 0, cos(TWO_PI / 3)) * scale;
	ofVec3f v3 = ofVec3f(sin(TWO_PI * 2 / 3), 0, cos(TWO_PI * 2 / 3)) * scale;

	ofVboMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	mesh.addVertex(v0);
	mesh.addVertex(v2);
	mesh.addVertex(v1);

	mesh.addVertex(v0);
	mesh.addVertex(v3);
	mesh.addVertex(v1);

	mesh.addVertex(v0);
	mesh.addVertex(v2);
	mesh.addVertex(v3);

	mesh.addVertex(v1);
	mesh.addVertex(v2);
	mesh.addVertex(v3);

	return mesh;
}

// =========================================================================================
string SceneB::getName() {
	return "SceneB";
}