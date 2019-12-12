#include "ofApp.h"
#include "SceneA.h"
#include "SceneB.h"
#include "SceneC.h"
#include "SceneD.h"

//--------------------------------------------------------------
void ofApp::setup() {
	
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

	stateMachine.addState<SceneA>();
	stateMachine.addState<SceneB>();
	stateMachine.addState<SceneC>();
	stateMachine.addState<SceneD>();
	stateMachine.changeState("SceneA");

	//ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofEnableAntiAliasing();
}

//--------------------------------------------------------------
void ofApp::update(){
	stateMachine.getSharedData().time = ofGetElapsedTimef();
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	stateMachine.getSharedData().post[0]->setEnabled(sub->isBloom);
	stateMachine.getSharedData().post[1]->setEnabled(sub->isEdge);
	stateMachine.getSharedData().post[2]->setEnabled(sub->isDof);
	stateMachine.getSharedData().post[3]->setEnabled(sub->isNoiseWarp);
	stateMachine.getSharedData().post[4]->setEnabled(sub->isPixelate);
	stateMachine.getSharedData().post[5]->setEnabled(sub->isRGBShift);
	stateMachine.getSharedData().post[6]->setEnabled(sub->isZoomBlur);
	stateMachine.getSharedData().post[7]->setEnabled(sub->isGodray);
	stateMachine.getSharedData().post[8]->setEnabled(sub->isInvert);

	sub->fbo.getTexture() = stateMachine.getSharedData().post.getProcessedTextureReference();
	sub->fps = ofGetFrameRate();
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
