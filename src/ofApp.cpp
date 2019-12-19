#include "ofApp.h"
#include "SceneA.h"
#include "SceneB.h"
#include "SceneC.h"
#include "SceneD.h"
#include "SceneE.h"
#include "SceneF.h"
#include "Debug.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// Sound setup
	//soundStream.setup(this, 0, 2, 44100, 256);
	decayRate = 0.05;
	minimumThreshold = 0.05;
	kickThreshold = minimumThreshold;
	curVol = 0.0;
	smoothedVol = 0.0;
	stateMachine.getSharedData().volume = 0;
	ofSoundStreamSettings settings;
	auto devices = soundStream.getMatchingDevices("default");
	if (!devices.empty()) {
		settings.setInDevice(devices[0]);
	}
	settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 2;
	settings.bufferSize = 256;
	settings.setApi(ofSoundDevice::MS_DS);
	soundStream.setup(settings);

	stateMachine.getSharedData().left.assign(settings.bufferSize, 0.0);
	stateMachine.getSharedData().right.assign(settings.bufferSize, 0.0);

	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	//ofEnableAntiAliasing();

	// GUI Settings
	stateMachine.getSharedData().gui.setup();
	stateMachine.getSharedData().gui.setPosition(10, 10);
	stateMachine.getSharedData().gui.add(isBloom.setup("Bloom", false));
	stateMachine.getSharedData().gui.add(isEdge.setup("Edge", false));
	stateMachine.getSharedData().gui.add(isNoiseWarp.setup("NoiseWarp", false));
	stateMachine.getSharedData().gui.add(isRGBShift.setup("RGBShift", false));
	stateMachine.getSharedData().gui.add(isZoomBlur.setup("ZoomBlur", false));
	stateMachine.getSharedData().gui.add(isInvert.setup("Invert", false));
	stateMachine.getSharedData().gui.add(isGlitch.setup("Glitch", false));
	stateMachine.getSharedData().gui.add(isBeyoon.setup("Beyoon", false));
	stateMachine.getSharedData().gui.add(isReflectX.setup("ReflectX", false));
	stateMachine.getSharedData().gui.add(isReflectY.setup("ReflectY", false));
	stateMachine.getSharedData().gui.add(isSplit.setup("Split", false));
	stateMachine.getSharedData().gui.add(sound.set("Sound", 0, 0, 1));
	stateMachine.getSharedData().gui.add(fps.set("fps", 60, 0, 60));
	stateMachine.getSharedData().gui.add(isKicked.setup("isKick", false));
	stateMachine.getSharedData().gui.add(bloomStrength.set("bloom", 0.0, 0.0, 5.0));
	stateMachine.getSharedData().gui.add(isZoomBlurReactive.setup("isZoomBlurReacitive", true));

	// Post Effect settings
	stateMachine.getSharedData().post.init(ofGetWidth(), ofGetHeight());
	stateMachine.getSharedData().post.setFlip(false);
	stateMachine.getSharedData().bloom = stateMachine.getSharedData().post.createPass<BloomPass>();
	stateMachine.getSharedData().bloom->setEnabled(isBloom);
	stateMachine.getSharedData().bloom->setStrength(1.0f);
	stateMachine.getSharedData().edge = stateMachine.getSharedData().post.createPass<EdgePass>();
	stateMachine.getSharedData().edge->setEnabled(isEdge);
	stateMachine.getSharedData().noiseWarp = stateMachine.getSharedData().post.createPass<NoiseWarpPass>();
	stateMachine.getSharedData().noiseWarp->setEnabled(isNoiseWarp);
	stateMachine.getSharedData().rgbShift = stateMachine.getSharedData().post.createPass<RGBShiftPass>();
	stateMachine.getSharedData().rgbShift->setEnabled(isRGBShift);
	stateMachine.getSharedData().zoomBlur = stateMachine.getSharedData().post.createPass<ZoomBlurPass>();
	stateMachine.getSharedData().zoomBlur->setEnabled(isZoomBlur);
	stateMachine.getSharedData().invert = stateMachine.getSharedData().post.createPass<Invert>();
	stateMachine.getSharedData().invert->setEnabled(isInvert);
	stateMachine.getSharedData().glitch = stateMachine.getSharedData().post.createPass<Glitch>();
	stateMachine.getSharedData().glitch->setEnabled(isGlitch);
	stateMachine.getSharedData().beyoon = stateMachine.getSharedData().post.createPass<Beyoon>();
	stateMachine.getSharedData().beyoon->setEnabled(isBeyoon);
	stateMachine.getSharedData().reflectX = stateMachine.getSharedData().post.createPass<ReflectX>();
	stateMachine.getSharedData().reflectX->setEnabled(isReflectX);
	stateMachine.getSharedData().reflectY = stateMachine.getSharedData().post.createPass<ReflectY>();
	stateMachine.getSharedData().reflectY->setEnabled(isReflectY);
	stateMachine.getSharedData().split = stateMachine.getSharedData().post.createPass<Split>();
	stateMachine.getSharedData().split->setEnabled(isSplit);

	// Allocate FBO
	stateMachine.getSharedData().fbo.allocate(ofGetWidth(), ofGetHeight());

	// Add scenes
	stateMachine.addState<SceneA>();
	stateMachine.addState<SceneB>();
	stateMachine.addState<SceneC>();
	stateMachine.addState<SceneD>();
	stateMachine.addState<SceneE>();
	stateMachine.addState<SceneF>();
	//stateMachine.addState<Debug>();
	 stateMachine.changeState("SceneA");
	// stateMachine.changeState("SceneB");
	//stateMachine.changeState("SceneC");
	// stateMachine.changeState("SceneD");
	// stateMachine.changeState("SceneE");
	// stateMachine.changeState("SceneF");
}

//--------------------------------------------------------------
void ofApp::update(){
	stateMachine.getSharedData().time = ofGetElapsedTimef();
	bloomStrength = stateMachine.getSharedData().bloom->getStrength();

	sub->fbo = stateMachine.getSharedData().fbo;

	sound = stateMachine.getSharedData().volume;

	stateMachine.getSharedData().rgbShift->setAmount(std::min(stateMachine.getSharedData().volume * 0.1f, 0.01f));
	stateMachine.getSharedData().zoomBlur->setWeight(std::min(stateMachine.getSharedData().volume * 10.0f, 0.3f));
	if (stateMachine.getSharedData().beyoon->getEnabled()) {
		float seed = ofRandom(0, 10);
		if (seed > 7.0) {
			stateMachine.getSharedData().beyoon->setOffset(ofVec2f(ofRandom(-0.7, 0.7), ofRandom(-0.8, 0.8)));
		}
	}
	fps = ofGetFrameRate();

	isBloom = stateMachine.getSharedData().bloom->getEnabled();
	isEdge = stateMachine.getSharedData().edge->getEnabled();
	isNoiseWarp = stateMachine.getSharedData().noiseWarp->getEnabled();
	isRGBShift = stateMachine.getSharedData().rgbShift->getEnabled();
	isZoomBlur = stateMachine.getSharedData().zoomBlur->getEnabled();
	isInvert = stateMachine.getSharedData().invert->getEnabled();
	isGlitch = stateMachine.getSharedData().glitch->getEnabled();
	isBeyoon = stateMachine.getSharedData().beyoon->getEnabled();
	isReflectX = stateMachine.getSharedData().reflectX->getEnabled();
	isReflectY = stateMachine.getSharedData().reflectY->getEnabled();
	isSplit = stateMachine.getSharedData().split->getEnabled();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		// Scene changing
	case '1':
		stateMachine.changeState("SceneA");
		stateMachine.getSharedData().bloom->setStrength(1.0);
		stateMachine.getSharedData().bloom->setEnabled(true);
		break;
	case '2':
		stateMachine.changeState("SceneB");
		stateMachine.getSharedData().bloom->setEnabled(true);
		stateMachine.getSharedData().bloom->setStrength(3.5);
		break;
	case '3': // ray marching
		stateMachine.changeState("SceneC");
		stateMachine.getSharedData().bloom->setEnabled(false);
		stateMachine.getSharedData().bloom->setStrength(1.0);
		break;
	case '4':
		stateMachine.changeState("SceneD");
		stateMachine.getSharedData().bloom->setEnabled(true);
		stateMachine.getSharedData().bloom->setStrength(1.0);
		break;
	case '5':
		stateMachine.changeState("SceneE");
		stateMachine.getSharedData().bloom->setEnabled(true);
		stateMachine.getSharedData().bloom->setStrength(1.0);
		break;
	case '6':
		stateMachine.changeState("SceneF");
		stateMachine.getSharedData().bloom->setEnabled(false);
		stateMachine.getSharedData().bloom->setStrength(0.5);
		break;
	// Post Effect enable/disable
	case 'q': // Bloom
		isBloom = !isBloom;
		stateMachine.getSharedData().bloom->setEnabled(isBloom);
		break;
	case 'w': // Edge
		isEdge = !isEdge;
		stateMachine.getSharedData().edge->setEnabled(isEdge);
		break;
	case 'e': // NoiseWarp
		isNoiseWarp = !isNoiseWarp;
		stateMachine.getSharedData().noiseWarp->setEnabled(isNoiseWarp);
		break;
	case 'r': // RGBShift
		isRGBShift = !isRGBShift;
		stateMachine.getSharedData().rgbShift->setEnabled(isRGBShift);
		break;
	case 't': // ZoomBlur
		isZoomBlur = !isZoomBlur;
		stateMachine.getSharedData().zoomBlur->setEnabled(isZoomBlur);
		break;
	case 'y':
		isInvert = !isInvert;
		stateMachine.getSharedData().invert->setEnabled(isInvert);
		break;
	case 'u':
		isGlitch = !isGlitch;
		stateMachine.getSharedData().glitch->setEnabled(isGlitch);
		break;
	case 'i':
		isBeyoon = !isBeyoon;
		stateMachine.getSharedData().beyoon->setEnabled(isBeyoon);
		break;
	case 'o':
		isReflectX = !isReflectX;
		stateMachine.getSharedData().reflectX->setEnabled(isReflectX);
		break;
	case 'p':
		isReflectY = !isReflectY;
		stateMachine.getSharedData().reflectY->setEnabled(isReflectY);
		break;
	case '@':
		isSplit = !isSplit;
		stateMachine.getSharedData().split->setEnabled(isSplit);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input) {
	curVol = 0.0;
	int numCounted = 0;
	for(int i = 0; i < input.getNumFrames(); i++) {
		stateMachine.getSharedData().left[i] = input[i * 2] * 0.5;
		stateMachine.getSharedData().right[i] = input[i * 2 + 1] * 0.5;
		curVol += stateMachine.getSharedData().left[i] * stateMachine.getSharedData().left[i];
		curVol += stateMachine.getSharedData().right[i] * stateMachine.getSharedData().right[i];
		numCounted += 2;
	}
	curVol /= (float)numCounted;
	curVol = sqrt(curVol);

	kickThreshold = ofLerp(kickThreshold, minimumThreshold, decayRate);

	if (curVol > kickThreshold) {
		kickThreshold = curVol;
		stateMachine.getSharedData().isKicked = true;
		isKicked = true;
	}
	else {
		stateMachine.getSharedData().isKicked = false;
		isKicked = false;
	}

	/*smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;*/

	stateMachine.getSharedData().volume = ofMap(curVol, 0, 0.17, 0, 1);
}