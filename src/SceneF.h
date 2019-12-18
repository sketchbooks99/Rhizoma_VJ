#pragma once

#include "ofxState.h"
#include "SharedData.h"

struct FCircle {
public:
	FCircle(unsigned int _res, unsigned int _size, ofVec3f _pos, ofVec3f _dir) {
		res = _res;
		size = _size;
		pos = _pos;
		dir = _dir;

		c.setCircleResolution(_res);
		c.setColor(ofColor(255, 255, 255, 255));
		c.arc(ofVec3f(0, 0, 0), size, size, 0, 360);
		c.arc(ofVec3f(0, 0, 0), size - 50, size - 50, 0, 360);

		nowRotate = 0;
	}

	void update() {
		pos += dir * speed;
		if (rotateTime > 0.0) rotateTime -= ofGetLastFrameTime();
		else rotateTime = 0.0;
	}

	void draw(float time) {
		ofPushMatrix();
		ofTranslate(pos);
		// ofRotateXDeg(90);
		if (rotateTime > 0.0)
			nowRotate += time * rotSpeed;
		ofRotateZRad(nowRotate);
		c.draw();
		ofPopMatrix();
	}

	void setSpeed(float _speed) { speed = _speed; }
	float getSpeed() { return speed; }

	void setPosition(ofVec3f _pos) { pos = _pos;}
	ofVec3f getPosition() { return pos;}

	void setDir(ofVec3f _dir) { dir = _dir;} 
	ofVec3f getDir() { return dir;}

	void setRotate(float _rotSpeed) { rotSpeed = _rotSpeed; rotateTime = 0.2f; }

	unsigned int size, res;
	float speed, rotateTime, rotSpeed, nowRotate;
	ofVec3f pos, vel, dir;
	ofPath c;
};

class SceneF : public itg::ofxState<SharedData> {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	string getName();

private:
	void scene1();
	void scene2();
	void scene3();
	void scene4();

    ofVboMesh terrain;
    ofShader terrainShader;
    myCamera cam;
    float time;
    ofFbo renderFbo;
    int sceneMode;
	vector<FCircle> circles;
};
