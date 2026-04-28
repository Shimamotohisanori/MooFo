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
#include <time.h>

namespace
{
	const char* GAMECLEAR_FILEPATH = "Assets/modelData/Cow/Model/Cow4.tkm"; //enModelUpAxis = enModelUpAxisZ;
	const char* THROW_ROPE_ANIMATION_FILE_PATH = "Assets/modelData/Cow/Animation/Idle2.tka";
	const char* FILEPATH_WALK = "Assets/modelData/Cow/Animation/Walk.tka";

	/** プレイヤーに引っ張られるときの力 */
	constexpr float PULL_POWER = 8.0f;

	constexpr float COW_MOVE_LIMIT_RADIUS = 1450.0f;

	/** この距離以内なら逃げる */
	constexpr float AVOID_DISTANCE = 80.0f;

	/** 逃げる強さ */
	constexpr float AVOID_POWER = 5.0f;
}

Cow::Cow()
{
	/** Idle */
	animationClips[EnAnimation_Idle].Load(THROW_ROPE_ANIMATION_FILE_PATH);
	animationClips[EnAnimation_Idle].SetLoopFlag(true);
	
	/** Walk */
	animationClips[EnAnimation_Walk].Load(FILEPATH_WALK);
	animationClips[EnAnimation_Walk].SetLoopFlag(true);
}

Cow::~Cow()
{
}

bool Cow::Start()
{
	
	srand(time(nullptr));

	m_cowmodelRender.Init(GAMECLEAR_FILEPATH,animationClips,EnAnimation_Num,enModelUpAxisZ);
	m_cowmodelRender.SetPosition(m_transform.GetPosition());
	m_cowmodelRender.Update();

	
	return true;
}

void Cow::Update()
{
	m_player = FindGO<Player>("player");
	m_countdown = FindGO<CountDown>("countdown");
	m_rope = FindGO<Rope>("rope");
	m_pause = FindGO<Pause>("pause");

	if (m_pause == nullptr || m_countdown == nullptr || m_player == nullptr || m_rope == nullptr)
	{
		return;
	}

	/* ポーズ中は牛を動かさないようにするため早期リターン */
	if (m_pause->GetIsPause())
	{
		return;
	}

	/** カウントダウン中は牛を動かさないようにするため早期リターン */
	if (m_countdown->GetCountDown())
	{
		return;
	}
	/*アニメーション*/
	PlayAnimation();
	
	/** プレイヤーから逃げる関数 */
	AvoidPlayer();

  if (m_rotationState == EnRotateState_MoveDir)
  {
		/*移動*/
		Move();
  }
	
	/*ステート*/
	ManageState();
	/*回転*/
	Rotation();
	
	/** 牛がプレイヤーに引っ張られる処理*/
	PulledByPlayer();

	/** 牛がプレイヤーに捕獲される処理*/
	CapturedByPlayer();

	/*モデルの位置を反映*/
	m_cowmodelRender.SetPosition(m_transform.GetPosition());
	/*モデルに回転を反映*/
	m_cowmodelRender.SetRotation(m_transform.GetRotation());

	/*モデルの更新*/
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
			m_moveDir = dir;
			/**2秒ごとに方向を変える */
			m_moveTimer = 120.0f;
			m_isMove = false;
		}

			/** 移動した後に必ず休む */
			else
			{
				m_moveDir = Vector3::Zero;
				/** 2秒休む */
				m_moveTimer = 120.0f;
				m_isMove = true;
			}
		}
	/** 移動 */
	Vector3 pos = m_transform.GetPosition();
	/** 少しづつ位置を動かしている */
	pos += m_moveDir * m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	/** ポジションを更新 */
	m_transform.SetPosition(pos);
	/** モデルに位置を反映 */
	m_cowmodelRender.SetPosition(m_transform.GetPosition());
	/** タイマーを減らす */
	m_moveTimer--;

	/** 移動できる範囲を制限する */
	/** 牛の移動範囲の中心 */
	Vector3 center = Vector3::Zero;

	/** 牛の位置から中心を引いてXZ平面上のベクトルを作る */
	Vector3 posXZ = pos - center;

	/** Y成分を0にしてXZ平面上のベクトルにする */
	posXZ.y = 0.0f;

	/** XZ平面上のベクトルの長さを求める */
	float distance = posXZ.Length();

	/** 牛の移動範囲の半径より遠くにいるときは
	 * 牛の位置を移動範囲の端にする */
	if (distance > COW_MOVE_LIMIT_RADIUS)
	{
		posXZ.Normalize();
		pos = center + posXZ * COW_MOVE_LIMIT_RADIUS;

		m_transform.SetPosition(pos);
		m_cowmodelRender.SetPosition(pos);
	}
}
void Cow::Rotation()
{
	/** 移動ステートの時は移動方向に回転する。 */
	if (m_rotationState == EnRotateState_MoveDir)
	{
		if (fabsf(m_moveDir.x) >= 0.0001f || fabsf(m_moveDir.z) >= 0.0001f)
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
	/** 捕まってない牛は絶対に引っ張られない */
	if (!m_isCaptured)
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
		cowPos += dir * PULL_POWER;

		m_transform.SetPosition(cowPos);

		m_cowmodelRender.SetPosition(m_transform.GetPosition());

		m_player->SetGetLeftButton1(false);
		m_player->SetGetRightButton1(false);
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
			/** 自分がロープに紐づいてる時だけフラグをおろす */
			if (m_rope && m_rope->GetHitCow() == this)
			{
				m_rope->SetIsHitCow(false);
				m_rope->SetHitCow(nullptr);
			}

			if (m_takingUFO)
			{
				m_takingUFO->SetIsCowTakeAwayed(false);
				m_takingUFO->ReMoveTargetCow();
			}

			/** カメラの牛捕獲フラグを下ろす */
			GameCamera* camera = FindGO<GameCamera>("gameCamera");
			if (camera)
			{
				camera->SetIsCowCaptured(false);
			}

			/** コンボを増やす */
			Combo* combo = FindGO<Combo>("combo");
			Game* game = FindGO<Game>("game");
			if (game && combo)
			{
				combo->AddCombo();
				combo->AddScore(100);
				game->ReMoveCow(this);
			}

			/** 牛の救出数を増やす */
			CowNumberOfRescues* cowNumberOfRescues = FindGO<CowNumberOfRescues>("cownumberofrescues");
			if (cowNumberOfRescues)
			{
				cowNumberOfRescues->AddRescue();
			}

			/** 状態をリセットする */
			m_isTakeAwayed = false;

			/** 牛を削除 */
			DeleteGO(this);
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
		cowPos += dir * AVOID_POWER;
		m_transform.SetPosition(cowPos);
		m_cowmodelRender.SetPosition(m_transform.GetPosition());

		/** 逃げる方向に回転 */
		m_transform.GetRotation().SetRotationYFromDirectionXZ(dir);
		m_transform.SetRotation(m_transform.GetRotation());
	}

	else
	{
		/** プレイヤーから一定距離以上なら通常のステートに戻す */
		m_cowState = 0;
		m_rotationState = EnRotateState_MoveDir;
	}


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
			m_cowmodelRender.PlayAnimation(EnAnimation_Walk);
			break;

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
