#include "stdafx.h"
#include "Cow.h"
#include "Rope/Rope.h"
#include "Source/Actor/Character/Player/Player.h"
#include "CountDown/CountDown.h"
#include "GameCamera/GameCamera.h"
#include "CowNumberOfRescues/CowNumberOfRescues.h"
#include "Source/Actor/Character/UFO/UFO.h"
#include "Pause/Pause.h"
#include "Combo/Combo.h"
#include "GameScene/Game.h"
#include"SoundManager/SoundManager.h"
#include "DummyCow.h"
#include "GameTimer/Timer.h"
#include "Source/Actor/Stage/CowFoodManager.h"
#include "EffectManager/EffectManager.h"
#include "CowShrinkHay.h"
#include "CowLuring.h"
#include "SoundManager/VoiceManager.h"
#include "GameTimer/AddTimerUI.h"

namespace
{
	/** 牛のタイプごとのモデルのファイルパス */
	const char* COW_MODEL_PATHS[static_cast<int>(Cow::EnCowType::en_Num)] =
	{
		"Assets/modelData/Cow/Model/Cow5.tkm",
		"Assets/modelData/Cow/Model/LightCow.tkm",
		"Assets/modelData/Cow/Model/ChaseCow.tkm",
		"Assets/modelData/Cow/Model/BonusCow.tkm",
	}; 

	/** 牛のアニメーションのファイルパス */
	const char* IDLE_ANIMATION_FILE_PATH = "Assets/modelData/Cow/Animation/Idle3.tka";

	/** 牛の歩くアニメーションのファイルパス */
	const char* WALK_ANIMATION_FILE_PATH = "Assets/modelData/Cow/Animation/Walk2.tka";

	const Vector3 EFFECT_SCALE = { 10.0f,10.0f,10.0f };

	/** プレイヤーに引っ張られるときの力 */
	constexpr float PULL_POWER = 13.0f;

	/** プレイヤーから逃げるときの力 */
	constexpr float COW_MOVE_LIMIT_RADIUS = 1450.0f;

	/** この距離以内なら逃げる */
	constexpr float AVOID_DISTANCE = 80.0f;

	/** 逃げる強さ */
	constexpr float AVOID_POWER = 5.0f;

	/** ボーナス牛の引っ張り抵抗 */
	constexpr float BONUS_COW_PULL_RESISTANCE = 0.4f;

	/** 牛の移動時間と休憩時間 */
	constexpr int RANDOMCOW_TIMER = 180 + 60;

	/** 納屋の位置 */
	const Vector3 BARN_POSITION = Vector3{ -1320.0f,0.0f,10.0f };

	/** 納屋の半径 */
	constexpr float BARN_RADIUS = 300.0f;

	/** 牛とプレイヤーの距離 */
	constexpr float COW_PLAYER_DISTANCE = 1000.0f;

	/** UFOが牛を捕獲する高さ */
	constexpr float UFO_CATCH_HEIGHT = 400.0f;

	/** 牛の最小スケール */
	constexpr float COW_MIN_SCALE = 0.1f;

	/** 追いかけるスピード */
	constexpr float CHASE_POWER = 1.0f;

	/** これ以上近づいたら停止する距離(プレイヤーにのめりこまないように) */
	constexpr float CHASE_STOP_DISTANCE = 60.0f;

	/** ボーナス牛が逃げ始める距離 */
	constexpr float BONUS_COW_AVOID_DISTANCE = 150.0f;

	/** ボーナス牛の逃げる強さ */
	constexpr float BONUS_COW_AVOID_POWER = 10.0f;

	/** ボーナス牛を救出した際のスコア */
	constexpr int BONUS_COW_SCORE = 900;

	/** ボーナス牛救出時に追加されるタイム */
	constexpr float BONUS_COW_TIME = 3.0f;
}

Cow::Cow()
{
	/** 待機アニメーションを読み込みループ設定する */
	animationClips[EnAnimation_Idle].Load(IDLE_ANIMATION_FILE_PATH);
	animationClips[EnAnimation_Idle].SetLoopFlag(true);
	
	/** 歩きアニメーションを読み込みループ設定する */
	animationClips[EnAnimation_Walk].Load(WALK_ANIMATION_FILE_PATH);
	animationClips[EnAnimation_Walk].SetLoopFlag(true);
}

Cow::~Cow()
{
	/** 牛が餌を食べる音の再生を止める */
	if (m_cowEatSE)
	{
		DeleteGO(m_cowEatSE);
	}
}

bool Cow::Start()
{
	/** サウンドマネージャーを取得する */
	m_CowSound  = FindGO<SoundManager>("soundmanager");

	/** レイトレーシングを無効にする */
	m_cowmodelRender.SetRaytracingWorld(false);

	/** キャラクターコントローラーを初期化する */
	m_cowCharacterController.Init(19.0f, 40.0f, m_transform.GetPosition());

	/** ボイスマネージャーを取得 */
	m_voiceManager = FindGO<VoiceManager>("voicemanager");

	/** カウントダウンの情報を取得*/
	m_countdown = FindGO<CountDown>("countdown");

	return true;
}

void Cow::Update()
{
	
	

	/** 削除予約されているなら */
	if (m_isPendingKill)
	{
		/** Ropeとの紐づけ解除 */
		if (m_rope && m_rope->GetHitCow() == this)
		{
			m_rope->SetIsHitCow(false);
			m_rope->SetHitCow(nullptr);
		}

		/** UFOとの紐づけ解除 */
		/** 連れ去られた牛が削除されるときに
		UFOのフラグをfalseにする */
		if (m_takingUFO && m_isTakeAwayed)
		{
			m_takingUFO->SetIsCowTakeAwayed(false);
			m_takingUFO->ReMoveTargetCow();
			m_takingUFO = nullptr;
		}

		/** Game に「この牛を aliveCows から消して」と伝える */
		if (!m_isDeadFlag)
		{
			if (!m_game)
			{
				m_game = FindGO<Game>("game");
			}
			if (m_game)
			{
				m_game->ReMoveCow(this);
			}
		}
		DeleteGO(this);
		return;
	}


		
	/** アップデートできるかどうかを判断する関数 */
	if (!CanUpdate())
	{
		return;
	}

	/** アニメーションの再生 */
	PlayAnimation();

	if (m_isEating)
	{
		Eating();
		return;
	}
	else
	{
		/** 牛が餌を食べている音の再生を止める */
		if (m_cowEatSE)
		{
			DeleteGO(m_cowEatSE);
			m_cowEatSE = nullptr;
		}
	}

	switch (m_cowType)
	{
	case EnCowType::en_Chase:
		ChasePlayer();
		break;

	case EnCowType::en_Light:
	case EnCowType::en_Random:
	default:
		AvoidPlayer();
		break;

	}

	if (m_cowType != EnCowType::en_Chase)
	{
		/** 一番近い餌を探す処理 */
		SearchNearestFood();

		if (m_isTargetFood)
		{
			MoveToFood();
		}
		else
		{
			if (m_rotationState == EnRotateState_MoveDir)
			{
				/** 移動 */
				Move();
			}
		}
	}
	
	/** 牛が納屋に入る処理 */
	EnterBarn();

	/** ステート管理 */
	ManageState();
	
	/** 回転 */
	Rotation();
	
	/** 牛がプレイヤーに引っ張られる処理 */
	PulledByPlayer();

	/** 牛がプレイヤーに捕獲される処理 */
	CapturedByPlayer();

	/** モデルのキャラコンを反映 */
	Vector3 pos = m_cowCharacterController.GetPosition();

	/** 連れ去られていない場合はY座標を固定する */
	if (!m_isTakeAwayed)
	{

		/** キャラコンのY座標が0.01f以上になったら0に戻す */
		if (pos.y > 0.01f)
		{
			pos.y = 0.0f;
			m_transform.SetPosition(pos);
			m_cowCharacterController.SetPosition(pos);
		}
	}

	m_transform.SetPosition(pos);

	/** モデルの位置を反映 */
	m_cowmodelRender.SetPosition(pos);
	
	/** モデルに回転を反映 */
	m_cowmodelRender.SetRotation(m_transform.GetRotation());

	/** 牛の大きさを更新 */
	UpdateScale();

	/** モデルの更新 */
	m_cowmodelRender.Update();
}


void Cow::Move()
{
	/** 削除予約されている、もしくは死んでいるなら移動しない */
	if (m_isPendingKill || m_isDeadFlag) return;

	if ((m_isTakeAwayed))
	{
		return;
	}
	/** ロープに捕まっているときは移動しない */
	if (m_isCaptured) return;

	if (m_isUFOAttracted)
	{
		if (m_targetUFO != nullptr)
		{
			if (m_targetUFO->IsPendingKill() || !m_targetUFO->IsLightEmitting() ||m_targetUFO->GetIsCowTakeAwayed())
			{
				m_targetUFO = nullptr;

			/** ターゲットを失ったら移動方向と速度をリセットし、即座に休止させる*/
				m_moveDir = Vector3::Zero;
				m_moveTimer = 0.0f;
				m_isMove = false;
			}
		}
		/** まだターゲットになるUFOが決まっていなければここで決める*/
		if (m_targetUFO == nullptr)
		{
			m_targetUFO = FindNearestEmittingUFO();
		}
		/** 決まっていたら、UFOに向かって歩く処理を行う。*/
		if (m_targetUFO != nullptr)
		{
			MoveTowardUFO(m_targetUFO);
			return;
		}
	}

	/** タイマーが0以上なら新しい方向を決める */
	if (m_moveTimer <= 0)
	{
		if (m_isUFOAttracted)
		{
			Vector3 dir
			{
				static_cast <float>(rand() % 3 - 1),
				0.0f,
				static_cast <float>(rand() % 3 - 1)
			};

			/** 0,0,0,になっても休憩させずに別の方向を強制的に与える*/
			if (dir.LengthSq() == 0)
			{
				dir = Vector3(1, 0, 0);
			}
			dir.Normalize();
			m_moveDir = dir * 100.0f;
			m_moveTimer = rand() % RANDOMCOW_TIMER;
			m_isMove = true;
		}
		else if (m_isMove)
		{
			Vector3 dir
			(
				/** (0, 1, 2, から - 1を引いているので) - 1, 0, 1の範囲でランダムな値を生成 */
				rand() % 3 - 1,//x
				0,             //yは常に0
				rand() % 3 - 1//z
			);

			/** 0,0,0になったら一秒休む */
			if (dir.LengthSq() == 0)
			{
				dir = Vector3(1, 0, 0);
			}
			dir.Normalize();
			m_moveDir = dir * 100.0f;
			/** 1～4秒間ランダムに方向を変える */
			m_moveTimer = rand() % RANDOMCOW_TIMER;
			m_isMove = false;
		}

			/** 移動した後に必ず休む */
			else
			{
				m_moveDir = Vector3::Zero;
				/** 1～4秒間のランダム時間に休む */
				m_moveTimer = rand() % RANDOMCOW_TIMER;
				m_isMove = true;
			}
		}
	/** 移動する距離を計算 */
	Vector3 move = m_moveDir * m_moveSpeed * g_gameTime->GetFrameDeltaTime();

	/** キャラクターコントローラーを使って移動 */
	Vector3 pos = m_cowCharacterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	m_transform.SetPosition(pos);
	m_cowmodelRender.SetPosition(pos);

	/** タイマーを減らす */
	m_moveTimer--;
}
void Cow::Rotation()
{
	/** 移動ステートの時は移動方向に回転する。 */
	if (m_rotationState == EnRotateState_MoveDir)
	{
		if (fabsf(m_moveDir.x) >= 0.01f || fabsf(m_moveDir.z) >= 0.01f)
		{
			/** 移動方向に回転させる */
			m_transform.GetRotation().SetRotationYFromDirectionXZ(m_moveDir);
			m_transform.SetRotation(m_transform.GetRotation());
		}
	}
	else if (m_rotationState == EnRotateState_Spin)
	{
		/** 回転ステートがスピンのときは常に回転する。 */
		m_transform.GetRotation().AddRotationDegY(3.0f);
		m_transform.SetRotation(m_transform.GetRotation());
	}
}

UFO* Cow::FindNearestEmittingUFO()
{
	if (m_isTakeAwayed)
	{
		return nullptr;
	}

	/** ゲームが空なら情報を読ませる*/
	if (m_game == nullptr)
	{
		m_game = FindGO<Game>("game");
		/** それでも空ならnullptrを返す*/
		if (m_game == nullptr)
		{
			return nullptr;
		}
	}
	/** UFOのリストを読み込む*/
	std::vector<UFO*>ufos = m_game->GetUFOs();

	UFO* nearestUFO = nullptr;
	float nearestDistsq = FLT_MAX;

	for (auto ufo : ufos)
	{
		/** ufoがnullptrなら処理を飛ばす*/
		if (ufo == nullptr)continue;
		/** ufoの光が出ていなければ処理を飛ばす*/	
		if (!ufo->IsLightEmitting())continue;

		/** 既に連れ去り中のUFOは対象外にする*/
		if (ufo->GetIsCowTakeAwayed())continue;
		float distSq = (ufo->GetPosition() - m_transform.GetPosition()).LengthSq();

		if (distSq < nearestDistsq)
		{
			nearestDistsq = distSq;
			nearestUFO = ufo;
		}
	}

	return nearestUFO;
}

void Cow::MoveTowardUFO(UFO* ufo)
{
	
	Vector3 toUFO = ufo->GetPosition() - m_transform.GetPosition();
	toUFO.y = 0.0f;

	if (toUFO.LengthSq() > 0.0001f)
	{
		toUFO.Normalize();
	}
	/** 回転も入れる*/
	m_moveDir = toUFO * 90.0f;
	m_moving = toUFO * m_moveSpeed * 2.0f;

	Vector3 pos = m_cowCharacterController.Execute(m_moving, g_gameTime->GetFrameDeltaTime());

	m_transform.SetPosition(pos);
	m_cowmodelRender.SetPosition(pos);

	m_isMove = true;
	m_moveTimer = 0.0f;
	
}



void Cow::SearchNearestFood()
{
	/** 牛の餌マネージャーを取得する */
	CowFoodManager* cowfoodmanager = FindGO<CowFoodManager>("cowfoodmanager");

	if (cowfoodmanager == nullptr)
	{
		return;
	}

	const auto& foodList = cowfoodmanager->GetFoodList();

	/** 餌リストが空なら参照をクリアしてターゲットフラグを下ろす */
	if (foodList.empty())
	{
		m_cowfoodmanager = nullptr;
		return;
	}

	float minDistance = FLT_MAX;

	/** 一番近くの餌を探すため一旦nullにリセットする */
	m_CowLuring = nullptr;

	/** 餌の中から最も近い牛を選ぶ */
	for (auto food : foodList)
	{
		Vector3 dir = food->GetPosition() - GetPosition();

		float distance = dir.Length();

		if (distance < minDistance)
		{
			minDistance = distance;

			m_CowLuring = food;
		}
	}
}

void Cow::MoveToFood()
{
	/** 牛が連れ去られている */
	if (m_isTakeAwayed)
	{
		return;
	}

	/** 別の牛が近くの餌を追いかけている間は処理しない */
	if (!m_isTargetFood)
	{
		return;
	}

	/** 餌がない場合は処理しない */
	if (m_CowLuring == nullptr)
	{
		return;
	}

	Vector3 dir = m_CowLuring->GetPosition() - GetPosition();

	dir.y = 0.0f;

	float distance = dir.Length();

	/** 餌に20.0fに以内まで近づいたら食べる状態に移行する */
	if (distance < 60.0f)
	{
		m_isEating = true;
		return;
	}

	dir.Normalize();
	
	m_moveDir = dir * 100.0f;

	const float speed = 50.0f;

	Vector3 move = dir * speed;

	/** キャラクターコントローラーで移動後の座標を取得する */
	Vector3 newPos = m_cowCharacterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	m_transform.SetPosition(newPos);
	m_cowmodelRender.SetPosition(newPos);
}




void Cow::Eating()
{
	/** 食べている経過時間を加算する */
	m_eatTimer += g_gameTime->GetFrameDeltaTime();

	auto soundmanager = FindGO<SoundManager>("soundmanager");

	/** 牛が餌を食べている音の再生 */
	if (m_cowEatSE == nullptr)
	{
		m_cowEatSE = soundmanager->PlayingSE(SoundSE::enEatCowFoodSE, true);	
	}

	/** 牛が餌を食べている音の距離によるボリューム調整 */
	if (m_cowEatSE)
	{
		auto player = FindGO<Player>("player");
		float distance = (player->GetPosition() - m_transform.GetPosition()).Length();
		float volume = max(0.0f, 1.0f - (distance / COW_PLAYER_DISTANCE));
		m_cowEatSE->SetVolume(volume * soundmanager->m_seVolume);
	}

	/** 一定時間経過したら食べ終わりの処理を行う */
	if (m_eatTimer >= 2.0f)
	{
		/** 満腹状態のエフェクトを再生させる。 */
		m_overdoseEffect = NewGO<nsK2EngineLow::EffectEmitter>(0);
		m_overdoseEffect->Init((int)EffectID::EffectID_Overdose);
		m_overdoseEffect->SetPosition(m_CowLuring->GetPosition());
		m_overdoseEffect->SetScale(EFFECT_SCALE);
		m_overdoseEffect->Play();
		m_overdoseEffect->Update();

		/** 縮小開始する */
		if (m_CowLuring != nullptr)
		{
			//牛の餌を同じ位置にスポーンして縮小を任せる
			CowShrinkHay* shrink = NewGO<CowShrinkHay>(0);
			shrink->SetPosition(m_CowLuring->GetPosition());
			m_CowLuring->StartShrink();
			//餌のマネージャーを消す
			CowFoodManager* mgr = FindGO<CowFoodManager>("cowfoodmanager");
			if (mgr)
			{
				mgr->RemoveFood(m_CowLuring);
			}
			m_CowLuring = nullptr;
		}

		/** 各フラグとタイマーをリセットして通常行動に戻す */
		m_isEating = false;
		m_isMove = false;
		m_eatTimer = 0.0f;
		m_isTargetFood = false;
	}
}

void Cow::EnterBarn()
{
	/** UFOに連れ去られている最中は納屋に入れない */
	if (m_isTakeAwayed) return;

	/** 牛の位置を取得 */
	Vector3 cowpos = m_cowCharacterController.GetPosition();

	/** 納屋はステージに統合されているので
	 * 今回はxz座標を基準にした円形の範囲を作り
	 * その中に入ったら牛が納屋に入れる処理を作る */
	float dx = cowpos.x - BARN_POSITION.x;
	float dz = cowpos.z - BARN_POSITION.z;
	float distanceSquared = dx * dx + dz * dz;

	/** 納屋の半径以内に入ったら納屋に入る */
	/** 牛を削除し、スコアや救出数を増やす */
	if (distanceSquared <= BARN_RADIUS * BARN_RADIUS)
	{

		/** 状態をリセットする */
		m_isDeadFlag = true;

		/** コンボを増やす */
		Combo* combo = FindGO<Combo>("combo");
		m_game = FindGO<Game>("game");
		if (combo)
		{
			combo->AddCombo();
			combo->AddScore(100);
			
			if (m_cowType == EnCowType::en_Bonus)
			{
				combo->AddScore(BONUS_COW_SCORE);

				if (m_timer)
				{
					m_timer->AddTimer(BONUS_COW_TIME);
				}
			}
		}

		/** 牛の救出数を増やす */
		CowNumberOfRescues* cowNumberOfRescues = FindGO<CowNumberOfRescues>("cownumberofrescues");
		if (cowNumberOfRescues)
		{
			cowNumberOfRescues->AddRescue();
		}

		m_dummyCow = NewGO<DummyCow>(0, "dummyCow");

		/** 元の牛と同じモデルを引き継がせる */
		m_dummyCow->SetModelPath(COW_MODEL_PATHS[static_cast<int>(m_cowType)]);
		
		/** dummyCowに牛の情報を渡す*/
		m_dummyCow->SetPosition(m_transform.GetPosition());
		m_dummyCow->SetRotation(m_transform.GetRotation());
		
		/** ジャンプアニメーションを再生*/
		m_dummyCow->PlayJumpAnimtion();

		if (m_game)
		{
			m_game->SetDuumyCow(m_dummyCow);
		}

		RequestKill();
	}
}

void Cow::UpdateScale()
{
	/** 出現中は徐々に大きくなる */
	if (m_isSpawning)
	{
		m_spawnScale += 0.05f;

		/** 1.0倍以上になったら1.0倍に固定して出現中フラグを下ろす */
		if(m_spawnScale >= 1.0f)
		{
			m_spawnScale = 1.0f;
			m_isSpawning = false;
		}

		m_cowmodelRender.SetScale(Vector3(m_spawnScale, m_spawnScale, m_spawnScale));
		return;
	}

	/** UFOに連れ去られていない時はスケールを元に戻す */
	if (!m_isTakeAwayed)
	{
		m_cowmodelRender.SetScale(Vector3(1.0f, 1.0f, 1.0f));
		return;
	}

	/** 牛の現在のY座標を取得 */
	float currentheight = m_transform.GetPosition().y;

	/** 0.0~1.0に正規化 (地上=0.0, UFOが牛を捕獲する高さ=1.0) */
	float t = currentheight / UFO_CATCH_HEIGHT;
	t = max(0.0f, min(1.0f, t));

	/** 地上なら1.0倍、UFOに近づくにつれて0.0倍に近づく */
	float scale = 1.0f - t;
	scale = max(scale, COW_MIN_SCALE);

	m_cowmodelRender.SetScale(Vector3(scale, scale, scale));
}

void Cow::ManageState()
{
	/** ロープに捕まっているときは状態を変えない */
	if (m_isCaptured)
	{
		return;
	}

	/** 歩きの時 */
	if (fabsf(m_moveDir.x) >= 0.0001f || fabsf(m_moveDir.z) >= 0.0001f)
	{
		m_cowState = 1;
	}

	/** 待機の時 */
	else
	{
		m_cowState = 0;
	}
}

void Cow::PulledByPlayer()
{
	/** 捕まってない牛もしくはタイムアウトしている場合は引っ張られない */
	if (!m_isCaptured || m_game->GetIsTimeOut())
	{
		return;
	}

	if (m_player->GetIsRightButton1() or m_player->GetIsLeftButton1())
	{
		/** プレイヤーの位置を取得 */
		Vector3 playerPos = m_player->GetPosition();

		/** 牛の位置 */
		Vector3 cowPos = m_transform.GetPosition();

		/** プレイヤーへの方向 */
		Vector3 dir = playerPos - cowPos;

		/** 正規化 */
		dir.Normalize();

		/** 牛をプレイヤーのいる位置まで徐々に移動 */
		Vector3 move = Vector3::Zero;

		/** ボーナス牛は引っ張りが重くなるようにする */
		move += dir * PULL_POWER * 100.0f * m_pullResistance;
		Vector3 newPos = m_cowCharacterController.Execute(move, g_gameTime->GetFrameDeltaTime());

		m_transform.SetPosition(newPos);

		m_cowmodelRender.SetPosition(newPos);
		m_player->SetGetLeftButton1(false);
		m_player->SetGetRightButton1(false);

		/** 牛の鳴き声効果音を流す*/
		if (m_CowSE == false)
		{
			m_cowCrySE = m_CowSound->PlayingSE(SoundSE::enCowCrySE, false);
			m_CowSE = true;
		}
		
	}
}

void Cow::CapturedByPlayer()
{
	/** ロープが牛に当たっているとき */
	if (m_isCaptured)
	{
		/** 牛とプレイヤーの間の距離を計算 */
		Vector3 playerPos = m_player->GetPosition();
		Vector3 cowPos = m_transform.GetPosition();

		/** プレイヤーへの方向 */
		Vector3 dir = playerPos - cowPos;
		
		
		/** 距離が一定以下なら捕獲される */
		if (dir.Length() < 70.0f)
		{
			m_isDeadFlag = true;

			/** 牛が捕獲されたときのボイスを再生 */
			/** ランダムでボイスを選択 */
			SoundVoice voice = (rand() % 2 == 0) ? SoundVoice::enVoice_Rescue1 : SoundVoice::enVoice_Rescue2;
			m_voiceManager->PlayingVoice(voice, false);

			if (m_takingUFO)
			{
				/** 牛捕獲コントローラーの捕獲フラグを下ろす */
				m_takingUFO->GetCowCaptureController()->SetCapturing(false);
				/** 牛捕獲コントローラーの捕獲終了の処理を行う関数を呼ぶ*/
				m_takingUFO->GetCowCaptureController()->EndCaptured();
			}

			/** カメラの牛捕獲フラグを下ろす */
			GameCamera* camera = FindGO<GameCamera>("gameCamera");
			if (camera)
			{
				camera->SetIsCowCaptured(false);
			}

			/** コンボを増やす */
			Combo* combo = FindGO<Combo>("combo");
			m_game = FindGO<Game>("game");
			if (combo)
			{
				combo->AddCombo();
				combo->AddScore(100);

				if (m_cowType == EnCowType::en_Bonus)
				{
					combo->AddScore(BONUS_COW_SCORE);

					if (m_timer)
					{
						m_timer->AddTimer(BONUS_COW_TIME);
					}

					AddTimerUI* m_addtimerUI = FindGO<AddTimerUI>("addTimerUI");
					if (m_addtimerUI)
					{
						m_addtimerUI->Play();
					}
				}
			}

			/** 牛の救出数を増やす */
			CowNumberOfRescues* cowNumberOfRescues = FindGO<CowNumberOfRescues>("cownumberofrescues");
			if (cowNumberOfRescues)
			{
				cowNumberOfRescues->AddRescue();
			}
			/** エフェクトを呼び出す*/
			m_takingUFO->PlayEffect();

			/** 状態をリセットする */
			m_isTakeAwayed = false;
			
		    m_dummyCow = NewGO<DummyCow>(0, "dummyCow");

			/** 元の牛と同じモデルを引き継がせる */
			m_dummyCow->SetModelPath(COW_MODEL_PATHS[static_cast<int>(m_cowType)]);

			/** dummyCowに牛の情報を渡す*/
			m_dummyCow->SetPosition(m_transform.GetPosition());
			m_dummyCow->SetRotation(m_transform.GetRotation());
			
			/** ジャンプアニメーションを再生*/
			m_dummyCow->PlayJumpAnimtion();

			if (m_game)
			{
				m_game->SetDuumyCow(m_dummyCow);
			}

			/** 牛を削除 */
			m_isCaptured = false;
			RequestKill();

			return;
		}
	}
}

void Cow::AvoidPlayer()
{	
	/** UFOに捕まっているときは逃げない */
	if (m_isTakeAwayed == true) return;

	/** ロープに捕まっているときは逃げない */
	if (m_isCaptured)return;

	/** プレイヤーが動いていないときは逃げない */
	if (!m_player->GetIsMoving())
	{
		/** 通常のステートに戻す */
		m_cowState = 0;
		m_rotationState = EnRotateState_MoveDir;

		return;
	}
	/** プレイヤーと牛の位置を取得 */
	Vector3 playerPos = m_player->GetPosition();
	Vector3 cowPos = m_transform.GetPosition();

	Vector3 dir = cowPos - playerPos;

	/** プレイヤーから一定距離以内なら逃げる */
	float dist = dir.Length();

	if (dist < AVOID_DISTANCE)
	{
		/** 牛のステートを逃げるに変更 */
		m_cowState = 2;

		dir.Normalize();
		Vector3 move = Vector3::Zero;
		move += dir * m_avoidPower * 100.0f;
		Vector3 newPos = m_cowCharacterController.Execute(move, g_gameTime->GetFrameDeltaTime());

		m_transform.SetPosition(newPos);
		m_cowmodelRender.SetPosition(newPos);

		/** 逃げる方向に回転 */
		m_transform.GetRotation().SetRotationYFromDirectionXZ(dir);
		m_transform.SetRotation(m_transform.GetRotation());
	}

}

void Cow::ChasePlayer()
{
	/** UFOに捕まっている間は追いかけない */
	if (m_isTakeAwayed == true) return;

	/** ロープに捕まっている時は追いかけない */
	if (m_isCaptured) return;

	/** プレイヤーと牛の位置を取得 */
	Vector3 playerPos = m_player->GetPosition();
	Vector3 cowPos = m_transform.GetPosition();

	Vector3 dir = playerPos - cowPos;
	float dist = dir.Length();

	/** 近づきすぎると停止して待機アニメーションに戻す */
	if (dist < CHASE_STOP_DISTANCE)
	{
		m_cowState = 0;
		m_rotationState = EnRotateState_MoveDir;
		m_moveDir = Vector3::Zero;
		return;
	}

	/** 歩きステートにする。*/
	m_cowState = 1;

	dir.Normalize();

	m_moveDir = dir * 100.0f;

	Vector3 move = Vector3::Zero;
	move += dir * CHASE_POWER * 100.0f;
	Vector3 newPos = m_cowCharacterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	m_transform.SetPosition(newPos);
	m_cowmodelRender.SetPosition(newPos);

	/** 進行方向に(プレイヤー方向)回転 */
	m_transform.GetRotation().SetRotationYFromDirectionXZ(dir);
	m_transform.SetRotation(m_transform.GetRotation());



}

void Cow::SetCowType(EnCowType type)
{
	m_cowType = type;

	m_isUFOAttracted = (type == EnCowType::en_Light);
	m_isChasingPlayer = (type == EnCowType::en_Chase);

	if (type == EnCowType::en_Chase)
	{
		m_isTargetFood = false;
	}
  
	/** ボーナス牛専用のステータス */
	if (type == EnCowType::en_Bonus)
	{
		m_pullResistance = BONUS_COW_PULL_RESISTANCE;
		m_avoidDistance  = BONUS_COW_AVOID_DISTANCE;
		m_avoidPower = BONUS_COW_AVOID_POWER;
	}
	else
	{
		m_pullResistance = 1.0f;
		m_avoidDistance = AVOID_DISTANCE;
		m_avoidPower = AVOID_POWER;
	}
	ApplyCowModel();
}

void Cow::ApplyCowModel()
{
	/** 既に初期化済みなら何もしない */
	if (m_isModelInitialized)
	{
		return;
	}

	int index = static_cast<int>(m_cowType);
	const char* modelPath = COW_MODEL_PATHS[index];

	m_cowmodelRender.Init(modelPath, animationClips, EnAnimation_Num, enModelUpAxisZ);
	m_isModelInitialized = true;
}

bool Cow::CanUpdate()
{
	m_player = FindGO<Player>("player");
	m_countdown = FindGO<CountDown>("countdown");
	m_rope = FindGO<Rope>("rope");
	m_pause = FindGO<Pause>("pause");
	m_game = FindGO<Game>("game");
	m_timer = FindGO<Timer>("timer");
	/** どれかが存在しないときは処理しないようにするため早期リターン */
	if (m_pause == nullptr ||
		m_countdown == nullptr ||
		m_player == nullptr ||
		m_rope == nullptr ||
		m_game == nullptr ||
		m_timer == nullptr
		)
	{
		return false;
	}

	/** 死んでいるときは処理しないようにするため早期リターン */
	if (m_isDeadFlag)
	{
		return false;
	}

	/** ロープに捕まっているときは処理しないようにするため早期リターン */
	if (IsDead())
	{
		return false;
	}

	/** タイムアウトのときは処理しないようにするため早期リターン */
	if (m_game->GetIsTimeOut())
	{
		return false;
	}

	/** ポーズ中のときは処理しないようにするため早期リターン */
	if (m_pause->GetIsPause())
	{
		return false;
	}

	/** カウントダウン中のときは処理しないようにするため早期リターン */
	if (m_countdown->GetCountDown())
	{
		return false;
	}

	/** タイマーが一秒未満なら処理しないようにするため早期リターン */
	if (m_timer->GetTimer() < 1.0f)
	{
		return false;
	}

	return true;
}

void Cow::RequestKill()
{
	/** 牛の削除フラグを立てる */
	m_isPendingKill = true;
}

void Cow::TakeAwayedByUFO(Vector3 direction, float speed)
{
	if (direction.LengthSq() < 0.0001f)
	{
		return;
	}

	/** 方向ベクトルにする */
	direction.Normalize();

	/** Excuteに渡すのは「速度」*/
	Vector3 velocity = direction * speed * 45.0f;

	/** UFOに連れて行かれるときは牛を上に移動させる */
	Vector3 newPos = m_cowCharacterController.Execute(velocity, g_gameTime->GetFrameDeltaTime());
	m_transform.SetPosition(newPos);
	m_cowmodelRender.SetPosition(newPos);
}

void Cow::PlayAnimation()
{
	/** ロープに捕まっているときはアニメーションを変えない */
	if (m_isCaptured)
	{
		return;
	}

	switch (m_cowState)
	{
	case 0:
			m_cowmodelRender.PlayAnimation(EnAnimation_Idle);
			break;

	case 1:
	case 2:
			m_cowmodelRender.PlayAnimation(EnAnimation_Walk);
			break;

	default:
		break;
	}
}


void Cow::Render(RenderContext& rc)
{
	/** カウントダウン中は牛を描画しない*/
	if (m_countdown && m_countdown->GetCountDown())
	{
		return;
	}
	/** モデルが初期化されてない場合は描画しない */
	if (!m_isModelInitialized)
	{
		return;
	}

	/** 普通の牛のモデルを描画する */
	m_cowmodelRender.Draw(rc);
}
