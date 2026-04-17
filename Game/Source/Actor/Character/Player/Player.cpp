#include "stdafx.h"
#include "Player.h"
#include "Rope/Rope.h"
#include "GameScene/Game.h"
#include "Transform/Transform.h"
#include "CountDown/CountDown.h"

namespace
{
	const char* FILEPATH = "Assets/modelData/CowBoy/CowBoy3.tkm";
	const char* ANIMATION_IDLEFILEPATH = "Assets/modelData/CowBoy/Idle3.tka";
	const char* ANIMATION_RUNFILEPATH = "Assets/modelData/CowBoy/Run3.tka";
	const float CHRACTER_CONTROLLER_WIDTH = 25.0f;
	const float CHRACTER_CONTROLLER_HIGHT = 75.0f;
	const float PLAYER_MOVE_LIMIT_RADIUS = 1450.0f;

}

Player::Player()
{
	//アニメーションクリップの再生
	animationClips[enAnimationClip_Idle].Load(ANIMATION_IDLEFILEPATH);
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Run].Load(ANIMATION_RUNFILEPATH);
	animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_playerModelRender.Init(FILEPATH, animationClips, enAnimationClip_Num, enModelUpAxisZ);


}

Player::~Player()
{
	DeleteGO(m_rope);
}

bool Player::Start()
{
	m_countDown = FindGO<CountDown>("countdown");
	m_game = FindGO<Game>("game");
	m_rope = NewGO<Rope>(0, "rope");

    m_characterController.Init(CHRACTER_CONTROLLER_WIDTH, CHRACTER_CONTROLLER_HIGHT, m_transform.GetPosition());
	m_playerModelRender.SetPosition(m_transform.GetPosition());
	m_playerModelRender.Update();
	return true;
}

void Player::Update()
{
	if (m_countDown->GetCountDown())
	{
		return;
	}
	/*移動*/
	Move();
	/*回転*/
	Rotation();
	/*ロープを投げる処理*/
	ThrowRope();
	/*ロープを引っ張る処理*/
	PullRope();
	/*ステート管理*/
	ManageState();

	/** プレイヤーが動いていたら */
	if (fabsf(m_moveSpeed.x) >= 0.0001f || fabsf(m_moveSpeed.z) >= 0.0001f)
	{
		m_isMoving = true;
	}

	/** プレイヤーが動いていなかったら */
	else
	{
		m_isMoving = false;
	}

	/*アニメーション*/
	PlayAnimation();
	m_playerModelRender.Update();
}

void Player::Move()
{

	if (m_rope->GetIsThrowRope() or m_rope->GetIsHitCow())
	{
		m_moveSpeed = Vector3::Zero;
		//ロープを投げているときとロープが牛に当たっているときは移動できないようにする
		return;
	}

	//左スティックの入力量を取得
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.z = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right   = g_camera3D->GetRight();

	//正規化
	right.y = 0.0f;
	forward.y = 0.0f;
	forward.Normalize();
	right.Normalize();

	//入力量を反映
	Vector3 moveDir = forward * stickL.z + right * stickL.x;
	moveDir *= 150.0f;
	m_moveSpeed = moveDir;

	//左スティックの入力量と120.0fを乗算
	right*= stickL.x * 150.0f;
	forward *= stickL.z * 150.0f;

	//移動速度にカメラの前方向と右方向を加算
	m_moveSpeed += right + forward;
	//キャラクターコントローラーを使って座標を移動させる
	m_transform.GetPosition() = m_characterController.Execute(m_moveSpeed,g_gameTime->GetFrameDeltaTime());

	Vector3 position = m_transform.GetPosition();

	/** XZ平面の距離計算 */
	Vector3 posXZ = position;
	posXZ.y = 0.0f;
	float distsance = posXZ.Length();

	/** 半径を超えたら円周上に戻す */
	if (distsance > PLAYER_MOVE_LIMIT_RADIUS)
	{
		/** XZ平面の正規化 */
		posXZ.Normalize();

		/** 半径を掛ける */
		posXZ *= PLAYER_MOVE_LIMIT_RADIUS;

		/** Y座標はそのまま */
		/** XZ平面の座標を更新 */
		position.x = posXZ.x;
		position.z = posXZ.z;

		m_transform.SetPosition(position);
	}


	//モデルの座標をキャラクターコントローラーの座標に合わせる
	m_playerModelRender.SetPosition(m_transform.GetPosition());

}

void Player::Rotation()
{
	if (m_rope->GetIsThrowRope())
	{
		//ロープを投げているときは回転できないようにする
		return;
	}
	if (fabsf(m_moveSpeed.x) >= 0.0001f || fabsf(m_moveSpeed.z) >= 0.0001f)
	{
		//キャラクターの方向を変える
		m_transform.GetRotation().SetRotationYFromDirectionXZ(m_moveSpeed);

		//モデルの回転をキャラクターの回転に合わせる
		m_playerModelRender.SetRotation(m_transform.GetRotation());
	}
}

void Player::ThrowRope()
{
	if (m_throwRopeCoolTime > 0.0f) {
		m_throwRopeCoolTime -= g_gameTime->GetFrameDeltaTime();
	}

	if (m_rope->GetIsHitCow() or !m_rope)
	{
		//ロープが存在しない、ロープが牛に当たっているときはロープを投げられないようにする
		return;
	}

	/** クールダウン中はロープを投げられない */
	if (m_throwRopeCoolTime > 0.0f) return;

	//RB2ボタンが押されていて、ロープを投げていないとき
	if (g_pad[0]->IsTrigger(enButtonRB2) && !m_rope->GetIsThrowRope())
	{
		//ロープを投げる
		m_rope->SetIsThrowRope(true);

		/** クールダウン開始 */
		m_throwRopeCoolTime = 1.5f;
	}
}

void Player::PullRope()
{

	if (m_rope)
	{
		//ロープが牛に当たっているとき
		if (m_rope->GetIsHitCow())
		{
			if (g_pad[0]->IsTrigger(enButtonRB1) && !m_isRightButton1)
			{
				m_isRightButton1 = true;
				m_isLeftButton1 = false;
			}

			if (g_pad[0]->IsTrigger(enButtonLB1) && !m_isLeftButton1)
			{
				m_isLeftButton1 = true;
				m_isRightButton1 = false;
			}

		}
	}
}


void Player::ManageState()
{
	if (fabsf(m_moveSpeed.x) >= 0.01f || fabsf(m_moveSpeed.z) >= 0.01f)
	{
		//移動しているときは走るアニメーションにする
		m_playerState = 1;
	}
	else
	{
		//移動していないときは待機アニメーションにする
		m_playerState = 0;
	}

}


void Player::PlayAnimation()
{
	//前の状態と同じ状態だったらアニメーションを切り替えない
	if (m_playerState == m_prevPlayerState)
	{
		return;
	}
	m_prevPlayerState = m_playerState;
	switch (m_playerState)
	{
	case 0:
		//待機アニメーション
		m_playerModelRender.PlayAnimation(enAnimationClip_Idle);
		break;
	case 1:
		//走るアニメーション
		m_playerModelRender.PlayAnimation(enAnimationClip_Run);
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	m_playerModelRender.Draw(rc);
}