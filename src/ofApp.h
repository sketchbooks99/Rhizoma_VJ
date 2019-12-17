#pragma once

#include "ofMain.h"
#include "ofxStateMachine.h"
#include "SharedData.h"
#include "ofxPostProcessing.h"
#include "subApp.h"
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
		ofxToggle isBloom, isEdge, isDof, isGodray, isNoiseWarp, isPixelate, isRGBShift, isRimHighlight, isZoomBlur, isInvert, isGlitch, isRotate;
		ofParameter<float> sound, fps;
		ofxToggle isKicked;
		ofxPanel gui;

		// Sound
		ofSoundStream soundStream;
		float smoothedVol, curVol;
		float kickThreshold, minimumThreshold, decayRate;

		shared_ptr<subApp> sub;
};
