# ライゾマ忘年会が初VJ!!????

# エフェクトについて
ポストエフェクトをかける場合は、各シーンの描画内容に対して行うようにする。
よっぽどのことがない限り、シーン全体に同じエフェクトをかける。
ofxPostProcessingで管理
各シーンで必ず
```
fbo.begin();
// draw scene
fbo.end();

getSharedData().post.begin();
fbo.draw(0, 0);
getSharedData().post.end();
```
みたいにする。
# シーン遷移について
ofxStateMachineを用いて行う。
シーン全体で共有のデータ`post`などにする

# TODO
- [x] posteffectは全部fboに書き込んでからかける？
- 毛座どりさんのを参考にしたやつ。角数が違う円をいっぱい書く。volumetric light、パーティクル
- レイマーチングネタ
- 円でかわいいやつ
- 海
- それぞれのシーンの親和性を高める。
- Audio Reactive
- ボタンでシーン制御できるように。
