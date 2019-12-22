//
//  Trail.hpp
//  gpuBasedTrail
//
//  Created by 木内舜司 on 2019/03/11.
//

#ifndef Trail_hpp
#define Trail_hpp

#include "ofMain.h"

struct pingPongBuffer {
public:
    void allocate(int _width, int _height, int _internalformat = GL_RGBA) {
        
        for(int i = 0; i < 2; i++) {
            FBOs[i].allocate(_width, _height, _internalformat);
            FBOs[i].getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        }
        
        src = &FBOs[0];
        dst = &FBOs[1];
        
        clear();
    }
    
    void swap() {
        std::swap(src, dst);
    }
    
    void clear() {
        for(int i = 0; i < 2; i++) {
            FBOs[i].begin();
            ofClear(0, 255);
            FBOs[i].end();
        }
    }
    
    ofFbo& operator[](int n) { return FBOs[n]; }
    ofFbo *src; // Source      -> Ping
    ofFbo *dst; // Destination -> Pong
    
private:
    ofFbo FBOs[2]; // real addresses of ping/pong
};

class Trail {
public:
	struct data {
		ofVec4f pos;
		ofVec4f vel;
	};
    void init(int _num, int _length);
    void createBuffers(int _num, int _length);
    void draw(ofPolyRenderMode mode = OF_MESH_FILL);
    void debugTexture(int x, int y);
    void update();

	void loadCompute(std::string _compute) {
		compute.loadCompute(_compute);
	}
    
    void loadVelocityShader(std::string vert, std::string frag) {
        updateVel.load(vert, frag);
    }
    
    void loadPositionShader(std::string vert, std::string frag) {
        updatePos.load(vert, frag);
    }
    
    void loadRenderShader(std::string vert, std::string frag) {
        render.load(vert, frag);
    }
    
    void loadRenderShader(std::string vert, std::string frag, std::string geom)
    {
        render.load(vert, frag, geom);
    }
    
    int getLength() { return length; }
    int getNum() { return num; }
    
    void setWidth(float _width) { width = _width; }
    float getWidth() { return width; }

	void setColor(ofVec3f _color1, ofVec3f _color2) { color1 = _color1; color2 = _color2; }
    
    void setUniforms(ofVec3f _camPos, float _speed, float _size) {
        camPos = _camPos;
        speed = _speed;
        size = _size;
    }
    
    void createMesh(int _length) {
        mesh.clear();
        for(int i=0; i<_length; i++) {
            mesh.addVertex(ofVec3f(0, 0, 0));
            mesh.addTexCoord(ofVec2f(i, 0));
            mesh.addColor(ofFloatColor(1,1,1,1));
        }
    }
    
private:
    float num, length, width, speed, size;
    ofVec3f color1, color2;
    //pingPongBuffer velBuffer, posBuffer;
	ofBufferObject dataBuffer;
    ofVboMesh mesh;
    ofShader render, updateVel, updatePos, compute;
    ofVec3f camPos;
};

#endif /* Trail_hpp */
