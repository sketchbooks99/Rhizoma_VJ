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
	soundFbo.allocate(rs);
	//rs.numSamples = 16;
	rs.numSamples = 4;
	renderFbo.allocate(rs);

	// Load shaders
	renderShader.load("shader/SceneA/render.vert", "shader/SceneA/render.frag");
	volumetricShader.load("shader/SceneA/volumetric.vert", "shader/SceneA/volumetric.frag");
	planeShader.load("shader/SceneA/plane.vert", "shader/SceneA/plane.frag");
	sphereShader.load("shader/SceneA/sphere.vert", "shader/SceneA/sphere.frag", "shader/SceneA/sphere.geom");
	wallShader.load("shader/SceneA/wallShader.vert", "shader/SceneA/wallShader.frag");// , "shader/SceneA/wallShader.geom");
	boxShader.load("shader/SceneA/box.vert", "shader/SceneA/box.frag", "shader/SceneA/box.geom");
	renderParticle.load("shader/SceneA/renderParticle.vert", "shader/SceneA/renderParticle.frag");
	particleCompute.loadCompute("shader/SceneA/compute.glsl");
	particleCompute1.loadCompute("shader/SceneA/compute1.glsl");

	// Init custom GUI
	gui.setup();
	gui.setPosition(getSharedData().gui.getWidth() + 10, 10);
	gui.add(isPixeled.setup("isPixeled", false));
	gui.add(isColored.setup("isColored", false));
	gui.add(isReactive.setup("isReactive", false));
	gui.add(isLayer.setup("isLayer", false));
	gui.add(isSpawn.setup("isSpawn", false));
	gui.add(isSpawnParticle.setup("isSpawnParticle", false));

	// Geometry settings
	int xRes = 300, yRes = 300;
	plane = ofPlanePrimitive(300, 300, xRes, yRes).getMesh();
	for (int x = 0; x < xRes; x++) {
		for (int y = 0; y < yRes; y++) {
			plane.addTexCoord(ofVec2f(x, y) / xRes);
			//plane.addColor(ofFloatColor(0.4, 0.8, 0.8, 0.6));
			plane.addColor(ofFloatColor(1.0, 1.0, 1.0, 0.5));
		}
	}
	wall = ofBoxPrimitive(1, 10, 1).getMesh();

	// audio plane
	int bufferSize = getSharedData().left.size();
	//audioPlane = ofPlanePrimitive(300, 300, bufferSize, bufferSize).getMesh();

	// Sphere man
	sphere = ofIcoSpherePrimitive(16, 2).getMesh();

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (j == 0) sceneArray[i][j] = true;
			else sceneArray[i][j] = false;
		}
	}

	// General Settings
	time = getSharedData().time;
	camIdx = 0;
	attIdx = 0;
	sceneMode = 0;
	cam.setDistance(150);

	camRadiuses.push_back(ofVec3f(60.0));
	timeOffsets.push_back(ofVec3f(0.4));
	for (int i = 0; i < 4; i++) {
		ofVec3f radius;
		radius.x = ofRandom(50, 150);
		radius.y = ofRandom(50, 150);
		radius.z = ofRandom(50, 150);
		camRadiuses.push_back(radius);

		ofVec3f offset;
		offset.x = ofRandom(-1.5, 1.5);
		offset.y = ofRandom(-1.5, 1.5);
		offset.z = ofRandom(-1.5, 1.5);
		timeOffsets.push_back(offset);
	}

	// center
	float offsetY = 700;
	float offsetZ = -200;
	float x = 0;
	float y = 0;
	scenePoint.push_back(ofVec3f(x, y, 0.0));
	spawnPoints.push_back(ofVec3f(x, 0.0, y));
	circleCamPosition.push_back(ofVec3f(x, y + offsetY, offsetZ));
	x = ofRandom(-ofGetWidth()/2, 0);
	y = ofRandom(-ofGetHeight()/2, 0);
	scenePoint.push_back(ofVec3f(x, y, 0.0));
	spawnPoints.push_back(ofVec3f(x, 0.0, y));
	circleCamPosition.push_back(ofVec3f(x, y + offsetY, offsetZ));
	x = ofRandom(-ofGetWidth() / 2, 0);
	y = ofRandom(0, ofGetHeight());
	scenePoint.push_back(ofVec3f(x, y, 0.0));
	spawnPoints.push_back(ofVec3f(x, 0.0, y));
	circleCamPosition.push_back(ofVec3f(x, y + offsetY, offsetZ));
	x = ofRandom(0, ofGetWidth() / 2);
	y = ofRandom(0, ofGetHeight() / 2);
	scenePoint.push_back(ofVec3f(x, y, 0.0));
	spawnPoints.push_back(ofVec3f(x, 0.0, y));
	circleCamPosition.push_back(ofVec3f(x, y + offsetY, offsetZ));
	x = ofRandom(0, ofGetWidth() / 2);
	y = ofRandom(-ofGetHeight()/2, 0);
	scenePoint.push_back(ofVec3f(x, y, 0.0));
	spawnPoints.push_back(ofVec3f(x, 0.0, y));
	circleCamPosition.push_back(ofVec3f(x, y + offsetY, offsetZ));
	circleColor.push_back(ofColor(255, 255, 100));
	circleColor.push_back(ofColor(30, 100, 255, 100));
	circleColor.push_back(ofColor(100, 255, 30, 100));
	circleColor.push_back(ofColor(30, 255, 100, 100));
	circleColor.push_back(ofColor(100, 30, 255, 100));
	numCircle = 1;
	circleCamIdx = 0;

	// Prepare to particle
	int numParticlePerSpawn = 4096;
	spawnBuffer.allocate(spawnPoints, GL_DYNAMIC_DRAW);
	spawnBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	// Position and Velocity data for particle
	vector<Particle> bufferData;
	bufferData.resize(numParticlePerSpawn* spawnPoints.size());
	for (auto& b : bufferData) {
		/*b.pos.x = 0.0;
		b.pos.y = 0.0;
		b.pos.z = 0.0;*/
		b.pos.x = ofRandom(-1.0, 1.0);
		b.pos.y = ofRandom(-1.0, 1.0);
		b.pos.z = ofRandom(-1.0, 1.0);

		b.vel.x = 0.0;
		b.vel.y = 0.0;
		b.vel.z = 0.0;
	}
	positionBuffer.allocate(bufferData, GL_DYNAMIC_DRAW);
	positionBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);

	//particle.setMode(OF_PRIMITIVE_POINTS);
	for (int i = 0; i < bufferData.size(); i++) {
		particle.addVertex(ofVec3f(0, 0, 0));
		particle.addTexCoord(ofVec2f(i, 0));
		particle.addColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
	}

	vector<Lifetime> lifetimes;
	lifetimes.resize(numParticlePerSpawn* spawnPoints.size());
	for (auto& l : lifetimes) {
		l.maxAge = (int)ofRandom(1.0, 3.0);
		l.age = 0.0;
	}
	lifeBuffer.allocate(lifetimes, GL_DYNAMIC_DRAW);
	lifeBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 2);
}

// =========================================================================================
void SceneA::update() {
	// Scene update
	time = getSharedData().time;

	if (getSharedData().isKicked) planeHeight = 10.0;
	else planeHeight -= 0.1;

	if (sceneMode == 0) {
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
		scene4();
	}
	else if (sceneMode == 4) {
		scene5();
	}

	getSharedData().fbo.begin();

	ofClear(0, 0, 0, 255);
	ofSetColor(255, 255, 255);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	soundFbo.draw(0, 0);
	//glBlendFunc(GL_ONE, GL_ONE);
	getSharedData().post.draw();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ofDisableBlendMode();

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

//-----------------------------------------------------------------------------------------
void SceneA::soundScene() {

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ofPushMatrix();
	ofTranslate(0, ofGetHeight() / 2);
	if (sceneArray[0][1]) {
		float seed = ofRandom(0.0, 10.0);
		if (seed > 9.0) {
			rect r;
			r.pos = ofVec2f(ofGetWidth() * 2.0, ofRandom(-ofGetHeight() / 2, ofGetHeight()));
			r.vel = ofVec2f(-300, 0);
			rects.push_back(r);
		}
		for (int i = 0; i < rects.size(); i++) {
			rects[i].update();
			rects[i].draw();
			if (rects[i].pos.x < -ofGetWidth() * 1.5) rects.erase(rects.begin() + i);
		}
	}
	if (sceneArray[0][2]) {
		for (int i = 0; i < 4; i++) {
			float seed = ofRandom(0.0, 10.0);
			if (seed > 9.0) {
				ofSetColor(ofRandom(0, 255));
				ofDrawRectangle(ofGetWidth() * i / 4, -ofGetHeight()/2, ofGetWidth() / 4, ofGetHeight());
			}
		}
	}
	if (sceneArray[0][3]) {
		float offsetX = ofGetWidth() / 50;
		ofSetColor(255);
		float preY = 0.0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 50; j++) {
				float x = offsetX * j;
				float y = ofNoise(time * 100.0 + i) * ofGetHeight() - ofGetHeight()/2;
				ofDrawLine(ofVec2f((j - 1) * offsetX, preY), ofVec2f(x, y));
				preY = y;
			}
		}
	}
	for (int i = 0; i < 20; i++) {
		ofSetColor(ofFloatColor(fmod(time * 3.0 + i * 0.01, 1.0)));
		ofDrawRectangle(i * 100, getSharedData().left[i] * 800, 30, getSharedData().left[i] * 5000.0f);
	}
	ofPopMatrix();
}

// =========================================================================================
void SceneA::scene1() {
	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	soundScene();
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

// =========================================================================================
void SceneA::scene2() {
	cam.setPosition(
		camRadiuses[camIdx].x * sin(time * timeOffsets[camIdx].x),
		camRadiuses[camIdx].y * cos(time * timeOffsets[camIdx].y),
		camRadiuses[camIdx].z * cos(time * timeOffsets[camIdx].z)
	);
	cam.lookAt(ofVec3f(0, 0, 0));

	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	// render scene
	sphereShader.begin();
	sphereShader.setUniform1f("time", time);
	sphereShader.setUniform1f("volume", getSharedData().volume);
	sphereShader.setUniform3f("inColor", ofVec3f(0.2));
	sphereShader.setUniform3f("outColor", ofVec3f(0.0, 0.5, 1.0));
	sphere.draw(OF_MESH_FILL);
	sphereShader.end();

	glDisable(GL_DEPTH_TEST);
	cam.end();

	if(isLayer)
		soundScene();
	renderFbo.end();

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}
//------------------------------------------------------------------------------------------
void SceneA::scene3() {

	cam.setPosition(
		camRadiuses[camIdx].x * sin(time * timeOffsets[camIdx].x),
		camRadiuses[camIdx].y * cos(time * timeOffsets[camIdx].y),
		camRadiuses[camIdx].z * cos(time * timeOffsets[camIdx].z)
	);
	cam.lookAt(ofVec3f(0, 0, 0));

	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();
	glEnable(GL_DEPTH_TEST);
	
	// render scene
	ofPushMatrix();
	ofTranslate(0, 60, 0);
	ofRotateXDeg(90);

	planeShader.begin();
	planeShader.setUniform1f("intensity", 1.0);
	planeShader.setUniform1f("time", time);
	plane.draw(OF_MESH_POINTS);
	planeShader.end();
	ofPopMatrix();

	sphereShader.begin();
	sphereShader.setUniform1f("time", time);
	sphereShader.setUniform1f("volume", getSharedData().volume);
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

	cam.setPosition(
		camRadiuses[camIdx].x * sin(time * timeOffsets[camIdx].x) * 5.0,
		camRadiuses[camIdx].y * cos(time * timeOffsets[camIdx].y) * 5.0,
		camRadiuses[camIdx].z * cos(time * timeOffsets[camIdx].z) * 5.0
	);
	/*cam.setPosition(300, 300, 300);*/
	cam.lookAt(ofVec3f(0, 0, 0));

	spawnBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	spawnBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	positionBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	positionBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 1);
	lifeBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	lifeBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 2);

	particleCompute.begin();
	particleCompute.setUniform1f("time", time);
	particleCompute.setUniform1f("timestep", 0.01);
	particleCompute.setUniform1f("scale", 0.02);
	particleCompute.setUniform1i("isSpawn", isSpawn);
	particleCompute.dispatchCompute((positionBuffer.size() + 1024 - 1) / 1024, 1, 1);
	particleCompute.end();

	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	ofMatrix4x4 model;
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 projection = cam.getProjectionMatrix();
	ofMatrix4x4 mvpMatrix = model * view * projection;

	renderParticle.begin();
	renderParticle.setUniform1f("time", time);
	renderParticle.setUniform3f("pointColor", ofVec3f(-1.0, 0.0, 0.0));
	particle.draw(OF_MESH_POINTS);
	if (sceneArray[sceneMode][1])
		renderParticle.setUniform1i("isColored", true);
	else 
		renderParticle.setUniform1i("isColored", false);
	if (sceneArray[sceneMode][2])
		particle.draw(OF_MESH_WIREFRAME);
	renderParticle.end();

	if (isLayer)
		soundScene();

	glDisable(GL_DEPTH_TEST);
	cam.end();
	renderFbo.end();

	lifeBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 2);
	lifeBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
	positionBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 1);
	positionBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
	spawnBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
	spawnBuffer.unbind(GL_SHADER_STORAGE_BUFFER);

	getSharedData().post.begin();
	renderFbo.draw(0, 0);
	getSharedData().post.end();
}

//------------------------------------------------------------------------------------------
void SceneA::scene5() {

	if (scene5Idx == 0) {
		cam.setPosition(0, 0, ofGetHeight());
		cam.lookAt(ofVec3f(0, 0, 0));
	}
	else if (scene5Idx == 1 || scene5Idx == 2) {
		cam.setPosition(circleCamPosition[circleCamIdx]);
		cam.lookAt(scenePoint[circleCamIdx]);
		/*cam.setPosition(sin(time) * 1000, -100, cos(time) * 1000);
		cam.lookAt(ofVec3f(0, 0, 0));*/
	}

	renderFbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.begin();
	glEnable(GL_DEPTH_TEST);

	ofMatrix4x4 model;
	ofMatrix4x4 view = ofGetCurrentViewMatrix();
	ofMatrix4x4 projection = cam.getProjectionMatrix();
	ofMatrix4x4 mvpMatrix = model * view * projection;
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	for (int i = 0; i < numCircle; i++) {
		for (int j = 0; j < 5 + (int)(getSharedData().left[i] * 300.0 * (numCircle + 1)); j++) {
			ofNoFill();
			ofSetColor((circleColor[i] + ofColor(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10))) * (0.2 * (numCircle + 1)));
			float x = sin(j * ofNoise(time * 300.0, j) * TWO_PI) * 20.0 + getSharedData().left[i] * 100.0;
			float radius = ofNoise(i * 0.01 + time, j * 0.1) * (100.0 + ((numCircle + 1) * 0.5 * 100));
			int seed = (int)ofRandom(0, 200);
			ofVec3f offsetedPos = scenePoint[i] + ofVec3f(x, 0, 0);
			ofDrawCircle(scenePoint[i] + ofVec3f(x, 0, 0), radius + getSharedData().left[i + seed] * 500.0);
			ofSetColor(200, 200, 200, 100);
			seed = (int)ofRandom(0, 10);
			ofSetLineWidth(5);
			if (seed > 3) ofDrawLine(offsetedPos, scenePoint[(int)ofRandom(0, numCircle)]);
			ofFill();
			ofSetColor(circleColor[i] + ofColor(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10)));
			ofDrawCircle(scenePoint[i] + ofVec3f(x, 0, 0) * 0.5, ofRandom(10, 30));
		}
	}

	if (isSpawn) {
		for (int i = 0; i < numCircle; i++) {
			ofPushMatrix();
			//ofTranslate(scenePoint[i] + ofVec3f(0, 0, -400));
			ofTranslate(scenePoint[i] + ofVec3f(sin(time * 0.378 + i) * 500, cos(time * 0.378 + i) * 500, 300));
			ofRotate(time * 30, (i+1) * 0.467 - (int)((i + 1) * 0.467), (i+1) * 0.378 - (int)((i + 1) * 0.467), (i+1) * 0.789 - (int)((i + 1) * 0.467));
			ofScale(4,4,4);
			sphereShader.begin();
			sphereShader.setUniform1f("time", time);
			sphereShader.setUniform1f("volume", getSharedData().volume);
			sphereShader.setUniform3f("inColor", (ofVec3f(1.0) - ofVec3f(circleColor[i].r/255, circleColor[i].g/255, circleColor[i].b / 255)) * 0.2);
			sphereShader.setUniform3f("outColor", ofVec3f(circleColor[i].r / 255, circleColor[i].g / 255, circleColor[i].b / 255));
			sphere.draw(OF_MESH_FILL);
			sphereShader.end();
			ofPopMatrix();
		}
	}

	glDisable(GL_DEPTH_TEST);
	cam.end();

	if (isLayer)
		soundScene();

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
		//if (sceneMode == 3) attIdx = (int)ofRandom(0, spawnPoints.size());
		if (sceneMode > 0) camIdx = (int)ofRandom(0, 4);
		circleCamIdx = (int)ofRandom(0, numCircle);
		break;
	case 's':
		isLayer = !isLayer;
		break;
	// Change child scene
	case 'd':
		isColored = !isColored;
		break;
	case 'f':
		numCircle = numCircle < 5 ? numCircle + 1 : 5;
		break;
	case 'z': // simple 
		sceneMode = 0;
		isColored = false;
		break;
	case 'x': // colored start scene
		sceneMode = 1;
		isColored = false;
		break;
	case 'c': // only sphere scene
		sceneMode = 2;
		isColored = false;
		break;
	case 'v':  // pop up cube
		sceneMode = 3;
		isColored = false;
		getSharedData().bloom->setStrength(1.5);
		break;
	case 'b':  // room
		sceneMode = 4;
		break;
	//case 'n': // geometry in cube
	//	sceneMode = 5;
	//	break;
	//case 'm': 
	//	sceneMode = 6;
		break;
	case 'g':
		sceneArray[sceneMode][0] = !sceneArray[sceneMode][0];
		scene5Idx = 0;
		break;
	case 'h':
		sceneArray[sceneMode][1] = !sceneArray[sceneMode][1];
		scene5Idx = 1;
		break;
	case 'j':
		sceneArray[sceneMode][2] = !sceneArray[sceneMode][2];
		scene5Idx = 2;
		break;
	case 'k':
		sceneArray[sceneMode][3] = !sceneArray[sceneMode][3];
		scene5Idx = 3;
		if (sceneMode == 4) isSpawn = !isSpawn;
		if (sceneMode == 5) isSpawnParticle = !isSpawnParticle;
		break;
	case 'l':
		sceneArray[sceneMode][4] = !sceneArray[sceneMode][4];
		scene5Idx = 4;
		break;
	}
}

// =========================================================================================
string SceneA::getName() {
	return "SceneA";
}