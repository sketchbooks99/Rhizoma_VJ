#include "subApp.h"

void subApp::setup() {
	ofBackground(0);
	gui.setup();
	gui.setPosition(10, 40);
	gui.add(isBloom.setup("Bloom", false));
	gui.add(isEdge.setup("Edge", false));
	gui.add(isDof.setup("Dof", false));
	gui.add(isNoiseWarp.setup("NoiseWarp", false));
	gui.add(isPixelate.setup("Pixelate", false));
	gui.add(isRGBShift.setup("RGBShift", false));
	gui.add(isZoomBlur.setup("ZoomBlur", false));
	gui.add(isGodray.setup("Godray", false));
	gui.add(isInvert.setup("Invert", false));

	fbo.allocate(1920, 1080);
}

void subApp::draw() {
	ofDrawBitmapString(ofToString(fps), 10, 10);
	fbo.draw(gui.getPosition().x + gui.getWidth() + 10, 0, fbo.getWidth() * 0.2, fbo.getHeight() * 0.2);
	//mainApp->stateMachine.getSharedData().post.draw(gui.getPosition().x + gui.getWidth(), 0, 200, 150);
	gui.draw();
}

void subApp::keyPressed(int key) {
	switch (key) {
	// Post Effect enable/disable
	case 'q': // Bloom
		isBloom = !isBloom;
		break;
	case 'w': // Edge
		isEdge = !isEdge;
		break;
	case 'e': // Dof
		isDof = !isDof;
		break;
	case 'r': // NoiseWarp
		isNoiseWarp = !isNoiseWarp;
		break;
	case 't': // Pixelate
		isPixelate = !isPixelate;
		break;
	case 'y': // RGBShift
		isRGBShift = !isRGBShift;
		break;
	case 'u': // ZoomBlur
		isZoomBlur = !isZoomBlur;
		break;
	case 'i': // Godray
		isGodray = !isGodray;
		break;
	case 'o':
		isInvert = !isInvert;
		break;
		
	// Scene Change
	case '1':
	case '2':
	case '3':
	case '4':
	}
}