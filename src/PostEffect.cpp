#include "PostEffect.h"

void PostEffect::setup() {
	bloom.allocate(ofGetWidth(), ofGetHeight());
	bloom.setStrength(1.0);
	bloom.setBlurSize(3.0);
	bloom.setThreshold(0.5);
	bloom.setupShaders();
}
