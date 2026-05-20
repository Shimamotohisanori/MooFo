#pragma once
/** エフェクトのID*/
/** 必要に応じて追加する */
enum class EffectID
{
	EffectID_UFOLight,
	EffectID_HeartEffect,
	EffectID_Num
};

/** エフェクトマネージャークラス */
class EffectManager : public IGameObject
{
public:
	EffectManager() {}
	~EffectManager() {}

	/** エフェクトの初期化 */
	bool Start() override;

	/** エフェクトの読み込み関数 */
	void LoadEffects();
};

