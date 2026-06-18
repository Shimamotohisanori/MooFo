#include "stdafx.h"
#include "EffectManager/EffectManager.h"

bool EffectManager::Start()
{
	LoadEffects();
	return true;
}

void EffectManager::LoadEffects()
{
	/** UFOの光*/
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect(
		(int)EffectID::EffectID_UFOLight,
		u"Assets/Effects/UFOLight.efk"
	);

	/** 救出された牛が出すハート*/
	nsK2EngineLow::EffectEngine::GetInstance()
		->ResistEffect((int)EffectID::EffectID_HeartEffect, u"Assets/Effects/heartEffect.efk");

	/** 牛の餌*/
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect
	(
		(int)EffectID::EffectID_CowFoodEffect,
		u"Assets/Effects/cowFood.efk"
	);

	/** 牛の餌に近づくと表示されるAボタン */
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect
	(
		(int)EffectID::EffectID_Abutton,
		u"Assets/Effects/Abutton.efk"
	);

	/** 牛舎付近のエリア範囲 */
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect
	(
		(int)EffectID::EffectID_Area,
		u"Assets/Effects/Area.efk"
	);

	/** 牛が満腹状態 */
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect
	(
		(int)EffectID::EffectID_Overdose,
		u"Assets/Effects/Overdose.efk"
	);

	/** 一回目のUFO混乱エフェクト*/
	nsK2EngineLow::EffectEngine::GetInstance()
		->ResistEffect((int)EffectID::EffectID_UFOConfusionEffect_1, u"Assets/Effects/UFOConfusion_1.efk");
	/** 二回目のUFO混乱エフェクト*/
	nsK2EngineLow::EffectEngine::GetInstance()
		->ResistEffect((int)EffectID::EffectID_UFOConfusionEffect_2, u"Assets/Effects/UFOConfusion_2.efk");
	/** 三回目のUFO混乱エフェクト*/
	nsK2EngineLow::EffectEngine::GetInstance()
		->ResistEffect((int)EffectID::EffectID_UFOConfusionEffect_3, u"Assets/Effects/UFOConfusion_3.efk");
}
