#pragma once

#include "ofxPostProcessing.h"
#include "ofxGui.h"

class myCamera : public ofEasyCam {
private:
	float shakeTime;
	ofVec3f beforeShakePosition, nowPosition;
	ofTimer timer;
public:
	void customSetPosition(ofVec3f pos, ofVec3f lookPos) {
		if (shakeTime > 0.0f) {
			float x = ofRandom(-20, 20);
			float y = ofRandom(-20, 20);
			this->setPosition(pos + ofVec3f(x, y, 0));
			this->lookAt(lookPos + ofVec3f(x, y, 0));
			shakeTime -= 0.01;
		}
		else {
			this->setPosition(pos);
			this->lookAt(lookPos);
		}
	}
	void setBeforePos(ofVec3f beforePos) { beforeShakePosition = beforePos; }
	void setShakeTime(float time) { shakeTime = time; }
};

class SharedData {
public:
	ofxPostProcessing post;
	ofFbo fbo;
	ofxPanel gui;
	float time, fps;

	// sound
	float volume;
	bool isKicked;
	/*
	float *fftSmoothed;
	float *volume;
	int nBandsToGet;
	ofSoundPlayer sound;*/
};
