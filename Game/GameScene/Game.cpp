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
#include "Rope/Rope.h"
#include "CowNumberOfRescues/CowNumberOfRescues.h"
#include "SoundManager/SoundManager.h"

Game::~Game()
{
	//プレイヤーを削除
	DeleteGO(m_player);
	//ステージを削除
	DeleteGO(m_stage);

	//牛を削除
	for (int i = 0; i < EnCow_Num; i++)
	{
		if (m_cow[i] != nullptr)
		{
			DeleteGO(m_cow[i]);
		}
	}

	/** SpawnCow等で増えた牛も含めて生きている牛は全て削除 */
	for (auto cow : m_aliveCows)
	{
		if (cow)
		{
			DeleteGO(cow);
		}
	}
	m_aliveCows.clear();

	/** UFOを削除 */
	for (int i = 0; i < EnUFO_Num; i++)
	{
		if (m_UFO[i] != nullptr)
		{
			DeleteGO(m_UFO[i]);
		}
	}

	//タイマーを削除
	DeleteGO(m_timer);
	//ゲームカメラを削除
	DeleteGO(m_gameCamera);
	//カウントダウンの削除
	DeleteGO(m_countDown);
	//Pauseの削除
	DeleteGO(m_pause);
	//牛の救出数の削除
	DeleteGO(m_cowNumberOfRescues);

}
bool Game::Start()
{
	m_countDown = NewGO<CountDown>(0, "countdown");

	m_player = NewGO <Player>(0, "player");
	//ステージの生成
	m_stage = NewGO<Stage>(0, "stage");
	
	//牛の生成
	for (int i = 0; i < _countof(COW_INFOMATIONS); i++)
	{
		m_cow[i] = NewGO<Cow>(0, COW_INFOMATIONS[i].objectName.c_str());
		m_cow[i]->SetPosition(COW_INFOMATIONS[i].pos);
		m_aliveCows.push_back(m_cow[i]);
	}

	//UFOの生成
	for (int i = 0; i < _countof(UFO_INFOMATIONS); i++)
	{
		m_UFO[i] = NewGO<UFO>(0, UFO_INFOMATIONS[i].objectName.c_str());
		m_UFO[i]->SetPosition(UFO_INFOMATIONS[i].pos);
	}

	//タイマーの生成
	m_timer = NewGO<Timer>(0, "timer");

	//ポーズ画面の生成をするが非アクティブにする
	m_pause = NewGO<Pause>(0, "pause");
	m_pause->Deactivate();
	
	m_inGameSound = FindGO<SoundManager>("soundmanager");	

	//ゲームカメラの生成
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//牛の救出数の生成
	m_cowNumberOfRescues = NewGO<CowNumberOfRescues>(0, "cownumberofrescues");

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

	SpawnCow();

	// 毎フレームロープに最新の牛リストを渡す
	Rope* rope = FindGO<Rope>("rope");
	if (rope)
	{
		rope->SetCowList(m_aliveCows);
	}

	//３秒以内に次の牛を救出できなければコンボが途切れたら
	if(m_comboTimer >0.0f)
	{
		m_comboTimer--;
	}
	else
	{
		m_combo = 0;
	}




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

void Game::SpawnCow()
{
	if (m_timer->GetTimer() <= 4.0f)
	{
		//タイマーが4秒以下なら牛を補充しない
		return;
	}
	// 現在の牛の数が10体未満なら補充
	if (m_aliveCows.size() < _countof(COW_INFOMATIONS))
	{
		m_spawnTimer += g_gameTime->GetFrameDeltaTime();

		// 3秒ごとに1体補充
		if (m_spawnTimer >= 3.0f)
		{
			m_spawnTimer = 0.0f;

			// 新しい牛を生成
			Cow* newCow = NewGO<Cow>(0, "cow");

			// スポーン位置（例：ランダム）
			Vector3 pos;
			pos.x = (rand() % 600) - 300; // -300〜300
			pos.y = 0.0f;
			pos.z = (rand() % 600) - 300; // -300〜300

			newCow->SetPosition(pos);

			// 生きている牛リストに追加
			m_aliveCows.push_back(newCow);
		}
	}

}


void Game::AddCombo()
{
	m_combo++;
	//5秒以内に牛を救出出来ればコンボ継続
	m_comboTimer = 300.0f;


	//コンボの時だけ制限時間を増やす
	if (m_combo >= 2)
	{
		m_timer = FindGO<Timer>("timer");
		if (m_timer)
		{
			//コンボすると３秒追加
			m_timer->AddTimer(3.0f);
		}
	}
}


bool Game::IsCombo()const
{
	return m_combo >= 2;
}

