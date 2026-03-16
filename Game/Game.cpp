#include "stdafx.h"
#include "Game.h"
#include"GameCamera.h"
#include "Source/Actor/Stage/Stage.h"
#include"Source/Actor/Character/Player/Player.h"
#include"Source/Actor/Character/UFO/UFO.h"
#include"Source/Actor/Character/Cow/Cow.h"
#include"Timer.h"
#include"GameClear.h"
#include"GameOver.h"




Game::~Game()
{
	//プレイヤーを削除
	DeleteGO(m_player);
	//ステージを削除
	DeleteGO(m_stage);
	//牛を削除
	DeleteGO(m_cow);
	//回転牛を削除
	DeleteGO(m_spincow);
	//UFOを削除
	DeleteGO(m_UFO);
	//棒立ち状態のUFOを削除
	DeleteGO(m_IdleUFO);
	//タイマーを削除
	DeleteGO(m_timer);
	//ゲームカメラを削除
	DeleteGO(m_gameCamera);


}
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
	m_spincow = NewGO<Cow>(0, "spinCow");
	m_spincow->m_rotationState = Cow::EnRotatitonState_Spin;
	m_spincow->Setposition(Vector3(300.0f, 0.0f, 0.0f));

	//ランダムに移動するUFOの生成
	m_UFO = NewGO<UFO>(0, "UFO");
	m_UFO->SetPosition(Vector3(0.0f, 70.0f, 0.0f));

	//Idle状態のUFOの生成
	m_IdleUFO = NewGO<UFO>(0, "idleUFO");
	m_IdleUFO->m_UFOState = UFO::EnUFOState_Idle;
	m_IdleUFO->SetPosition(Vector3(300.0f, 70.0f, 0.0f));

	m_timer = NewGO<Timer>(0, "timer");

	//ゲームカメラの生成
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	
	return true;
}



void Game::Update()
{
	//クリア処理
	Clear();
	//ゲームオーバー処理
	Death();
}


void Game::Clear()
{
	if (m_timer->GetTimer() <= 0.0f)
	{
		//ゲームクリアの画像を呼び出す
		m_gameClear = NewGO<GameClear>(0, "gameClear");
		DeleteGO(this);

	}
}

void Game::Death()
{
	if (m_isDead)return;
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_isDead = true;
		m_gameOver = NewGO<GameOver>(0, "gameover");
		DeleteGO(this);
	}
}

