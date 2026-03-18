#include "stdafx.h"
#include "Cow.h"
#include "Rope/Rope.h"
#include "Source/Actor/Character/Player/Player.h"
#include"CountDown/CountDown.h"
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

}

bool Cow::Start()
{
	
	srand(time(nullptr));

	m_CowmodelRender.Init(FILEPATH,animationClips,EnAnimation_Num,enModelUpAxisZ);
	
	m_player = FindGO<Player>("player");
	m_countdown = FindGO<CountDown>("countdown");
	m_rope = FindGO<Rope>("rope");
	return true;
}

void Cow::Update()
{
	//カウントダウン中は動かないようにするため早期リターン
	if (m_countdown->GetControlEnabled())
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
	m_CowmodelRender.Update();

	/*モデルの位置を反映*/
	m_CowmodelRender.SetPosition(m_transform.GetPosition());
	/*モデルに回転を反映*/
	m_CowmodelRender.SetRotation(m_transform.GetRotation());
}


void Cow::Move()
{

	//ロープが牛に当たっているときは移動しない
	if (m_rope->GetIsHitCow())
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
	m_CowmodelRender.SetPosition(m_transform.GetPosition());
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
	else if (m_rotationState == EnRotatitonState_Spin)
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

		m_CowmodelRender.SetPosition(m_transform.GetPosition());

		m_player->SetGetLeftButton1(false);
		m_player->SetGetRightButton1(false);
	}
}

void Cow::CapturedByPlayer()
{
	//ロープが牛に当たっているとき
	if (m_rope->GetIsHitCow())
	{
		//牛とプレイヤーの間の距離を計算
		Vector3 playerPos = m_player->GetPosition();
		Vector3 cowPos = m_transform.GetPosition();

		//プレイヤーへの方向
		Vector3 dir = playerPos - cowPos;

		//距離が一定以下なら捕獲される
		if (dir.Length() < 10.0f)
		{
			//捕獲されたときの処理
			m_rope->SetIsHitCow(false);

			//牛を削除
			DeleteGO(this);
		}
	}
}

void Cow::PlayAnimation()
{
	switch (m_cowState)
	{
	case 0 :
			m_CowmodelRender.PlayAnimation(EnAnimation_Idle);
			break;

	case 1:
			m_CowmodelRender.PlayAnimation(EnAnimation_Walk);
			break;

	default:
		break;
	}
}


void Cow::Render(RenderContext& rc)
{
	m_CowmodelRender.Draw(rc);
}
