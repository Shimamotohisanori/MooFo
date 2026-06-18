#include "stdafx.h"
#include "UFO.h"
#include "CountDown/CountDown.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "GameScene/Game.h"
#include "CowNumberOfRescues/CowNumberOfRescues.h"
#include "Rope/Rope.h"
#include "GameCamera/GameCamera.h"
#include "Score/Score.h"
#include "Source/Actor/Character/UFO/CowCaptureController.h"
#include "Pause/Pause.h"
#include "UFOLightManager.h"
#include "Combo/Combo.h"
#include "SoundManager/SoundManager.h"
#include "Source/Actor/Character/Player/Player.h"
#include "EffectManager/EffectManager.h"
namespace
{
	/** UFOのモデルファイルパス */
	const char* UFO_MODEL_FILEPATH = "Assets/modelData/UFO/UFO.tkm"; //enModelUpAxis = enModelUpAxisZ;

	/** UFOの大きさ*/
	constexpr float UFO_SCALE = 3.5f;

	/** UFOの移動方向を決めるための乱数の範囲 */
	constexpr int   MOVE_DIR_RANGE = 3;

	/** UFOの移動時間と休む時間 */
	constexpr float REST_TIME_SEC = 60.0f;
	constexpr float MOVE_TIME_SEC = 120.0f;

	/** UFOの回転の閾値 */
	constexpr float ROTATION_THRESHOLD = 0.0001f;

	/** 牛を連れていくときのUFOの高さと牛を連れていく速さ */
	constexpr float CAPTURE_HEIGHT = 150.0f;
	constexpr float CAPTURE_SPEED = 0.2f;

	/** 牛を見つけるときの距離 */
	constexpr float CAPTURE_RANGE = 30.0f;

	/**移動可能エリア（柵の内側）*/
	constexpr float AREA_MIN_X = -1300.0f;
	constexpr float AREA_MAX_X = 1300.0f;
	constexpr float AREA_MIN_Z = -600.0f;
	constexpr float AREA_MAX_Z = 500.0f;

	/** UFO同士の回避処理のための距離の二乗と力 */
	constexpr float UFO_AVOID_RANGE_SQ = 1500.0f;
	constexpr float UFO_AVOID_FORCE = 1.5f;

	/** UFO同士の最低距離*/
	constexpr float MIN_DIST = 300.0f; 

	/** UFO同士の最低距離の二乗*/
	constexpr float MIN_DIST_SQ = MIN_DIST * MIN_DIST;

	constexpr float UFO_PLAYER_DISTANCE = 1000.0f;
}


UFO::UFO()
{

}


UFO::~UFO()
{	
	UFOLightManager* manager = FindGO<UFOLightManager>("ufolightmanager");
	if (manager)
	{
		manager->UnregisterUFO(this);
	}

	/** UFOの混乱エフェクトを停止 */
	if (m_UFOConfusionEffect)
	{
		if (!m_UFOConfusionEffect->IsDead())
		{
			m_UFOConfusionEffect->Stop();
			DeleteGO(m_UFOConfusionEffect);
		}
		m_UFOConfusionEffect = nullptr;
	}

	/** SEを消す */
	if (m_UFOCaptureSE)
	{
		DeleteGO(m_UFOCaptureSE);
		m_UFOCaptureSE = nullptr;
	}
}


bool UFO::Start()
{	
	/** マネージャーに自分を登録する*/	
	UFOLightManager* manager = FindGO<UFOLightManager>("ufolightmanager");
	if (manager)
	{
		manager->RegisterUFO(this);
	}	

	/** コントローラーに自分自身をセット */
	m_cowCaptureController.SetUFO(this);

	m_ufomodelRender.SetRaytracingWorld(false);
	m_ufomodelRender.Init(UFO_MODEL_FILEPATH);
	m_ufomodelRender.SetScale(UFO_SCALE,UFO_SCALE,UFO_SCALE);

	/** 出現演出中なら上方の開始位置をモデルに反映する */
	if (m_UFOState == EnUFOState_Spawning && m_spawnStartPos.LengthSq() > 0.0f)
	{
		m_ufomodelRender.SetPosition(m_spawnStartPos);
	}
	else
	{
		m_ufomodelRender.SetPosition(m_transform.GetPosition());
	}
	m_ufomodelRender.Update();
	
	return true;
}

void UFO::StartSpawnAnimation(const Vector3& finalPos)
{
	m_spawnEndPos = finalPos;
	/** 出現開始位置は画面上方に設定 */
	m_spawnStartPos = finalPos;
	/** 好みに応じて調整*/
	m_spawnStartPos.y += 2000.0f;

	/** スポーン位置を先に確定させる*/
	m_spawnPos = finalPos;
	/** 上の位置に配置*/
	SetPosition(m_spawnStartPos);

	m_spawnTimer = SPAWN__DURATION;
	m_UFOState = EnUFOState_Spawning;
}



void UFO::Update()
{
	/* UFOのサウンドを更新する関数 */
	UpdateUFOSound();

	/** アップデートできるかどうかを判断する */
	if (!CanUFOUpdate())
	{
		return;
	}
	
	/** 出現演出中は通常処理をスキップ */
	if (m_UFOState == EnUFOState_Spawning)
	{
		UpdateSpawning();
		/** 降下中は光を消しておく*/
		m_cowCaptureController.SyncState(CowCaptureController::Wait);
		/** モデルの位置と回転を更新 */
		m_ufomodelRender.SetPosition(m_transform.GetPosition());
		m_ufomodelRender.SetRotation(m_transform.GetRotation());
		m_ufomodelRender.Update();
		return;
	}
	/** 上昇処理中は通常処理を飛ばす */
	if (m_isConfusedAscemding)
	{
		ConfusedAscent();

		/** エフェクト位置を追従*/
		if (m_UFOConfusionEffect)
		{
			Vector3 effectPos = m_transform.GetPosition();
			effectPos.y += 350.0f;
			m_UFOConfusionEffect->SetPosition(effectPos);
		}
		m_ufomodelRender.SetPosition(m_transform.GetPosition());
		m_ufomodelRender.SetRotation(m_transform.GetRotation());
		m_ufomodelRender.Update();
		return;
	}
	if (m_UFOState == EnUFOState_Move)
	{
		/** 移動 */
		Move();
		
		/** 回転 */
		Rotation();
	}


	if (m_UFOConfusionEffect)
	{
		Vector3 effectPos = m_transform.GetPosition();
		effectPos.y += 350.0f;
		m_UFOConfusionEffect->SetPosition(effectPos);
	}
	/** 牛捕獲コントローラーの更新 */
	m_cowCaptureController.Update();

	/**　光が出ているときだけ探す */
	if (IsLightEmitting())
	{
		/** 牛を見つける関数 */
		FindTheCow();
	}
	
	/** 牛を連れていく関数 */
	TakeAwayTheCow();

	/** もしUFOが牛を引っ張るSEが流れていたら */
	if (m_UFOCaptureSE)
	{
		/** UFOが牛を引っ張る際のSE関数 */
		UFOSEDistance();
	}
	/** モデルの位置を反映 */
	m_ufomodelRender.SetPosition(m_transform.GetPosition());

	/** モデルの回転を反映 */
	m_ufomodelRender.SetRotation(m_transform.GetRotation());
	
	/** モデルの更新 */
	m_ufomodelRender.Update();
}
		

void UFO::Move()
{
	/** 牛を連れていってる最中は移動させない */
	if(m_isCowTakeAwayed)
	{
		return;
	}

	Vector3 pos = m_transform.GetPosition();

	/** 追尾中なら近くの牛の方向に少しずつ進む */
	if (m_isChasing)
	{
		/** 現在の位置に少しだけ移動量を足している */
		pos += m_moveDir * m_moveSpeed * g_gameTime->GetFrameDeltaTime();

		/** 追跡中でも反発処理を行う */
		ApplyUFOAvoidance(pos);
		/** UFOは常に地面と同じ高さを保つ */
		pos.y = m_spawnPos.y;

		/** 計算した新しい位置を、実際のUFOに反映する */
		m_transform.SetPosition(pos);
		return;
	}

	if (m_moveTimer <= 0)
	{
		Vector3 dir
		(
			/** (0, 1, 2, から - 1を引いているので) - 1, 0, 1の範囲でランダムな値を生成 */
			rand() % MOVE_DIR_RANGE - 1,/** x */
			0,                          /** yは常に0 */
			rand() % MOVE_DIR_RANGE - 1 /** z */
		);

		/** 0, 0, 0になったら一秒休む */
		if (dir.LengthSq() == 0)
		{
			m_moveDir = Vector3::Zero;
			m_moveTimer = REST_TIME_SEC;/** 1秒休む */
		}

		else
		{
			dir.Normalize();
			m_moveDir = dir;
			m_moveTimer = MOVE_TIME_SEC;/** 2秒ごとに方向を変える */
		}
	}

	/** ランダム方向へ移動 */
	pos += m_moveDir * m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	
	/** 通常移動でも反発処理を行う */
	ApplyUFOAvoidance(pos);

	/** 制限区域内に収める */
	ClampToArea(pos);

	/** 計算した新しい位置を、実際のUFOに反映する */
	m_transform.SetPosition(pos);

	/** モデルに位置を反映 */
	m_ufomodelRender.SetPosition(m_transform.GetPosition());
	
	/** 制限区域内に収め
	/** タイマーを減らす */
	m_moveTimer--;

}
	

void UFO::Rotation()
{
	/** 牛を連れていってる最中は回転させない */
	if(m_isCowTakeAwayed)
	{
		return;
	}

	/** 少しでも動いたら移動方向に向きを回転させる */
	if (fabsf(m_moveDir.x) >= ROTATION_THRESHOLD || fabsf(m_moveDir.z) >= ROTATION_THRESHOLD)
	{
		/** 移動方向に回転させる */
		m_transform.GetRotation().SetRotationYFromDirectionXZ(m_moveDir);
		m_transform.SetRotation(m_transform.GetRotation());
	}
}


void UFO::TakeAwayTheCow()
{
	/** 牛を連れていけるかどうかのフラグが立っているか
	* 牛本体がそもそも空ではなかったら処理をする */
	if (!m_isCowTakeAwayed or
		m_targetCow == nullptr) return;

	/** 削除予定済みの牛または死亡した牛には触らない */
	if (m_targetCow->GetIsPendingKill() || m_targetCow->GetIsDeadFlag())
	{
		m_targetCow = nullptr;
		m_isCowTakeAwayed = false;
		m_isChasing = false;
		return;
	}

	/** 牛を回転状態にする */
	m_targetCow->ChangeRotationState();

	/** UFOの位置に牛を近づける */
	Vector3 cowPos = m_targetCow->GetPosition();
	Vector3 ufoPos = m_transform.GetPosition();
	ufoPos.y += CAPTURE_HEIGHT;

	Vector3 dir = ufoPos - cowPos;
	float dist = dir.Length();

	/** 牛とUFOの距離が0.5以上だったら牛をUFOに近づける */
	if (dist > 0.5f)
	{
		m_targetCow->TakeAwayedByUFO(dir, CAPTURE_SPEED);
	}

	//** 牛とUFOの距離が0.5未満だったら牛を削除し、スコアを減らす */
	else
	{
		/** 牛が連れ去られたらスコアを減らす処理 */
		m_score->DecreaseScore(100);

		/** 牛の状態を連れていかれる前の状態に戻す */
		m_targetCow->SetIsTakeAwayed(false);

		if (auto rope = FindGO<Rope>("rope"))
		{
			if (rope->GetHitCow() == m_targetCow)
			{
				rope->SetIsHitCow(false);
				rope->SetHitCow(nullptr);
			}
		}

		if (auto camera = FindGO<GameCamera>("gameCamera"))
		{
			camera->SetIsCowCaptured(false);
		}
		
		/** Gameに通知してaliveCowsから外す */
		Game* game = FindGO<Game>("game");
		Combo* combo = FindGO<Combo>("combo");
		if (game && combo)
		{
			combo->ResetCombo();
			game->ReMoveCow(m_targetCow);
		}

		/** 牛を削除予定に入れる */
		m_targetCow->SetIsDeadFlag(true);
		m_targetCow->RequestKill();

		/** 状態をリセットする */
		m_targetCow = nullptr;
		m_isCowTakeAwayed = false;
		m_isChasing = false;
		m_failCount = 0;

		return;
	}

}


void UFO::PlayEffect()
{
	/** 混乱エフェクトが再生中かつ、死亡していなかったら*/
	if(m_UFOConfusionEffect&& !m_UFOConfusionEffect->IsDead())
	{
		m_UFOConfusionEffect->Stop();
		DeleteGO(m_UFOConfusionEffect);
		m_UFOConfusionEffect = nullptr;
	}
	/** 失敗回数を増加させる(最大三回)*/
	m_failCount++;

	if (m_failCount > 3)
	{
		m_failCount = 3;
	}

	/** 失敗回数に応じてエフェクトを変える*/
	EffectID effectID = EffectID::EffectID_UFOConfusionEffect_1;
	switch (m_failCount)
	{
	case 1:
		effectID = EffectID::EffectID_UFOConfusionEffect_1;
		break;
	case 2:
		effectID = EffectID::EffectID_UFOConfusionEffect_2;
		break;
	case 3:
		effectID = EffectID::EffectID_UFOConfusionEffect_3;
		break;
	}

	/** エフェクトを再生する*/
	m_UFOConfusionEffect = NewGO<nsK2EngineLow::EffectEmitter>(0);
	m_UFOConfusionEffect->Init((int)effectID);
	m_UFOConfusionEffect->SetScale({ 10.0f, 10.0f, 10.0f });
	m_UFOConfusionEffect->Play();

	/** 三回失敗すると上昇する*/
	if (m_failCount == 3)
	{
		m_isConfusedAscemding = true;
		m_ascentTimer = ASCENT_TIME;
	}

}

void UFO::ConfusedAscent()
{
	/** 混乱上昇中でなければ処理しない */
	if (!m_isConfusedAscemding)
	{
		return;
	}
	/** 上昇処理 */
	Vector3 pos = m_transform.GetPosition();
	pos.y += ASCENT_SPEED;
	m_transform.SetPosition(pos);
	/** 上昇しているときは光を出さない*/
	m_cowCaptureController.SyncState(CowCaptureController::Wait);

	/** 上昇タイマーを減らす */
	m_ascentTimer--;

	/** 上昇タイマーが0になったら上昇を終了する */
	if (m_ascentTimer <= 0.0f)
	{
		Game* game = FindGO<Game>("game");
		if(game&&m_slotIndex>=0)
		{
			game->RequestUFORespawn(m_slotIndex);
		}
		DeleteGO(this);
	}
}


void UFO::UpdateSpawning()
{
	m_spawnTimer -= g_gameTime->GetFrameDeltaTime();
		/** 0～1に正規化(1～0に減るので反転)*/
		float t = 1.0f - (m_spawnTimer / SPAWN__DURATION);
		t = max(0.0f, min(t, 1.0f));

		/**イーズアウト最初は速く、終わりはゆっくり*/
		float eased = 1.0f - (1.0f - t) * (1.0f - t);

		Vector3 pos;
		pos.x = m_spawnStartPos.x;
		pos.y = m_spawnStartPos.y + (m_spawnEndPos.y - m_spawnStartPos.y) * eased;
		pos.z = m_spawnStartPos.z;
		SetPosition(pos);

		/** 降下完了*/
		if (m_spawnTimer <= 0.0f)
		{
			SetPosition(m_spawnEndPos);
			/** 既存の牛を探すステートに変更*/
			m_UFOState = EnUFOState_Move;
		}
		/** 演出中は通常の動きをスキップ*/
		return;

}


void UFO::UFOSEDistance()
{
	/** SEが再生されていなかったら早期リターンする */
	if (m_UFOCaptureSE == nullptr)
	{
		return;
	}

	/** プレイヤーを見つける */
	auto player = FindGO<Player>("player");

	/** プレイヤーとUFOのポジションの距離を代入する */
	float distance = (player->GetPosition() - m_transform.GetPosition()).Length();

	/** 音量からdistanceと音量が聞こえる距離を割る */
	float volume = 1.0f - (distance / UFO_PLAYER_DISTANCE);

	if (volume < 0.0f)
	{
		volume = 0.0f;
	}

	if (volume > 1.0f)
	{
		volume = 1.0f;
	}

	auto soundManager = FindGO<SoundManager>("soundmanager");

	float managerVolume = soundManager->m_seVolume;

	float finalVolume = volume * managerVolume;

	/** UFOが牛を引っ張るSEをSetVolumeに代入させる */
	if (!m_UFOCaptureSE->IsDead())
	{
		m_UFOCaptureSE->SetVolume(finalVolume);
	}
}

CowCaptureController* UFO::GetCowCaptureController()
{
	return &m_cowCaptureController;
}


void UFO::FindTheCow()
{
	/** 光が出ていないときは探さないようにする */
	if (!m_cowCaptureController.GetIsEmitting()) return;
	
	/** 牛を連れていけるかどうかのフラグが立っていたら処理しない */
	if (m_isCowTakeAwayed) return;

	auto cow = FindGOs<Cow>("cow");

	/** 最も近い牛 */
	Cow* nearestCow = nullptr;

	/** 最も近い牛を見つけるために
	 * FLT_MAXを利用して一度最大数にしておく */
	float nearestDistSq = FLT_MAX;

	/** 牛を一匹ずつ見る */
	for (auto c : cow)
	{
		/** 削除予定済みの牛は無視 */
		if (c->GetIsPendingKill() || c->GetIsDeadFlag())
		{
			continue;
		}

		/** すでに他のUFOが狙っている牛は無視 */
		if (c->GetTakingUFO() != nullptr && c->GetTakingUFO() != this)
		{
			continue;
		}

		/** 連れ去られている牛も無視 */
		if (c->GetIsTakeAwayed())
		{
			continue;
		}

		/** UFOと牛の距離を計算 */
		Vector3 diff = c->GetPosition() - m_transform.GetPosition();
		
		/** y軸は考慮しない */
		diff.y = 0.0f;

		float distSq = diff.LengthSq();

		if (distSq < nearestDistSq)
		{
			nearestDistSq = distSq;
			nearestCow = c;
		}
	}

	/** 近くに牛が見つかっているかどうか */
	if (nearestCow)
	{
		/** 追尾開始した瞬間に TakingUFO をセットする */
		if (nearestCow->GetTakingUFO() == nullptr)
		{
			nearestCow->SetTakingUFO(this);
		}

		/**  追尾中だったらtrue */
		m_isChasing = true;

		/** 牛の方向を作る */
		Vector3 dir = nearestCow->GetPosition() - m_transform.GetPosition();
		dir.Normalize();

		/** dirの方向に進むようにセットする */
		m_moveDir = dir;


		/** もし牛を牛を見つけるときの距離のほうが大きかったら */
		if (sqrtf(nearestDistSq) < CAPTURE_RANGE)
		{
			/** この牛を捕まえる(true) */
			m_isCowTakeAwayed = true;
			m_targetCow = nearestCow;

			/** いま追っている牛をtrueにして他のUFOは追尾しないようにする */
			m_targetCow->SetIsTakeAwayed(true);
		}
	}

	/** それいがいは追尾しない */
	else
	{
		m_isChasing = false;
	}

}


void UFO::ApplyUFOAvoidance(Vector3& pos)
{
	Game* game = FindGO<Game>("game");
	if (!game) return;

	/** UFOのリストを取得 */
	auto ufos = game->GetUFOs();

	
	for (auto u : ufos)
	{
		/** 自分自身はスキップ */
		if (u == this) continue;

		/** 自分と他のUFOの距離を計算 */
		Vector3 otherPos = u->m_transform.GetPosition();

		Vector3 diff = pos - otherPos;
		diff.y = 0.0f;

		/** UFO同士の距離の二乗 */
		float distSq = diff.LengthSq();

		/** 近すぎたら反発する */
		if (distSq < MIN_DIST_SQ)
		{
			/** 反発する方向を計算 */
			float dist = sqrtf(distSq);

			/**	正規化 */
			if (dist == 0.0f)
			{
				/** 永遠に重ならないようにランダム関数でどっちか逃がす */
				diff = Vector3(rand() % 3 - 1, 0, rand() % 3 - 1);
				diff.Normalize();
			}
			else
			{
				diff /= dist;
			}

			/** 反発する力を計算 */
			float pushBack = (MIN_DIST - dist) * 1.0f;

			pos += diff * pushBack;
			/** UFOは常に地面と同じ高さを保つ */
			pos.y = m_spawnPos.y;
			/** 方向ベクトルの少しだけ補正を掛ける */
			Vector3 newDir = m_moveDir + diff * 0.3f;
			newDir.Normalize();
			m_moveDir = newDir;
		}
	}
}


void UFO::ClampToArea(Vector3& pos)
{
	/** UFOが制限区域を出ないようにする */
	if (pos.x < AREA_MIN_X)
	{
		pos.x = AREA_MIN_X;
		m_moveDir.x *= -1;
	}
	else if (pos.x > AREA_MAX_X)
	{
		pos.x = AREA_MAX_X;
		m_moveDir.x *= -1;
	}

	if (pos.z < AREA_MIN_Z)
	{
		pos.z = AREA_MIN_Z;
		m_moveDir.z *= -1;
	}
	else if (pos.z > AREA_MAX_Z)
	{
		pos.z = AREA_MAX_Z;
		m_moveDir.z *= -1;
	}
}

bool UFO::CanUFOUpdate()
{
	m_game = FindGO<Game>("game");
	m_countdown = FindGO<CountDown>("countdown");
	m_pause = FindGO<Pause>("pause");
	m_score = FindGO<Score>("score");

	/** UFOがゲームシーンに存在していないときは処理をしない */
	if (m_pause == nullptr || m_countdown == nullptr || m_game == nullptr || m_game->IsDead())
	{
		return false;
	}

	/** タイムアウトしているときはUFOを動かさない */
	if (m_game->GetIsTimeOut())
	{
		return false;
	}

	/** ポーズ中はUFOを動かさない */
	if (m_pause->GetIsPause())
	{
		return false;
	}

	/** カウントダウン中はUFOを動かさない */
	if (m_countdown->GetCountDown())
	{
		return false;
	}

	return true;
}


void UFO::UpdateUFOSound()
{
	Game* game = FindGO<Game>("game");
	Pause* pause = FindGO<Pause>("pause");
	/** タイムアウトしていれば */
	if (game && game->GetIsTimeOut())
	{
		if (m_UFOCaptureSE != nullptr && !m_UFOCaptureSE->IsDead())
		{
			DeleteGO(m_UFOCaptureSE);
			m_UFOCaptureSE = nullptr;
		}

		return;
	}

	/** ポーズ中かつSEが存在していたら */
	if (pause && pause->GetIsPause() && m_UFOCaptureSE != nullptr)
	{
		/** SEを消す */
		DeleteGO(m_UFOCaptureSE);
		m_UFOCaptureSE = nullptr;
	}

	/** 牛を捕まえていなくてかつSEが存在していたら */
	if (!m_isCowTakeAwayed && m_UFOCaptureSE != nullptr)
	{
		/** SEを消す */
		DeleteGO(m_UFOCaptureSE);
		m_UFOCaptureSE = nullptr;
	}

	/** ポーズ中かつ牛を捕まえているかつSEが存在していなかったら */
	if (pause && !pause->GetIsPause() && m_isCowTakeAwayed && !m_UFOCaptureSE)
	{
		/** SEを再生させる */
		auto soundManager = FindGO<SoundManager>("soundmanager");
		m_UFOCaptureSE = soundManager->PlayingSE(SoundSE::enUFOCaptureSE, true);
	}
	/** 牛を連れ去った瞬間のSEを再生 */
	if (m_isCowTakeAwayed || m_targetCow != nullptr)
	{
		if (!m_hasCryed && m_isCowTakeAwayed)
		{
			auto player = FindGO<Player>("player");
			float distance = (player->GetPosition() - m_transform.GetPosition()).Length();
			float volume = max(0.0f, 1.0f - (distance / UFO_PLAYER_DISTANCE));

			auto soundmanager = FindGO<SoundManager>("soundmanager");
			auto se = soundmanager->PlayingSE(SoundSE::enCowCrySE, false);
			se->SetVolume(volume * soundmanager->m_seVolume);

			/** 鳴いた */
			m_hasCryed = true;
		}
		
	}

	else
	{
		/** 牛が鳴いていない場合 */
		m_hasCryed = false;
	}
	
}


void UFO::Render(RenderContext& rc)
{
	m_ufomodelRender.Draw(rc);
}

