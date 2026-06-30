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
#include "GameTimer/AddTimerUI.h"
#include "Source/Actor/Character/UFO/UFOLightUI.h"
#include "Source/Actor/Stage/CowFood.h"
#include "Source/Actor/Stage/CowFoodManager.h"
#include "Source/Actor/Character/Cow/CowLuring.h"
#include "FadeManager/FadeManager.h"
#include "Aiming/Aiming.h"

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

	
	m_aliveCows.clear();

	/** プレイヤーを削除 */
	DeleteGO(m_player);

	/** タイマーを削除 */
	DeleteGO(m_timer);

	/** スコアを削除 */
	if (m_score && !m_score->IsDead())
	{
      DeleteGO(m_score);
	  m_score = nullptr;
	}

	/** ゲームカメラを削除 */
	DeleteGO(m_gameCamera);
	
	/** 空の削除 */
	DeleteGO(m_skyCube);

	/** カウントダウンの削除 */
	DeleteGO(m_countDown);

	/** ポーズの削除 */
	DeleteGO(m_pause);

	/** 牛の救出数の削除 */
	if (m_cowNumberOfRescues && !m_cowNumberOfRescues->IsDead())
		DeleteGO(m_cowNumberOfRescues);
	
	/** ミニマップの削除 */
	DeleteGO(m_map);

	/** タイマー追加UIの削除 */
	DeleteGO(m_addTimerUI);

	/** UFOのライトUI削除 */
	DeleteGO(m_ufoLightUI);

	/** 牛の餌削除 */
	DeleteGO(m_cowFood);

	/** 牛の餌の管理削除 */
	DeleteGO(m_cowFoodManager);

	/** 牛の餌のモデル削除 */
	DeleteGO(m_cowLuring);

	/** 照準の削除 */
	DeleteGO(m_aiming);

	/** コンボクラスの削除 */
	if (m_combo && !m_combo->IsDead())
	{
		DeleteGO(m_combo);
		m_combo = nullptr;
	}

	if (m_fadeManager && !m_fadeManager->IsDead())
	{
		DeleteGO(m_fadeManager);
		m_fadeManager = nullptr;
	}
}
bool Game::Start()
{
	/** ロード画面で生成済みのオブジェクトを参照するだけにする */
	m_player = FindGO<Player>("player");
	m_stage = FindGO<Stage>("stage");
	m_gameCamera = FindGO<GameCamera>("gameCamera");
	m_skyCube = FindGO<SkyCube>("skyCube");
	m_inGameSound = FindGO<SoundManager>("soundmanager");
	m_cowFood = FindGO<CowFood>("cowfood");
	m_cowFoodManager = FindGO<CowFoodManager>("cowfoodmanager");
	m_cowLuring = FindGO<CowLuring>("cowluring");
	

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

	/** UFOのライトUIを生成 */
	m_ufoLightUI = NewGO<UFOLightUI>(0, "ufoLightUI");
  
	/** 照準を生成 */
	m_aiming = NewGO<Aiming>(0, "aiming");

	/** フェードのマネージャーを生成*/
	m_fadeManager = NewGO<FadeManager>(0, "fadeManager");	

	m_isSound = false;
	m_spawnTimer = 0.0f;

	m_timeOutImage.Init("Assets/sprite/NumberUI/TimeOut.DDS", 100, 100);
	m_timeOutImage.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_timeOutImage.Update();
	
	return true;
}


void Game::Update()
{
	
	/** 生きている牛のリストをループして牛が存在するか確認する */
	for (auto it = m_aliveCows.begin(); it != m_aliveCows.end();)
	{
		Cow* cow = *it;
		if (!cow || cow->GetIsDeadFlag())
		{
			it = m_aliveCows.erase(it);
		}

		else
		{
			++it;
		}
	}

	/** 音楽が再生されていない場合 */
	if (!m_isSound)
	{
		m_inGameBGM = m_inGameSound->PlayingBGM(SoundBGM::enInGameBGM, true);
		m_isSound = true;
	}

	/** ポーズ中はゲーム処理をしない */
	if (m_pause->IsActive())
	{
		return;
	}

	/** セレクトボタンを押していて
	 * カウントダウン中でないかつ
	 * タイムアウトしていない場合かつ
	 *　５秒前になっていなかったら*/
	if (g_pad[0]->IsTrigger(enButtonSelect)
		&& !m_countDown->GetCountDown()
		&& !m_isTimeOut
		&& !m_timer->IsFiveCountDown())
	{
		/** タイムアウトなら */
		if (m_isTimeOut)
		{
			return;
		}

		/** 決定ボタンのSEを再生する。 */
		m_pauseSE = m_inGameSound->PlayingSE(SoundSE::enPauseSE, false);
		DeleteGO(m_inGameBGM);

		/** ポーズ画面をアクティブにする */
		m_pause->Activate();
		m_pause->SetIsPause(true);

	}

	/** 牛を生む関数 */
	SpawnCow();


	UpdateUFORespawn();
	/* タイムアウト処理 */
	TimeOut();

	/** 毎フレームロープに最新の牛リストを渡す */
	Rope* rope = FindGO<Rope>("rope");
	if (rope)
	{
		rope->SetCowList(m_aliveCows);
	}

	/** ３秒以内に次の牛を救出できなければコンボが途切れたら */
	if(m_combo->GetComboTimer() > 0.0f)
	{
		m_combo->DecreaseComboTimer(g_gameTime->GetFrameDeltaTime());
	}
}


void Game::Clear()
{
	/** 牛の餌を全て消す */
	if (m_cowFoodManager)
	{
		m_cowFoodManager->ClearAllFood();
	}

	/** ここでスコアを取得する */
	int ClearfinalScore = m_score ? m_score->GetScore() : 0;

	/** 牛の救出数を取得する変数 */
	int ClearfinalRescue = m_cowNumberOfRescues ? m_cowNumberOfRescues->GetNumberOfRescues() : 0;

	/** スコアと牛の救出数を削除することによって二重削除を防ぐ */
	if (m_score && !m_score->IsDead())
	{
		DeleteGO(m_score);
		m_score = nullptr;
	}

	if (m_cowNumberOfRescues && !m_cowNumberOfRescues->IsDead())
	{
		DeleteGO(m_cowNumberOfRescues);
		m_cowNumberOfRescues = nullptr;
	}

	/** 残っている牛を全部消す */
	for (auto cow : m_aliveCows)
	{
		if (cow && !cow->GetIsPendingKill() && !cow->GetIsDeadFlag())
		{
			DeleteGO(cow);
		}
	}
	m_aliveCows.clear();

	/** ゲームクリアの画像を呼び出す */
	m_gameClear = NewGO<GameClear>(0, "gameClear");

	/** ゲームクリアの画像にスコアを渡す */
	m_gameClear->SetFinalClearScore(ClearfinalScore);
	
	/** ゲームクリアの画像に牛の救出数を渡す */
	m_gameClear->SetFinalClearRescue(ClearfinalRescue);

	DeleteGO(m_inGameBGM);
	
	DeleteGO(this);
	return;
}

void Game::SetUFOList(const std::vector<UFO*>&ufos)
{
	/** UFOの配列に引数で渡されたUFOのリストをセットする */
	for (int i = 0; i < ufos.size() && i < EnUFO_Num; i++)
	{
		m_UFO[i] = ufos[i];
		if (m_UFO[i])
		{
			m_UFO[i]->SetSlotIndex(i);
		}

	}
}
void Game::Death()
{

	/** 牛の餌を全て消す */
	if (m_cowFoodManager)
	{
		m_cowFoodManager->ClearAllFood();
	}

	/** ここでスコアを取得 */
	int finalScore = m_score ? m_score->GetScore() : 0;

	/** 牛の救出数を取得する変数 */
	int finalRescue = m_cowNumberOfRescues ? m_cowNumberOfRescues->GetNumberOfRescues() : 0;

	/** スコアと牛の救出数を削除することによって二重削除を防ぐ */
	if (m_score && !m_score->IsDead())
	{
		DeleteGO(m_score);
		m_score = nullptr;
	}

	if (m_cowNumberOfRescues && !m_cowNumberOfRescues->IsDead())
	{
		DeleteGO(m_cowNumberOfRescues);
		m_cowNumberOfRescues = nullptr;
	}

	/** 残っている牛を全部消す */
	for (auto cow : m_aliveCows)
	{
		if (cow && !cow->GetIsPendingKill() && !cow->GetIsDeadFlag())
		{
			DeleteGO(cow);
		}
	}
	m_aliveCows.clear();

	/** ゲームオーバーの画像を呼び出す */
	m_gameOver = NewGO<GameOver>(0, "gameover");
	
	/** ゲームオーバーの画像にスコアを渡す */
	m_gameOver->SetFinalScore(finalScore);
	
	/** ゲームオーバーの画像に牛の救出数を渡す */
	m_gameOver->SetFinalRescue(finalRescue);
	
	DeleteGO(m_inGameBGM);
	
	DeleteGO(this);
	return;
}

void Game::ReMoveCow(Cow* cow)
{
	// Rope にも通知
	Rope* rope = FindGO<Rope>("rope");
	if (rope && rope->GetHitCow() == cow)
	{
		rope->SetIsHitCow(false);
		rope->SetHitCow(nullptr);
	}
}

void Game::KillAllCows()
{
	/** 生きている牛のリストをループして全ての牛を消す */
	for (auto cow : m_aliveCows)
	{
		if (cow)
		{
			DeleteGO(cow);
		}
	}
	m_aliveCows.clear();
}

void Game::RequestUFORespawn(int slotIndex)
{
	/** スロットをnullptrにしておく(消えたUFOへの参照を切るため)*/
	m_UFO[slotIndex] = nullptr;

	UFORespawnRequest request;
	request.SlotIndex = slotIndex;
	request.RespawnTimer = UFO_RESPAWN_TIME;
	m_ufoRespawnRequests.push_back(request);
}


void Game::UpdateUFORespawn()
{
	/** 復活待ちのUFOが居なければ何もしない*/
	if (m_ufoRespawnRequests.empty())
	{
		return;
	}

	float dt = g_gameTime->GetFrameDeltaTime();

	/** UFOの再出現リクエストをループして処理する */
	m_ufoRespawnRequests.erase(
		std::remove_if(
			m_ufoRespawnRequests.begin(),
			m_ufoRespawnRequests.end(),
			/**  リストの要素を1個ずつ取り出して判定する。*/
			[&](UFORespawnRequest& request)
			{
				request.RespawnTimer -= dt;
				/** タイマーが0以下になったらUFOを再出現させる */
				if (request.RespawnTimer <= 0.0f)
				{
					/** スロット番号の取得*/
					int idx = request.SlotIndex;
					UFO* ufo = NewGO<UFO>(0, "UFO");
					//ufo->SetPosition(UFO_INFOMATIONS[idx].pos);
					ufo->StartSpawnAnimation(UFO_INFOMATIONS[idx].pos);
					/** UFOの情報を設定 */
					ufo->SetSlotIndex(idx);
					/** Gameが持つUFO配列に保存する*/
					m_UFO[idx] = ufo;
				   /** remove_ifはtrue = 消える false = 残る
				   なのでUFOが再出現した場合はtrueを返してリストから削除する */
					return true;
				}
				/** タイマーが0以下になっていない場合はリストに残す */
				return false;
		}
		
	),
		m_ufoRespawnRequests.end()
	);
}


void Game::SpawnCow()
{
	if (m_timer->GetTimer() <= 4.0f || m_isTimeOut)
	{
		/** タイマーが4秒以下なら牛を補充しない */
		return;
	}

	/** 現在の牛の数が10体未満なら補充 */
	if (m_aliveCows.size() < COW_NUM)
	{
		m_spawnTimer += g_gameTime->GetFrameDeltaTime();

		/** 3秒ごとに1体補充 */
		if (m_spawnTimer >=NEW_SPAWN_TIMER)
		{
			
			int currentrescues = m_cowNumberOfRescues->GetNumberOfRescues();

			/** もし牛の救出数が一定以上ならスポーンする範囲を大きくする */
			if (currentrescues >= 10)
			{
				m_difficultyLevelSpawnRange = 600;
			}
			
			else if (currentrescues >= 5)
			{
				m_difficultyLevelSpawnRange = 400;
			}

			else
			{
				m_difficultyLevelSpawnRange = 0;
			}

			m_spawnTimer = 0.0f;

			/** 新しい牛を生成 */
			Cow* newCow = NewGO<Cow>(0, "cow");

			
			/** スポーン位置（例：ランダム） */
			Vector3 pos;

			/** ランダムスポーン範囲に難易度による調整を加える */
			int range = RANDOM_SPAWN_RANGE + m_difficultyLevelSpawnRange;

			/** ランダムな位置を生成する */
			/** 難易度(牛の救出数)に応じてスポーン範囲を調整 */
			pos.x = (rand() % (range * 2)) - range;
			pos.y = 0.0f;
			pos.z = (rand() % (range * 2)) - range;

			/** ここでUFOに向かって歩く牛を生成させる*/
			newCow->SetUFOAttracted(rand() % 2 == 0);
			/** UFOに向かって歩く牛の速度も適応させる*/
			newCow->IsMoving();
			newCow->SetPosition(pos);
			/** 生きている牛リストに追加 */
			m_aliveCows.push_back(newCow);
		}
	}

}

void Game::TimeOut()
{
	/** タイマーとタイムアウトフラグを見る*/
	if (m_timer->GetTimer() < 0.0f && !m_isTimeOut)
	{
		/** タイムアウトフラグを立てる */
		m_isTimeOut = true;

		m_timeOutSE = m_inGameSound->PlayingSE(SoundSE::enTimeOutSE, false);

	}

	/** タイムアウトフラグが立っているなら */
	if (m_isTimeOut)
	{
		m_timeOutTimer += g_gameTime->GetFrameDeltaTime();
		/** タイムアウトから4秒かつフェード処理をしていなかったら*/
		if (m_timeOutTimer > 4.0f && !m_isfadeStart)
		{
			m_fadeManager->SetFadeIn();
			m_isfadeStart = true;
		}
		/** フェードインが終わっていたら*/
		if (m_isfadeStart && m_fadeManager->IsFadeInComplete() && !m_isFadeOut)
		{
			m_fadeManager->SetFadeOut();
			m_isFadeOut = true;
		
			/** ゲームオーバーかゲームクリアかを判断する */
			if (m_cowNumberOfRescues->GetNumberOfRescues() >= 10)
			{
				/** ゲームクリアの処理 */
				Clear();
			}

			else
			{
				/** ゲームオーバーの処理 */
				Death();
			}

			return;
		}

		/** タイムアウト画像の大きさを徐々に大きくする */
		/** 目標のスケールを設定 */
		Vector3 targetScale = Vector3(10.0f, 10.0f, 1.0f);

		m_timeOutImageScale.x += (targetScale.x - m_timeOutImageScale.x) * 0.1f;
		m_timeOutImageScale.y += (targetScale.y - m_timeOutImageScale.y) * 0.1f;

		m_timeOutImage.SetScale(m_timeOutImageScale);

		m_timeOutImage.Update();
	}
}

void Game::Render(RenderContext& rc)
{
	if (IsFadeTimeOut())
	{
		return;
	}

	if (m_isTimeOut)
	{
		m_timeOutImage.Draw(rc);
	}
}