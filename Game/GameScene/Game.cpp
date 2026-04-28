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
	/** カウントダウンの生成 */
	m_countDown = NewGO<CountDown>(0, "countdown");

	/** プレイヤーの生成 */
	m_player = NewGO <Player>(0, "player");

	/** ステージの生成 */
	m_stage = NewGO<Stage>(0, "stage");
	
	/** 牛の生成 */
	for (int i = 0; i < COW_NUM; i++)
	{
		Cow* cow = NewGO<Cow>(0, "cow");

		/** 毎回ランダムな位置を生成する*/
		
		/** ランダムスポーン位置 */ 
		Vector3 randomPos;

		/** ランダムスポーン位置を生成する */
		/** XZ平面の - 300から300の範囲でランダムな位置を生成する */
		randomPos.x = (rand() % RANDOM_SPAWN_RANGE_DOUBLE) - RANDOM_SPAWN_RANGE;
		randomPos.y = 0.0f;
		randomPos.z = (rand() % RANDOM_SPAWN_RANGE_DOUBLE) - RANDOM_SPAWN_RANGE;

		cow->SetPosition(randomPos);
		m_aliveCows.push_back(cow);
	}

	/** UFOの生成 */
	for (int i = 0; i < _countof(UFO_INFOMATIONS); i++)
	{
		m_UFO[i] = NewGO<UFO>(0, UFO_INFOMATIONS[i].objectName.c_str());
		m_UFO[i]->SetPosition(UFO_INFOMATIONS[i].pos);
	}

	/** スコアの生成 */
	m_score = NewGO<Score>(0, "score");

	/** 牛の救出数の生成 */
	m_cowNumberOfRescues = NewGO<CowNumberOfRescues>(0, "cownumberofrescues");

	/** タイマーの生成 */
	m_timer = NewGO<Timer>(0, "timer");

	/** ポーズ画面の生成をするが非アクティブにする */
	m_pause = NewGO<Pause>(0, "pause");
	m_pause->Deactivate();
	
	/** サウンドマネージャーの生成 */
	m_inGameSound = FindGO<SoundManager>("soundmanager");	

	/** ゲームカメラの生成 */
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	/** ミニマップの生成 */
	m_map = NewGO<Map>(0, "map");

	/** コンボの生成 */
	m_combo = NewGO<Combo>(0, "combo");

	/** タイマー追加UIの生成 */
	m_addTimerUI = NewGO<AddTimerUI>(0, "addTimerUI");

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

void Game::Render(RenderContext& rc)
{

}