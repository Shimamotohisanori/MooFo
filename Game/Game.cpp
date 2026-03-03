#include "stdafx.h"
#include "Game.h"
#include"GameCamera.h"
#include "Source/Actor/Stage/Stage.h"
#include"Source/Actor/Character/Player/Player.h"
#include"Source/Actor/Character/Cow/Cow.h"

bool Game::Start()
{
	//m_modelRender.Init("Assets/modelData/unityChan.tkm");
	m_player = NewGO < Player>(0, "player");
	m_stage = NewGO<Stage>(0, "stage");
	m_cow = NewGO<Cow>(0, "cow");
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	return true;
}

void Game::Update()
{
	
}

void Game::Render(RenderContext& rc)
{

}