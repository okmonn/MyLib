#pragma once
#include "SndFunc.h"
#include <memory>

class Compute;
class Sound;

// サウンドエフェクター
class Effector
{
	// エフェクトパラメータ
	struct Param {
		//コンプレッサ
		snd::Compressor comp;
		// ディストーション
		float distortion;
		//トレモロ
		snd::Toremor toremor;
		//読み込みインデックス
		float index;
		//パン
		float pan;
		//音量
		float volume;
	};

public:
	// コンストラクタ
	Effector(Sound* sound);
	// デストラクタ
	~Effector();

	// 実行
	void Execution(std::vector<float>& input);

private:
	Effector(const Effector&) = delete;
	void operator=(const Effector&) = delete;

	// 初期化
	void Init(void);


	// サウンド
	Sound* sound;

	// コンピュート
	std::unique_ptr<Compute>compute;

	// エフェクトパラメータ
	Param param;
};
