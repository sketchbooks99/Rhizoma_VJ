#include "ofApp.h"
#include "SceneA.h"
#include "SceneB.h"
#include "SceneC.h"
#include "SceneD.h"
#include "Debug.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	//ofEnableAntiAliasing();

	// GUI Settings
	stateMachine.getSharedData().gui.setup();
	stateMachine.getSharedData().gui.setPosition(10, 10);
	stateMachine.getSharedData().gui.add(isBloom.setup("Bloom", false));
	stateMachine.getSharedData().gui.add(isEdge.setup("Edge", false));
	stateMachine.getSharedData().gui.add(isRotate.setup("Rotate", false));
	stateMachine.getSharedData().gui.add(isNoiseWarp.setup("NoiseWarp", false));
	stateMachine.getSharedData().gui.add(isPixelate.setup("Pixelate", false));
	stateMachine.getSharedData().gui.add(isRGBShift.setup("RGBShift", false));
	stateMachine.getSharedData().gui.add(isZoomBlur.setup("ZoomBlur", false));
	stateMachine.getSharedData().gui.add(isGodray.setup("Godray", false));
	stateMachine.getSharedData().gui.add(isInvert.setup("Invert", false));
	stateMachine.getSharedData().gui.add(isGlitch.setup("Glitch", false));
	stateMachine.getSharedData().gui.add(sound.set("Sound", 0, 0, 1));
	stateMachine.getSharedData().gui.add(fps.set("fps", 60, 0, 60));
	stateMachine.getSharedData().gui.add(isKicked.setup("isKick", false));

	stateMachine.getSharedData().post.init(ofGetWidth(), ofGetHeight());
	stateMachine.getSharedData().post.setFlip(false);
	stateMachine.getSharedData().bloom = stateMachine.getSharedData().post.createPass<BloomPass>();
	stateMachine.getSharedData().bloom->setEnabled(isBloom);
	stateMachine.getSharedData().bloom->setStrength(1.0f);
	stateMachine.getSharedData().edge = stateMachine.getSharedData().post.createPass<EdgePass>();
	stateMachine.getSharedData().edge->setEnabled(isEdge);
	stateMachine.getSharedData().rotate = stateMachine.getSharedData().post.createPass<Rotate>();
	stateMachine.getSharedData().rotate->setEnabled(isRotate);
	stateMachine.getSharedData().noiseWarp = stateMachine.getSharedData().post.createPass<NoiseWarpPass>();
	stateMachine.getSharedData().noiseWarp->setEnabled(isNoiseWarp);
	stateMachine.getSharedData().pixelate = stateMachine.getSharedData().post.createPass<PixelatePass>();
	stateMachine.getSharedData().pixelate->setEnabled(isPixelate);
	stateMachine.getSharedData().rgbShift = stateMachine.getSharedData().post.createPass<RGBShiftPass>();
	stateMachine.getSharedData().rgbShift->setEnabled(isRGBShift);
	stateMachine.getSharedData().zoomBlur = stateMachine.getSharedData().post.createPass<ZoomBlurPass>();
	stateMachine.getSharedData().zoomBlur->setEnabled(isZoomBlur);
	stateMachine.getSharedData().godray = stateMachine.getSharedData().post.createPass<GodRaysPass>();
	stateMachine.getSharedData().godray->setEnabled(isGodray);
	stateMachine.getSharedData().invert = stateMachine.getSharedData().post.createPass<Invert>();
	stateMachine.getSharedData().invert->setEnabled(isInvert);
	stateMachine.getSharedData().glitch = stateMachine.getSharedData().post.createPass<Glitch>();
	stateMachine.getSharedData().glitch->setEnabled(isGlitch);

	stateMachine.getSharedData().fbo.allocate(ofGetWidth(), ofGetHeight());

	stateMachine.addState<SceneA>();
	stateMachine.addState<SceneB>();
	stateMachine.addState<SceneC>();
	stateMachine.addState<SceneD>();
	stateMachine.addState<Debug>();
	stateMachine.changeState("SceneA");

	stateMachine.getSharedData().bloom->setEnabled(false);

	// Sound setup
	//soundStream.setup(this, 0, 2, 44100, 256);
	decayRate = 0.05;
	minimumThreshold = 0.1;
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
}

//--------------------------------------------------------------
void ofApp::update(){
	stateMachine.getSharedData().time = ofGetElapsedTimef();

	sub->fbo = stateMachine.getSharedData().fbo;

	sound = stateMachine.getSharedData().volume;

	stateMachine.getSharedData().rgbShift->setAmount(stateMachine.getSharedData().volume);
	if (isZoomBlurReactive) {
		stateMachine.getSharedData().zoomBlur->setEnabled(stateMachine.getSharedData().isKicked);
	}
	fps = ofGetFrameRate();

	isBloom = stateMachine.getSharedData().bloom->getEnabled();
	isEdge = stateMachine.getSharedData().edge->getEnabled();
	isRotate = stateMachine.getSharedData().rotate->getEnabled();
	isNoiseWarp = stateMachine.getSharedData().noiseWarp->getEnabled();
	isPixelate = stateMachine.getSharedData().pixelate->getEnabled();
	isRGBShift = stateMachine.getSharedData().rgbShift->getEnabled();
	isZoomBlur = stateMachine.getSharedData().zoomBlur->getEnabled();
	isGodray = stateMachine.getSharedData().godray->getEnabled();
	isInvert = stateMachine.getSharedData().invert->getEnabled();
	isGlitch = stateMachine.getSharedData().glitch->getEnabled();
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
		stateMachine.getSharedData().bloom->setStrength(1.5);
		break;
	case '2':
		stateMachine.changeState("SceneB");
		stateMachine.getSharedData().bloom->setEnabled(true);
		stateMachine.getSharedData().bloom->setStrength(3.5);
		break;
	case '3':
		stateMachine.changeState("SceneC");
		break;
	case '4':
		stateMachine.changeState("SceneD");
		break;
	case '5':
		stateMachine.changeState("Debug");
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
	case 'e':
		isRotate = !isRotate;
		stateMachine.getSharedData().rotate->setEnabled(isRotate);
		break;
	case 'r': // NoiseWarp
		isNoiseWarp = !isNoiseWarp;
		stateMachine.getSharedData().noiseWarp->setEnabled(isNoiseWarp);
		break;
	case 't': // Pixelate
		isPixelate = !isPixelate;
		stateMachine.getSharedData().pixelate->setEnabled(isPixelate);
		break;
	case 'y': // RGBShift
		isRGBShift = !isRGBShift;
		stateMachine.getSharedData().rgbShift->setEnabled(isRGBShift);
		break;
	case 'u': // ZoomBlur
		isZoomBlur = !isZoomBlur;
		stateMachine.getSharedData().zoomBlur->setEnabled(isZoomBlur);
		break;
	case 'i': // Godray
		isGodray = !isGodray;
		stateMachine.getSharedData().godray->setEnabled(isGodray);
		break;
	case 'o':
		isInvert = !isInvert;
		stateMachine.getSharedData().invert->setEnabled(isInvert);
		break;
	case 'p':
		isGlitch = !isGlitch;
		stateMachine.getSharedData().glitch->setEnabled(isGlitch);
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