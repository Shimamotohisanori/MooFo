#include "stdafx.h"
#include "Game.h"
#include"GameCamera.h"
#include "Source/Actor/Stage/Stage.h"
#include"Source/Actor/Character/Player/Player.h"
#include"Source/Actor/Character/UFO/UFO.h"
#include"Source/Actor/Character/Cow/Cow.h"

bool Game::Start()
{
	//m_modelRender.Init("Assets/modelData/unityChan.tkm");
	m_player = NewGO < Player>(0, "player");
	//ステージの生成
	m_stage = NewGO<Stage>(0, "stage");
	//ランダムに移動する牛の生成
	m_cow = NewGO<Cow>(0, "cow");
	//回転ステートがスピンの牛の生成
	Cow* spinCow = NewGO<Cow>(0, "spinCow");
	spinCow->m_rotationState = Cow::EnRotatitonState_Spin;

	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	m_UFO = NewGO<UFO>(0, "UFO");
	return true;
}

void Game::Update()
{
	
}

void Game::Render(RenderContext& rc)
{

}