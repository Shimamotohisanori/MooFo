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
		u"Game/Assets/Effects/UFOLight1.efk"
	);
}
