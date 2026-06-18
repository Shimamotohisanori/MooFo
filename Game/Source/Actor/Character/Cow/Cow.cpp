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
#include "CowLuring.h"

namespace
{
	/** 牛のモデルファイルパス */
	const char* COW_MOCEL_FILEPATH = "Assets/modelData/Cow/Model/Cow5.tkm"; //enModelUpAxis = enModelUpAxisZ;

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

	/** 牛の移動時間と休憩時間 */
	constexpr int RANDOMCOW_TIMER = 180 + 60;

	/** 納屋の位置 */
	const Vector3 BARN_POSITION = Vector3{ -1320.0f,0.0f,10.0f };

	/** 納屋の半径 */
	constexpr float BARN_RADIUS = 300.0f;
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

}

bool Cow::Start()
{
	/** サウンドマネージャーを取得する */
	m_CowSound  = FindGO<SoundManager>("soundmanager");

	/** レイトレーシングを無効にする */
	m_cowmodelRender.SetRaytracingWorld(false);

	/** キャラクターコントローラーを初期化する */
	m_cowCharacterController.Init(20.0f, 20.0f, m_transform.GetPosition());

	/** 牛のモデルを初期化する */
	m_cowmodelRender.Init(COW_MOCEL_FILEPATH,animationClips,EnAnimation_Num,enModelUpAxisZ);
	m_cowmodelRender.SetPosition(m_transform.GetPosition());
	m_cowmodelRender.Update();
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
		if (m_takingUFO)
		{
			m_takingUFO->SetIsCowTakeAwayed(false);
			m_takingUFO->ReMoveTargetCow();
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
	
	/** プレイヤーから逃げる関数 */
	AvoidPlayer();

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
	m_transform.SetPosition(pos);

	/** モデルの位置を反映 */
	m_cowmodelRender.SetPosition(pos);
	
	/** モデルに回転を反映 */
	m_cowmodelRender.SetRotation(m_transform.GetRotation());

	/** モデルの更新 */
	m_cowmodelRender.Update();
}


void Cow::Move()
{

	/** ロープに捕まっているときは移動しない */
	if (m_isCaptured)
	{
		return;
	}

	/** タイマーが0以上なら新しい方向を決める */
	if (m_moveTimer <= 0)
	{
		if (m_isMove)
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

		if (m_CowLuring != nullptr)
		{
			/** 牛の餌マネージャーから餌を削除する */
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
			//m_game->ReMoveCow(this);
		}

		/** 牛の救出数を増やす */
		CowNumberOfRescues* cowNumberOfRescues = FindGO<CowNumberOfRescues>("cownumberofrescues");
		if (cowNumberOfRescues)
		{
			cowNumberOfRescues->AddRescue();
		}

		m_dummyCow = NewGO<DummyCow>(0, "dummyCow");
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
		move += dir * PULL_POWER * 100.0f;
		Vector3 newPos = m_cowCharacterController.Execute(move, g_gameTime->GetFrameDeltaTime());

		m_transform.SetPosition(newPos);

		m_cowmodelRender.SetPosition(newPos);
		m_player->SetGetLeftButton1(false);
		m_player->SetGetRightButton1(false);

		/** 牛の鳴き声効果音を流す*/
		if (m_CowSE == false)
		{
			m_CowCrySE = m_CowSound->PlayingSE(SoundSE::enCowCrySE, false);
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
		if (dir.Length() < 50.0f)
		{
			m_isDeadFlag = true;

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
				//m_game->ReMoveCow(this);
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
		move += dir * AVOID_POWER * 100.0f;
		Vector3 newPos = m_cowCharacterController.Execute(move, g_gameTime->GetFrameDeltaTime());

		m_transform.SetPosition(newPos);
		m_cowmodelRender.SetPosition(newPos);

		/** 逃げる方向に回転 */
		m_transform.GetRotation().SetRotationYFromDirectionXZ(dir);
		m_transform.SetRotation(m_transform.GetRotation());
	}

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
	Vector3 velocity = direction * speed * 40.0f;

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
	/** 普通の牛のモデルを描画する */
	m_cowmodelRender.Draw(rc);
}
