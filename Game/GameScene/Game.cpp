#include "stdafx.h"
#include "Game.h"
#include "GameCamera/GameCamera.h"
#include "Source/Actor/Stage/Stage.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/UFO/UFO.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "Pause/Pause.h"
#include "GameTimer/Timer.h"
#include "GameClear.h"
#include "GameOver.h"
#include "CountDown/CountDown.h"
#include "Rope/Rope.h"
#include "CowNumberOfRescues/CowNumberOfRescues.h"
#include "SoundManager/SoundManager.h"
#include "Score/Score.h"
#include "nature/SkyCube.h"
#include"Source/Actor/Character/UFO/CowCaptureController.h"

Game::~Game()
{
	
	//ステージを削除
	DeleteGO(m_stage);

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

	//プレイヤーを削除
	DeleteGO(m_player);

	//タイマーを削除
	DeleteGO(m_timer);

	//スコアを削除
	DeleteGO(m_score);
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

	//スコアの生成
	m_score = NewGO<Score>(0, "score");

	//牛の救出数の生成
	m_cowNumberOfRescues = NewGO<CowNumberOfRescues>(0, "cownumberofrescues");

	//タイマーの生成
	m_timer = NewGO<Timer>(0, "timer");
	//ポーズ画面の生成をするが非アクティブにする
	m_pause = NewGO<Pause>(0, "pause");
	m_pause->Deactivate();
	
	m_inGameSound = FindGO<SoundManager>("soundmanager");	

	//ゲームカメラの生成
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	InitSkyCube();

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
		//カウントダウン中はPauseの処理をしない
		if (m_countDown->GetCountDown())
		{
			return;
		}
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
		//コンボが途切れたらスコアの倍率を1倍に戻す
		m_scoreMagnification = 1;
	}
}


void Game::Clear()
{
	//ゲームクリアの画像にスコアを渡すための変数
	int ClearfinalScore = 0;
	if (m_score)
	{
		ClearfinalScore = m_score->GetScore();
	}
	//ゲームクリアの画像に牛の救出数を渡すための変数
	int ClearfinalRescue = 0;
	if (m_cowNumberOfRescues)
	{
		ClearfinalRescue = m_cowNumberOfRescues->GetNumberOfRescues();
	}
	if (m_timer->GetTimer() <= 0.0f && m_cowNumberOfRescues->GetNumberOfRescues() >=  15)
	{
		//ゲームクリアの画像を呼び出す
		m_gameClear = NewGO<GameClear>(0, "gameClear");
		//ゲームクリアの画像にスコアを渡す
		m_gameClear->SetFinalClearScore(ClearfinalScore);
		//ゲームクリアの画像に牛の救出数を渡す
		m_gameClear->SetFinalClearRescue(ClearfinalRescue);

		DeleteGO(p_inGameBGM);
		DeleteGO(this);
	}
}

void Game::Death()
{
	if (m_isDead)return;
	//ここでスコアを取得
	int finalScore = 0;
	if (m_score)
	{
		finalScore = m_score->GetScore();
	}
	//牛の救出数を取得する変数
	int finalRescue= 0;
	if (m_cowNumberOfRescues)
	{
		finalRescue = m_cowNumberOfRescues->GetNumberOfRescues();
	}

	if (m_timer->GetTimer() <=0.0f &&m_cowNumberOfRescues->GetNumberOfRescues( ) < 15)
	{
		m_isDead = true;
		//ゲームオーバーの画像を呼び出す
		m_gameOver = NewGO<GameOver>(0, "gameover");
		//ゲームオーバーの画像にスコアを渡す
		m_gameOver->SetFinalScore(finalScore);
		//ゲームオーバーの画像に牛の救出数を渡す
		m_gameOver->SetFinalRescue(finalRescue);
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

void Game::InitSkyCube()
{
	/** 現在の空を破棄*/
	DeleteGO(m_skyCube);

	/** 空を出す*/
	m_skyCube = NewGO<SkyCube>(0, "skyCube");

	/** 空の種類を先ほど破棄した空の変数に代入する*/
	m_skyCube->SetType((EnSkyCubeType)m_skyCubeType);

	/** 空の大きさを調整*/
	m_skyCube->SetScale(10000.0f);

	/** 環境光のためのIBLテクスチャをセットする*/
	/** このコードは、要するにg_renderingEngineのクラスの中にある
	 *SetAmbientByIBLTextureていう関数の中にある
	 *光の色が建物に反映される値を変更してる*/
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.6f);
}


void Game::AddCombo()
{
	m_combo++;
	//5秒以内に牛を救出出来ればコンボ継続
	m_comboTimer = 6000.0f;
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


void Game::ResetCombo()
{
	m_combo = 0;
	m_scoreMagnification = 1;
	m_comboTimer = 0.0f;
}
void Game:: AddScore(int score)
{
	int multiplier = 1;
	//5コンボするごとにスコアの獲得量を２倍
	if (m_combo % 5 == 0 && m_combo > 0)
	{
		multiplier = 2;
	}
	if (m_score)
	{
		m_score->AddScore(score *multiplier);
	}
	
}
bool Game::IsCombo()const
{
	return m_combo >= 2;
}

