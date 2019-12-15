#include "ofApp.h"
#include "SceneA.h"
#include "SceneB.h"
#include "SceneC.h"
#include "SceneD.h"

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
	stateMachine.getSharedData().gui.add(isDof.setup("Dof", false));
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
	stateMachine.getSharedData().post.createPass<BloomPass>()->setEnabled(isBloom);
	stateMachine.getSharedData().post.createPass<EdgePass>()->setEnabled(isEdge);
	stateMachine.getSharedData().post.createPass<DofPass>()->setEnabled(isDof);
	stateMachine.getSharedData().post.createPass<NoiseWarpPass>()->setEnabled(isNoiseWarp);
	stateMachine.getSharedData().post.createPass<PixelatePass>()->setEnabled(isPixelate);
	stateMachine.getSharedData().post.createPass<RGBShiftPass>()->setEnabled(isRGBShift);
	stateMachine.getSharedData().post.createPass<ZoomBlurPass>()->setEnabled(isZoomBlur);
	stateMachine.getSharedData().post.createPass<GodRaysPass>()->setEnabled(isGodray);
	stateMachine.getSharedData().post.createPass<Invert>()->setEnabled(isInvert);
	stateMachine.getSharedData().post.createPass<Glitch>()->setEnabled(isGlitch);

	stateMachine.getSharedData().fbo.allocate(ofGetWidth(), ofGetHeight());

	stateMachine.addState<SceneA>();
	stateMachine.addState<SceneB>();
	stateMachine.addState<SceneC>();
	stateMachine.addState<SceneD>();
	stateMachine.changeState("SceneA");

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
	soundStream.setup(settings);
}

//--------------------------------------------------------------
void ofApp::update(){
	stateMachine.getSharedData().time = ofGetElapsedTimef();

	sub->fbo = stateMachine.getSharedData().fbo;

	sound = stateMachine.getSharedData().volume;
	fps = ofGetFrameRate();
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
		break;
	case '2':
		stateMachine.changeState("SceneB");
		isBloom = true;
		break;
	case '3':
		stateMachine.changeState("SceneC");
		break;
	case '4':
		stateMachine.changeState("SceneD");
		break;
	case ' ':
		ofToggleFullscreen();
		break;
	// Post Effect enable/disable
	case 'q': // Bloom
		isBloom = !isBloom;
		stateMachine.getSharedData().post[0]->setEnabled(isBloom);
		break;
	case 'w': // Edge
		isEdge = !isEdge;
		stateMachine.getSharedData().post[1]->setEnabled(isEdge);
		break;
	case 'e': // Dof
		isDof = !isDof;
		stateMachine.getSharedData().post[2]->setEnabled(isDof);
		break;
	case 'r': // NoiseWarp
		isNoiseWarp = !isNoiseWarp;
		stateMachine.getSharedData().post[3]->setEnabled(isNoiseWarp);
		break;
	case 't': // Pixelate
		isPixelate = !isPixelate;
		stateMachine.getSharedData().post[4]->setEnabled(isPixelate);
		break;
	case 'y': // RGBShift
		isRGBShift = !isRGBShift;
		stateMachine.getSharedData().post[5]->setEnabled(isRGBShift);
		break;
	case 'u': // ZoomBlur
		isZoomBlur = !isZoomBlur;
		stateMachine.getSharedData().post[6]->setEnabled(isZoomBlur);
		break;
	case 'i': // Godray
		isGodray = !isGodray;
		stateMachine.getSharedData().post[7]->setEnabled(isGodray);
		break;
	case 'o':
		isInvert = !isInvert;
		stateMachine.getSharedData().post[8]->setEnabled(isInvert);
		break;
	case 'p':
		isGlitch = !isGlitch;
		stateMachine.getSharedData().post[9]->setEnabled(isGlitch);
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
		float left = input[i * 2] * 0.5;
		float right = input[i * 2 + 1] * 0.5;
		curVol += left * left;
		curVol += right * right;
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