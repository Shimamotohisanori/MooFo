#include "stdafx.h"
#include "Game.h"
#include"GameCamera/GameCamera.h"
#include "Source/Actor/Stage/Stage.h"
#include"Source/Actor/Character/Player/Player.h"
#include"Source/Actor/Character/UFO/UFO.h"
#include"Source/Actor/Character/Cow/Cow.h"
#include"Pause/Pause.h"
#include"GameTimer/Timer.h"
#include"GameClear.h"
#include"GameOver.h"
#include"CountDown/CountDown.h"
#include "SoundManager/SoundManager.h"

Game::~Game()
{
	//プレイヤーを削除
	DeleteGO(m_player);
	//ステージを削除
	DeleteGO(m_stage);

	//牛を削除
	for (int i = 0; i < 14; i++)
	{
		DeleteGO(m_cow[i]);
	}

	//牛を削除
	for (int i = 0; i < 4; i++)
	{
		DeleteGO(m_UFO[i]);
	}

	//タイマーを削除
	DeleteGO(m_timer);
	//ゲームカメラを削除
	DeleteGO(m_gameCamera);
	//カウントダウンの削除
	DeleteGO(m_countDown);
	//Pauseの削除
	DeleteGO(m_pause);

}
bool Game::Start()
{
	m_countDown = NewGO<CountDown>(0, "countdown");

	//m_modelRender.Init("Assets/modelData/unityChan.tkm");
	m_player = NewGO < Player>(0, "player");
	//ステージの生成
	m_stage = NewGO<Stage>(0, "stage");
	
	//牛の生成
	for (int i = 0; i < 15; i++)
	{
		m_cow[i] = NewGO<Cow>(0, "cow");
		m_cow[i]->Setposition(Vector3(-300.0f + i * 75.0f, 0.0f, 0.0f));
	}

	//UFOの生成
	for (int i = 0; i < 4; i++)
	{
		m_UFO[i] = NewGO<UFO>(0, "UFO");
		m_UFO[i]->SetPosition(Vector3(-300.0f + i * 600.0f, 70.0f, 0.0f));
	}

	//タイマーの生成
	m_timer = NewGO<Timer>(0, "timer");

	//ポーズ画面の生成をするが非アクティブにする
	m_pause = NewGO<Pause>(0, "pause");
	m_pause->Deactivate();
	
	m_inGameSound = FindGO<SoundManager>("soundmanager");	

	//ゲームカメラの生成
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	return true;
}



void Game::Update()
{

	if (!m_isSound)
	{
		p_inGameBGM = m_inGameSound->PlayingBGM(SoundBGM::enInGameBGM, false);
		m_isSound = true;
	}
	//ポーズ中はゲーム処理をしない
	if (m_pause->IsActive())
	{
		return;
	}
	//セレクトボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonSelect))
	{

		DeleteGO(p_inGameBGM);
    
		/** ポーズ画面をアクティブにする */
		m_pause->Activate();
		m_pause->SetIsPause(true);

  }
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
		DeleteGO(p_inGameBGM);
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
		DeleteGO(p_inGameBGM);
		DeleteGO(this);
	}
}

