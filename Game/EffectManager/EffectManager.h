#pragma once
/** エフェクトのID*/
/** 必要に応じて追加する */
enum class EffectID
{
	EffectID_UFOLight,/** UFOの光*/
	EffectID_HeartEffect,/** 救出された牛が出すハート*/
	EffectID_CowFoodEffect,/** 牛の餌*/
	EffectID_Abutton, /** 牛の餌に近づくと表示されるAボタン */
	EffectID_Area, /** 牛舎付近のエリア範囲 */
	EffectID_Overdose, /** 牛が満腹状態 */
	EffectID_UFOConfusionEffect_1,/**救出されたら出るUFOの混乱エフェクト(1段階目)*/
	EffectID_UFOConfusionEffect_2,/**救出されたら出るUFOの混乱エフェクト(2段階目)*/
	EffectID_UFOConfusionEffect_3,/**救出されたら出るUFOの混乱エフェクト(3段階目)*/
	EffectID_CowDeleteEffect,/**牛が削除されるときのエフェクト*/
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

