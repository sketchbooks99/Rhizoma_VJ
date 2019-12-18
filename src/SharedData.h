#pragma once

#include "ofxPostProcessing.h"
#include "ofxGui.h"
#include "ofxBvh.h"

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
	BloomPass::Ptr bloom;
	EdgePass::Ptr edge;
	DofPass::Ptr dof;
	NoiseWarpPass::Ptr noiseWarp;
	PixelatePass::Ptr pixelate;
	RGBShiftPass::Ptr rgbShift;
	ZoomBlurPass::Ptr zoomBlur;
	GodRaysPass::Ptr godray;
	Invert::Ptr invert;
	Glitch::Ptr glitch;
	Rotate::Ptr rotate;
	ofFbo fbo;
	ofxPanel gui;
	float time, fps;

	// sound
	float volume;
	bool isKicked;
	vector<float> left;
	vector<float> right;
	vector<float> volHistory;
	ofBufferObject leftBuffer, rightBuffer;
	/*
	float *fftSmoothed;
	float *volume;
	int nBandsToGet;
	ofSoundPlayer sound;*/
};
