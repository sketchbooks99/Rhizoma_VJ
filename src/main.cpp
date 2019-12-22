#include "ofMain.h"
#include "ofApp.h"
#include "subApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings s;

	s.setGLVersion(4, 5);
	s.setSize(2560, 1440);
	//s.setSize(1920, 1080);
	s.setPosition(ofVec2f(2400, 0));
	s.monitor = 1;
	//s.windowMode = OF_FULLSCREEN;
	// s.multiMonitorFullScreen = true;
	s.resizable = false;
	s.decorated = false;
	shared_ptr<ofAppBaseWindow> subWindow = ofCreateWindow(s);
	

	s.setSize(1920, 1080);
	//s.setSize(960, 540);
	s.setPosition(ofVec2f(0, 0));
	s.resizable = true;
	//s.windowMode = OF_FULLSCREEN;
	//s.monitor = 0;
	s.shareContextWith = subWindow;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(s);


	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<subApp> SubApp(new subApp);
	mainApp->sub = SubApp;

	ofRunApp(mainWindow, mainApp);
	ofRunApp(subWindow, SubApp);

	ofRunMainLoop();

}
