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
#include "Map/Map.h"
#include "nature/SkyCube.h"
#include "Combo/Combo.h"
#include"GameTimer/AddTimerUI.h"


namespace
{
	/**マジックナンバー対策*/
	const uint8_t COW_NUM = 10;
	/** 牛のランダムスポーン範囲 */
	const int RANDOM_SPAWN_RANGE = 300;
	const int RANDOM_SPAWN_RANGE_DOUBLE = 600;
	const float NEW_SPAWN_TIMER = 3.0f;
}

Game::~Game()
{
	
	/** ステージを削除 */
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

	/** プレイヤーを削除 */
	DeleteGO(m_player);

	/** タイマーを削除 */
	DeleteGO(m_timer);

	/** スコアを削除 */
	DeleteGO(m_score);

	/** ゲームカメラを削除 */
	DeleteGO(m_gameCamera);
	
	/** カウントダウンの削除 */
	DeleteGO(m_countDown);

	/** ポーズの削除 */
	DeleteGO(m_pause);

	/** 牛の救出数の削除 */
	DeleteGO(m_cowNumberOfRescues);
	/** ミニマップの削除 */
	DeleteGO(m_map);

	/** タイマー追加UIの削除 */
	DeleteGO(m_addTimerUI);
}
bool Game::Start()
{
	/** ロード画面で生成済みのオブジェクトを参照するだけにする */
	m_player = FindGO<Player>("player");
	m_stage = FindGO<Stage>("stage");
	m_gameCamera = FindGO<GameCamera>("gameCamera");
	m_skyCube = FindGO<SkyCube>("skyCube");
	m_inGameSound = FindGO<SoundManager>("soundmanager");

	

	/** スコアの生成 */
	m_score = NewGO<Score>(0, "score");

	/** 牛の救出数クラスの生成 */
	m_cowNumberOfRescues = NewGO<CowNumberOfRescues>(0, "cownumberofrescues");

	/** タイマーの生成 */
	m_timer = NewGO<Timer>(0, "timer");

	/** ポーズの生成 */
	m_pause = NewGO<Pause>(0, "pause");
	m_pause->Deactivate();

	/** マップを生成 */
	m_map = NewGO<Map>(0, "map");

	/** コンボを生成 */
	m_combo = NewGO<Combo>(0, "combo");

	/** 時間を増やすクラスの生成 */
	m_addTimerUI = NewGO<AddTimerUI>(0, "addTimerUI");

	/** カウントダウンはロード側で作っているなら FindGO、ゲーム側でだけなら NewGO */
	m_countDown = NewGO<CountDown>(0, "countdown");

	/** UFO は名前（またはインデックス）で取得 */
	for (int i = 0; i < EnUFO_Num; i++)
	{
		m_UFO[i] = FindGO<UFO>(UFO_INFOMATIONS[i].objectName.c_str());
	}

	m_isSound = false;
	m_isDead = false;
	m_spawnTimer = 0.0f;

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
	if(m_combo->GetComboTimer() > 0.0f)
	{
		m_combo->DecreaseComboTimer(g_gameTime->GetFrameDeltaTime());
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
	if (m_timer->GetTimer() <= 0.0f && m_cowNumberOfRescues->GetNumberOfRescues() >= 10)
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

	if (m_timer->GetTimer() <=0.0f &&m_cowNumberOfRescues->GetNumberOfRescues( ) < 10)
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
	if (m_aliveCows.size() < COW_NUM)
	{
		m_spawnTimer += g_gameTime->GetFrameDeltaTime();

		// 3秒ごとに1体補充
		if (m_spawnTimer >=NEW_SPAWN_TIMER)
		{
			m_spawnTimer = 0.0f;

			// 新しい牛を生成
			Cow* newCow = NewGO<Cow>(0, "cow");

			// スポーン位置（例：ランダム）
			Vector3 pos;
			pos.x = (rand() % RANDOM_SPAWN_RANGE_DOUBLE) - RANDOM_SPAWN_RANGE; // -300〜300
			pos.y = 0.0f;
			pos.z = (rand() % RANDOM_SPAWN_RANGE_DOUBLE) - RANDOM_SPAWN_RANGE; // -300〜300

			newCow->SetPosition(pos);

			// 生きている牛リストに追加
			m_aliveCows.push_back(newCow);
		}
	}

}

void Game::Render(RenderContext& rc)
{

}