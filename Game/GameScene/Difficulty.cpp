#include "stdafx.h"
#include "Difficulty.h"

namespace
{
	const char* DIFFICULTY_FILEPATH = "Assets/sprite/GameTransition/Difficulty/difficulty.dds";

	constexpr float WIDTH  = 1920.0f;
	constexpr float HEIGHT = 1080.0F;
}

Difficulty::Difficulty()
{

}

Difficulty::~Difficulty()
{

}

bool Difficulty::Start()
{
	m_difficultySprite.Init(DIFFICULTY_FILEPATH, WIDTH, HEIGHT);
	m_difficultySprite.Update();
	return true;
}

void Difficulty::Update()
{

}

void Difficulty::Render(RenderContext& rc)
{

}
