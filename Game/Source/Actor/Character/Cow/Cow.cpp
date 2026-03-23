#include "stdafx.h"
#include "Cow.h"
#include "Rope/Rope.h"
#include "Source/Actor/Character/Player/Player.h"
#include "CountDown/CountDown.h"
#include "GameCamera/GameCamera.h"
#include "GameScene/Game.h"
#include "CowNumberOfRescues/CowNumberOfRescues.h"
#include "Source/Actor/Character/UFO/UFO.h"
#include <time.h>
namespace
{
	const char* FILEPATH = "Assets/modelData/Cow/Model/Cow4.tkm"; //enModelUpAxis = enModelUpAxisZ;
	const char* THROW_ROPE_ANIMATION_FILE_PATH = "Assets/modelData/Cow/Animation/Idle2.tka";
	const char* FILEPATH_WALK = "Assets/modelData/Cow/Animation/Walk.tka";
}
Cow::Cow()
{
	//Idle
	animationClips[EnAnimation_Idle].Load(THROW_ROPE_ANIMATION_FILE_PATH);
	animationClips[EnAnimation_Idle].SetLoopFlag(true);
	//Walk
	animationClips[EnAnimation_Walk].Load(FILEPATH_WALK);
	animationClips[EnAnimation_Walk].SetLoopFlag(true);
}

Cow::~Cow()
{
	/** ロープに当たった牛が消えるときの処理 */
	if (m_rope && m_rope->GetHitCow() == this)
	{
		/** ロープの牛に当たったフラグをリセット */
		m_rope->SetIsHitCow(false);
		/** ロープの当たった牛をリセット */
		m_rope->SetHitCow(nullptr);
	}

	/** プレイヤーに引っ張られている牛が
	 * 消えるときの処理 */
	if (m_isCaptured)
	{
		GameCamera* camera = FindGO<GameCamera>("gameCamera");
		if (camera)
		{
			camera->SetIsCowCaptured(false);
		}
	}
}

bool Cow::Start()
{
	
	srand(time(nullptr));

	m_cowmodelRender.Init(FILEPATH,animationClips,EnAnimation_Num,enModelUpAxisZ);
	
	m_player = FindGO<Player>("player");
	m_countdown = FindGO<CountDown>("countdown");
	m_rope = FindGO<Rope>("rope");
	m_cowmodelRender.SetPosition(m_transform.GetPosition());
	m_cowmodelRender.Update();
	return true;
}

void Cow::Update()
{
	//カウントダウン中は牛を動かさないようにするため早期リターン
	if (m_countdown->GetCountDown())
	{
		return;
	}
	/*アニメーション*/
	PlayAnimation();

  if (m_rotationState == EnRotationState_MoveDir)
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

	/*モデルの更新*/
	m_cowmodelRender.Update();

	/*モデルの位置を反映*/
	m_cowmodelRender.SetPosition(m_transform.GetPosition());
	/*モデルに回転を反映*/
	m_cowmodelRender.SetRotation(m_transform.GetRotation());
}


void Cow::Move()
{

	//ロープに捕まっているときは移動しない
	if (m_isCaptured)
	{
		return;
	}

	//タイマーが0以上なら新しい方向を決める
	if (m_moveTimer <= 0)
	{
		if (m_isMove)
		{
			Vector3 dir
			(
				//(0,1,2,から-1を引いているので)-1,0,1の範囲でランダムな値を生成
				rand() % 3 - 1,//x
				0,             //yは常に0
				rand() % 3 - 1//z
			);

			// 0,0,0になったら一秒休む
			if (dir.LengthSq() == 0)
			{
				dir = Vector3(1, 0, 0);
			}
			dir.Normalize();
			m_moveDir = dir;
			m_moveTimer = 120.0f;//2秒ごとに方向を変える
			m_isMove = false;
		}
		//移動した後に必ず休む
			else
			{
				m_moveDir = Vector3::Zero;
				m_moveTimer = 120.0f;//2秒休む
				m_isMove = true;
			}
		}
	//移動
	Vector3 pos = m_transform.GetPosition();
	//少しづつ位置を動かしている
	pos += m_moveDir * m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	//ポジションを更新
	m_transform.SetPosition(pos);
	//モデルに位置を反映
	m_cowmodelRender.SetPosition(m_transform.GetPosition());
	//タイマーを減らす
	m_moveTimer--;
}
void Cow::Rotation()
{
	//移動ステートの時は移動方向に回転する。
	if (m_rotationState == EnRotationState_MoveDir)
	{
		if (fabsf(m_moveDir.x) >= 0.0001f || fabsf(m_moveDir.z) >= 0.0001f)
		{
			//移動方向に回転させる
			m_transform.GetRotation().SetRotationYFromDirectionXZ(m_moveDir);
			m_transform.SetRotation(m_transform.GetRotation());
		}
	}
	else if (m_rotationState == EnRotationState_Spin)
	{
		//回転ステートがスピンのときは常に回転する。
		m_transform.GetRotation().AddRotationDegY(3.0f);
		m_transform.SetRotation(m_transform.GetRotation());
	}
}

void Cow::ManageState()
{
	if (fabsf(m_moveDir.x) >= 0.0001f || fabsf(m_moveDir.z) >= 0.0001f)
	{
		m_cowState = 1;//歩き
	}
	else
	{
		m_cowState = 0;//待機
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
		//プレイヤーの位置を取得
		Vector3 playerPos = m_player->GetPosition();

		//牛の位置
		Vector3 cowPos = m_transform.GetPosition();

		//プレイヤーへの方向
		Vector3 dir = playerPos - cowPos;

		//正規化
		dir.Normalize();

		//牛をプレイヤーのいる位置まで徐々に移動
		cowPos += dir * 5.0f;

		m_transform.SetPosition(cowPos);

		m_cowmodelRender.SetPosition(m_transform.GetPosition());

		m_player->SetGetLeftButton1(false);
		m_player->SetGetRightButton1(false);
	}
}

void Cow::CapturedByPlayer()
{
	//ロープが牛に当たっているとき
	if (m_isCaptured)
	{
		//牛とプレイヤーの間の距離を計算
		Vector3 playerPos = m_player->GetPosition();
		Vector3 cowPos = m_transform.GetPosition();

		//プレイヤーへの方向
		Vector3 dir = playerPos - cowPos;

		//距離が一定以下なら捕獲される
		if (dir.Length() < 50.0f)
		{
			//捕獲されたときの処理
			m_rope->SetIsHitCow(false);
			m_rope->SetHitCow(nullptr);

			if (m_takingUFO)
			{
				m_takingUFO->SetIsCowTakeAwayed(false);
				m_takingUFO->ReMoveTargetCow();
			}

			//カメラの牛捕獲フラグを下ろす
			GameCamera* camera = FindGO<GameCamera>("gameCamera");
			if (camera)
			{
				camera->SetIsCowCaptured(false);
			}

			//コンボを増やす
			Game* game = FindGO<Game>("game");
			if (game)
			{
				game->AddCombo();
				game->ReMoveCow(this);
			}

			//牛の救出数を増やす
			CowNumberOfRescues* cowNumberOfRescues = FindGO<CowNumberOfRescues>("cownumberofrescues");
			if (cowNumberOfRescues)
			{
				cowNumberOfRescues->AddRescue();
			}

			//状態をリセットする
			m_isTakeAwayed = false;

			//牛を削除
			DeleteGO(this);
			return;
		}
	}
}

void Cow::PlayAnimation()
{
	switch (m_cowState)
	{
	case 0 :
			m_cowmodelRender.PlayAnimation(EnAnimation_Idle);
			break;

	case 1:
			m_cowmodelRender.PlayAnimation(EnAnimation_Walk);
			break;

	default:
		break;
	}
}


void Cow::Render(RenderContext& rc)
{
	m_cowmodelRender.Draw(rc);
}
