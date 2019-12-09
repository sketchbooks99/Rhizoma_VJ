#pragma once

#include "ofxState.h"
#include "SharedData.h"

class SceneA : public itg::ofxState<SharedData> {
public:
	void setup();
	void update();
	void draw();
	string getName();

private:
	struct Circle {
	public:
		Circle(unsigned int _res, unsigned int _size, ofVec3f _pos, ofColor _color, bool _isBright = false) {
			res = _res;
			size = _size;
			pos = _pos;
			isBright = _isBright;
			color = _color;

			inner.setCircleResolution(_res);
			inner.circle(pos, _size - 7);
			inner.setColor(_color);

			c.setCircleResolution(_res);
			c.arc(pos, _size, _size, 0, 360);
			c.arc(pos, _size - 7, _size - 7, 0, 360);
			c.setColor(ofColor(_color.r, _color.g, _color.b, _color.a * 2.0));
		}
		unsigned int res;
		unsigned int size;
		bool isBright;
		ofVec3f pos;
		ofPath c, inner;
		ofColor color;

		void draw() {
			inner.draw();
			c.draw();
		}
	};
	ofFbo renderFbo;
	ofEasyCam cam;

	// circles
	vector<Circle> circles;
};