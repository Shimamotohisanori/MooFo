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

	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect(
		(int)EffectID::EffectID_UFOLightCapture,
		u"Assets/Effects/UFOLightCapture.efk"
	);
}
