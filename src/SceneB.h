#pragma once

#include "ofxState.h"
#include "SharedData.h"

class SceneB : public itg::ofxState<SharedData> {
	void setup();
	void update();
	void draw();
	string getName();
};