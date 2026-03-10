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
	//座標を設定
	m_cow->Setposition(Vector3(0.0f, 0.0f, 0.0f));

	//回転ステートがスピンの牛の生成
	Cow* spinCow = NewGO<Cow>(0, "spinCow");
	spinCow->m_rotationState = Cow::EnRotatitonState_Spin;
	spinCow->Setposition(Vector3(300.0f, 0.0f, 0.0f));

	//ランダムに移動するUFOの生成
	m_UFO = NewGO<UFO>(0, "UFO");
	m_UFO->SetPosition(Vector3(0.0f, 70.0f, 0.0f));

	//Idle状態のUFOの生成
	UFO* idleUFO = NewGO<UFO>(0, "idleUFO");
	idleUFO->m_UFOState = UFO::EnUFOState_Idle;
	idleUFO->SetPosition(Vector3(300.0f, 70.0f, 0.0f));

	//ゲームカメラの生成
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	
	return true;
}

void Game::Update()
{
	
}

