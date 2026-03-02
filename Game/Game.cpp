#include "stdafx.h"
#include "Game.h"


bool Game::Start()
{
	m_modelRender.Init("Assets/modelData/unityChan.tkm");

	m_stageModelRender.Init("Assets/modelData/Stage/Stage.tkm");

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

	m_stageModelRender.Draw(rc);
}