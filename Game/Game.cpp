#include "stdafx.h"
#include "Game.h"
#include "Source/Actor/Stage/Stage.h"

bool Game::Start()
{
	m_modelRender.Init("Assets/modelData/unityChan.tkm");

	m_stage = NewGO<Stage>(0, "stage");

	return true;
}

void Game::Update()
{
	// g_renderingEngine->DisableRaytracing();
	m_modelRender.Update();
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}