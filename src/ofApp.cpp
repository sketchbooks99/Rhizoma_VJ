#include "ofApp.h"
#include "SceneA.h"
#include "SceneB.h"
#include "SceneC.h"

//--------------------------------------------------------------
void ofApp::setup() {
	stateMachine.getSharedData().gui.setup();
	stateMachine.getSharedData().gui.setPosition(10, 10);
	stateMachine.getSharedData().gui.add(isBloom.setup("Bloom", false));
	stateMachine.getSharedData().gui.add(isEdge.setup("Edge", false));
	stateMachine.getSharedData().gui.add(isDof.setup("Dof", false));
	stateMachine.getSharedData().gui.add(isNoiseWarp.setup("NoiseWarp", false));
	stateMachine.getSharedData().gui.add(isPixelate.setup("Pixelate", false));
	stateMachine.getSharedData().gui.add(isRGBShift.setup("RGBShift", false));
	stateMachine.getSharedData().gui.add(isSSAO.setup("SSAO", false));
	stateMachine.getSharedData().gui.add(isZoomBlur.setup("ZoomBlur", false));
	stateMachine.getSharedData().gui.add(isGodray.setup("Godray", false));
	
	stateMachine.getSharedData().post.init(ofGetWidth(), ofGetHeight());
	stateMachine.getSharedData().post.setFlip(false);
	stateMachine.getSharedData().post.createPass<BloomPass>()->setEnabled(isBloom);
	stateMachine.getSharedData().post.createPass<EdgePass>()->setEnabled(isEdge);
	stateMachine.getSharedData().post.createPass<DofPass>()->setEnabled(isDof);
	stateMachine.getSharedData().post.createPass<NoiseWarpPass>()->setEnabled(isNoiseWarp);
	stateMachine.getSharedData().post.createPass<PixelatePass>()->setEnabled(isPixelate);
	stateMachine.getSharedData().post.createPass<RGBShiftPass>()->setEnabled(isRGBShift);
	stateMachine.getSharedData().post.createPass<SSAOPass>()->setEnabled(isSSAO);
	stateMachine.getSharedData().post.createPass<ZoomBlurPass>()->setEnabled(isZoomBlur);
	stateMachine.getSharedData().post.createPass<GodRaysPass>()->setEnabled(isGodray);

	stateMachine.addState<SceneA>();
	stateMachine.addState<SceneB>();
	stateMachine.addState<SceneC>();
	stateMachine.changeState("SceneA");

	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
	for (int i = 0; i < stateMachine.getSharedData().gui.getControlNames().size(); i++) {
	 	 stateMachine.getSharedData().post[i]->setEnabled(stateMachine.getSharedData().gui.getToggle(stateMachine.getSharedData().gui.getControlNames()[i]));
	}
	stateMachine.getSharedData().time = ofGetElapsedTimef();
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case '1':
		cout << "1" << endl;
		stateMachine.changeState("SceneA");
		break;
	case '2':
		cout << "2" << endl;
		stateMachine.changeState("SceneB");
		break;
	case '3':
		stateMachine.changeState("SceneC");
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
