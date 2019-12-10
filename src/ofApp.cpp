#include "ofApp.h"
#include "SceneA.h"
#include "SceneB.h"
#include "SceneC.h"

//--------------------------------------------------------------
void ofApp::setup(){
	stateMachine.getSharedData().postEffect.setup();

	stateMachine.addState<SceneA>();
    stateMachine.addState<SceneB>();
    stateMachine.addState<SceneC>();
    
	stateMachine.changeState("SceneA");

	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 1:
            stateMachine.changeState("SceneA");
            break;
        case 2:
            stateMachine.changeState("SceneB");
            break;
        case 3:
            stateMachine.changeState("SceneC");
            break;
        case 4:
            stateMachine.changeState("SceneD");
            break;
        case 5:
            stateMachine.changeState("SceneE");
            break;
        case 'q':
            stateMachine.getSharedData().postEffect.post[0]->setEnabled(true);
            break;
        case 'w':
            stateMachine.getSharedData().postEffect.post[0]->setEnabled(true);
            break;
        case 'e':
            stateMachine.getSharedData().postEffect.post[0]->setEnabled(true);
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
