#include "subApp.h"

void subApp::setup() {
	ofBackground(0);
	gui.setup();
	gui.setPosition(10, 10);
	gui.add(isBloom.setup("Bloom", false));
	gui.add(isEdge.setup("Edge", false));
	gui.add(isDof.setup("Dof", false));
	gui.add(isNoiseWarp.setup("NoiseWarp", false));
	gui.add(isPixelate.setup("Pixelate", false));
	gui.add(isRGBShift.setup("RGBShift", false));
	gui.add(isSSAO.setup("SSAO", false));
	gui.add(isZoomBlur.setup("ZoomBlur", false));
	gui.add(isGodray.setup("Godray", false));

	fbo.allocate(1920, 1080);
}

void subApp::draw() {
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
	case 'u': // SSAO
		isSSAO = !isSSAO;
		break;
	case 'i': // ZoomBlur
		isZoomBlur = !isZoomBlur;
		break;
	case 'o': // Godray
		isGodray = !isGodray;
		break;
	}
}