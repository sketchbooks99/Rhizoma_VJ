#pragma once

#include "ofxState.h"
#include "SharedData.h"
#include "ofxGui.h"
//class myCamera : public ofEasyCam{
//private:
//	float shakeTime;
//	ofVec3f beforeShakePosition, nowPosition;
//	ofTimer timer;
//public:
//	void customSetPosition(ofVec3f pos, ofVec3f lookPos) {
//		if (shakeTime > 0.0f) {
//			float x = ofRandom(-20, 20);
//			float y = ofRandom(-20, 20);
//			this->setPosition(pos + ofVec3f(x, y, 0));
//			this->lookAt(lookPos + ofVec3f(x, y, 0));
//			shakeTime -= 0.01;
//		}
//		else {
//			this->setPosition(pos);
//			this->lookAt(lookPos);
//		}
//	}
//	void setBeforePos(ofVec3f beforePos) { beforeShakePosition = beforePos; }
//	void setShakeTime(float time) { shakeTime = time; }
//};

struct Circle {
public:
	Circle(unsigned int _res, unsigned int _size, ofVec3f _pos, ofColor _color, bool _isBright = false) {
		res = _res;
		size = _size;
		initPos = _pos;
		nowPos = _pos;
		isBright = _isBright;
		color = _color;
		intensity = 0;
		dir = ofVec2f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0)).normalized();
		initSpeed = nowSpeed = ofRandom(2, 5);
		buf = ofVec2f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0));
		speedUpTime = 0;

		inner.setCircleResolution(_res);
		inner.setColor(_color);
		inner.circle(ofVec3f(0, 0, 0), size - 7);

		c.setCircleResolution(_res);
		c.setColor(ofColor(_color.r, _color.g, _color.b, 255));
		c.arc(ofVec3f(0, 0, 0), size, size, 0, 360);
		c.arc(ofVec3f(0, 0, 0), size - 7, size - 7, 0, 360);
	}

	void resetPos(ofVec3f pos) {
		initPos = pos;
		nowPos = pos;
	}

	void update(float time) {
		if (speedUpTime > 0) {
			speedUpTime -= 0.01;
			nowSpeed = initSpeed * 10.0;
			float x = nowPos.x + dir.x * 2.0f * ofRandom(-nowSpeed, nowSpeed);
			float y = nowPos.y + dir.y * 2.0f * ofRandom(-nowSpeed, nowSpeed);
			nowPos = !isBright ? ofVec3f(x, y, nowPos.z) : nowPos;
		} else {
			float x = nowPos.x + dir.x * nowSpeed * ofNoise(time + buf.x);
			float y = nowPos.y + dir.y * nowSpeed * ofNoise(time + buf.y);
			nowSpeed = initSpeed;
			nowPos = !isBright ? ofVec3f(x, y, nowPos.z) : nowPos;
		}
	}

	void draw() {
		/*inner.draw();
		c.draw();*/
		ofPushMatrix();
		ofTranslate(nowPos);
		c.draw();
		inner.draw();
		ofPopMatrix();
	}

	void setSpeed(float _speed) { initSpeed = _speed; }
	float getSpeed() { return initSpeed; }

	void setBright(bool _isBright) { isBright = _isBright; }
	bool getBright() { return isBright; }

	void setIntensity(float _intensity) { intensity = _intensity; }
	float getIntensity() { return intensity; }

	void speedUp(float time) { speedUpTime = time; }

	void setSize(float _size) { size = _size; }
	float getSize() { return size; }

	void setLifetime(float _time) { lifetime = _time; }
	float getLifetime() { return lifetime; }

private:

	unsigned int size, res;
	ofVec2f dir, buf;
	float initSpeed, nowSpeed, speedUpTime;
	bool isBright;
	ofVec3f initPos, nowPos;
	ofPath c, inner;
	ofColor color;
	float intensity;
	float lifetime;
};

class SceneA : public itg::ofxState<SharedData> {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	string getName();

	struct Lifetime {
		float age;
		float maxAge;
	};

	struct Particle {
		ofVec3f pos;
		ofVec3f vel;
	};

	struct rect {
		ofVec2f pos;
		ofVec2f vel;

		void update() { pos += vel; }
		void draw() { ofDrawRectangle(pos, 200, 100); }
	};

private:

	void scene1();
	void scene2();
	void scene3();
	void scene4();
	void scene5();
	void soundScene();
	//void scene5();
	void sceneBurst();

	void drawWall();

	ofFbo renderFbo, normalFbo, occludeFbo, volumetricFbo, soundFbo;
	ofShader renderShader, volumetricShader, planeShader, wallShader, sphereShader, boxShader, particleCompute, renderParticle;

	ofxPanel gui;
	ofxToggle isColored, isPixeled, isReactive, isLayer;

	// camera
	myCamera cam;
	vector<ofVec3f> camRadiuses;
	vector<ofVec3f> timeOffsets;
	int camIdx;
	int attIdx;

	vector<ofVec3f> scenePoint;
	vector<ofColor> circleColor;
	vector<ofVec3f> circleCamPosition;
	int circleCamIdx, scene5Idx = 0;
	ofxToggle isSpawn, isSpawnParticle;
	int numCircle = 0;
	ofShader particleCompute1;

	float time;
	int sceneMode, childScene;
	bool sceneArray[5][5];
	vector<rect> rects;
	
	ofVboMesh plane, audioPlane, wall, box, sphere, particle;
	float planeHeight;
	float wallOffset = 1.0f;

	// Particle 
	vector<ofVec3f> spawnPoints;
	ofBufferObject spawnBuffer, positionBuffer, lifeBuffer;
};