//
//  Trail.cpp
//  gpuBasedTrail
//
//  Created by 木内舜司 on 2019/03/11.
//

#include "Trail.hpp"
//--------------------------------------------------------------
void Trail::init(int _num, int _length) {
    num = _num;
    length = _length;
}

//--------------------------------------------------------------
void Trail::createBuffers(int _num, int _length) {
	vector<data> trailData;
	trailData.resize(_length * _num);
	for (auto& t : trailData) {
		t.pos.x = ofRandom(-1.0, 1.0);
		t.pos.y = ofRandom(-1.0, 1.0);
		t.pos.z = ofRandom(-1.0, 1.0);
		t.pos.w = 0.0;

		t.vel.x = 0.0;
		t.vel.y = 0.0;
		t.vel.z = 0.0;
		t.vel.w = 0.0;
	}
	dataBuffer.allocate(trailData, GL_DYNAMIC_DRAW);
}

void Trail::debugTexture(int x, int y) {

}

//--------------------------------------------------------------
void Trail::draw(ofPolyRenderMode mode) {
	dataBuffer.bind(GL_SHADER_STORAGE_BUFFER);
	dataBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	compute.begin();
	compute.setUniform1f("time", ofGetElapsedTimef());
	compute.setUniform1f("timestep", 0.005);
	compute.setUniform1f("speed", speed);
	compute.end();
	
    render.begin();
    render.setUniform1i("numTrail", num);
    render.setUniform1i("trailLength", length);
    render.setUniform1f("trailWidth", width);
	render.setUniform3f("color1", color1);
	render.setUniform3f("color1", color2);
    render.setUniform1f("size", size);
    render.setUniform3f("camPos", camPos);
    render.setUniform1f("time", ofGetElapsedTimef());
    render.setUniform2f("texSize", (float)length, (float)num);
    mesh.drawInstanced(mode, num);
    render.end();

	dataBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
	dataBuffer.unbind(GL_SHADER_STORAGE_BUFFER);
}

//--------------------------------------------------------------
void Trail::update() {
    // 速度ピンポン
    //velBuffer.dst->begin();
    //ofClear(0);
    //updateVel.begin();
    //// １フレーム前の速度情報
    //updateVel.setUniformTexture("prevVelTex", velBuffer.src->getTexture(), 0);
    //// 頂点情報
    //updateVel.setUniformTexture("posTex", posBuffer.src->getTexture(), 1);
    //// 各種Uniform変数
    //updateVel.setUniform1f("time", ofGetElapsedTimef());
    //updateVel.setUniform1f("timestep", 0.005f);
    //
    //// 更新された速度情報をテクスチャに書き込む
    //velBuffer.src->draw(0, 0);
    //
    //updateVel.end();
    //velBuffer.dst->end();
    //
    //velBuffer.swap();
    //
    //// ========== 頂点ピンポン =========
    //// 頂点更新
    //posBuffer.dst->begin();
    //ofClear(0);
    //updatePos.begin();
    //updatePos.setUniform1f("speed", speed);
    //updatePos.setUniformTexture("posTex", posBuffer.src->getTexture(), 0);
    //updatePos.setUniformTexture("velTex", velBuffer.src->getTexture(), 1);
    //
    //posBuffer.src->draw(0, 0);
    //
    //updatePos.end();
    //posBuffer.dst->end();
    //
    //posBuffer.swap();
}
