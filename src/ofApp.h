#pragma once

#include "ofMain.h"
#include "ofxStateMachine.h"
#include "SharedData.h"
#include "ofxPostProcessing.h"
#include "subApp.h"
#include "ofxBvh.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void audioIn(ofSoundBuffer & input);
		
		itg::ofxStateMachine<SharedData> stateMachine; 
		ofxToggle isBloom, isEdge, isNoiseWarp, isRGBShift, isZoomBlur, isInvert, isGlitch, isRotate, isBeyoon, isReflectX, isReflectY, isSplit;
		ofParameter<float> sound, fps, bloomStrength;
		ofxToggle isKicked, isZoomBlurReactive, isGlitchReactive;
		ofxPanel gui;

		// Sound
		ofSoundStream soundStream;
		float smoothedVol, curVol;
		float kickThreshold, minimumThreshold, decayRate;

		shared_ptr<subApp> sub;
};
