#include "stdafx.h"
#include "EffectManager/EffectManager.h"

bool EffectManager::Start()
{
	LoadEffects();
	return true;
}

void EffectManager::LoadEffects()
{

	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect(
		(int)EffectID::EffectID_UFOLight,
		u"Assets/Effects/UFOLight.efk"
	);

	nsK2EngineLow::EffectEngine::GetInstance()
		->ResistEffect((int)EffectID::EffectID_HeartEffect, u"Assets/Effects/heartEffect.efk");

	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect
	(
		(int)EffectID::EffectID_CowFoodEffect,
		u"Assets/Effects/cowFood.efk"
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
