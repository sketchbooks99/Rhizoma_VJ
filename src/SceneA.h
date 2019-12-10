#pragma once

#include "ofxState.h"
#include "SharedData.h"
#include "ofxGui.h"

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
		speed = ofRandom(2, 5);
		buf = ofVec2f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0));

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
		if (!isBright) {
			float x = nowPos.x + ofNoise(time * buf.x) * dir.x * speed;
			float y = nowPos.y + ofNoise(time * buf.y) * dir.y * speed;
			nowPos = ofVec3f(x, y, nowPos.z);
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

	void setSpeed(float _speed) { speed = _speed; }
	float getSpeed() { return speed; }

	void setBright(bool _isBright) { isBright = _isBright; }
	bool getBright() { return isBright; }

	void setIntensity(float _intensity) { intensity = _intensity; }
	float getIntensity() { return intensity; }

private:

	unsigned int size, res;
	ofVec2f dir, buf;
	float speed;
	bool isBright;
	ofVec3f initPos, nowPos;
	//ofVboMesh outline, inner;
	ofPath c, inner;
	ofColor color;
	float intensity;
};

class SceneA : public itg::ofxState<SharedData> {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	string getName();

private:

	Circle spawnCircle(float xRange, float yRange, ofVec2f sizeRange, ofColor color, bool isBright);

	ofFbo renderFbo, occludeFbo, volumetricFbo;
	ofEasyCam cam;
	ofShader renderShader, volumetricShader;

	ofxPanel gui;
	ofParameter<float> density, weight, decay, exposure, screenY;

	// circles
	vector<Circle> circles;

	// camera
	vector<ofVec3f> camPoses;
	int camIdx;

	float* time;
};