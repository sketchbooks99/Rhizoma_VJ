#include "ofMain.h"
#include "ofApp.h"
#include "subApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings s;

	s.setGLVersion(4, 1);
	s.setSize(1080, 720);
	s.setPosition(ofVec2f(0, 0));
	s.resizable = false;
	shared_ptr<ofAppBaseWindow> subWindow = ofCreateWindow(s);

	s.setSize(1920, 1080);
	s.setPosition(ofVec2f(500, 0));
	s.resizable = true;
	s.shareContextWith = subWindow;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(s);

	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<subApp> SubApp(new subApp);
	mainApp->sub = SubApp;

	ofRunApp(mainWindow, mainApp);
	ofRunApp(subWindow, SubApp);

	ofRunMainLoop();

}
