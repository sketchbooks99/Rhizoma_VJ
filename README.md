# ライゾマ忘年会が初VJ!!????

# エフェクトについて
ポストエフェクトをかける場合は、各シーンの描画内容に対して行うようにする。
よっぽどのことがない限り、シーン全体に同じエフェクトをかける。
`PostEffect`クラスで管理
各シーンで必ず
```
getSharedData().postEffect.getEffect("bloom").begin();

// draw scene

getSharedData().postEffect.getEffect("bloom").end();
```
みたいにする。
# シーン遷移について
ofxStateMachineを用いて行う。
シーン全体で共有のデータ`postEffect`などは
```
StateMachine.getSharedData().postEffect.setup()
```
みたいにする。(ofApp::setup()にて)
