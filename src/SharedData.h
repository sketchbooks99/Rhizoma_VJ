#pragma once

#include "ofxPostProcessing.h"
#include "ofxGui.h"
#include "ofxBvh.h"
#include "Trail.hpp"

class PointLight {
public:
	PointLight() {
		sphere = ofSpherePrimitive(8, 8);
		velocity = ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
	}

	// color
	void setColor(float r, float g, float b) {
		color = ofVec3f(r, g, b);
	}
	void setColor(float _color) {
		color = ofVec3f(_color, _color, _color);
	}
	void setGray(float _gray) {
		gray = _gray;
	}
	void setColor(ofVec3f _color) {
		color = _color;
	}
	ofVec3f getColor() const { return color; }
	float getGray() const { return gray; }

	// position
	void setPosition(float x, float y, float z) {
		position = ofVec3f(x, y, z);
	}
	void setPosition(ofVec3f _position) {
		position = _position;
	}
	ofVec3f getPosition() const { return position; }

	void setVelocity(float x, float y, float z) {
		velocity = ofVec3f(x, y, z);
	}
	ofVec3f getVelocity() const { return velocity; }

	void setRadius(float _radius) {
		radius = _radius;
	}
	float getRadius() const { return radius; }

	void draw() {
		//        ofSetColor(color.x, color.y, color.z);
		ofPushMatrix();
		ofTranslate(position);
		sphere.draw();
		ofPopMatrix();
	}
private:
	ofVec3f position;
	ofVec3f color;
	float gray;
	ofVec3f velocity;
	float radius;

	ofSpherePrimitive sphere;
};

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
	Beyoon::Ptr beyoon;
	ReflectX::Ptr reflectX;
	ReflectY::Ptr reflectY;
	Split::Ptr split;
	
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
