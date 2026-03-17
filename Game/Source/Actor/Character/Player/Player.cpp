#include "stdafx.h"
#include "Player.h"
#include "Rope/Rope.h"
#include"Transform/Transform.h"

namespace
{
	const char* FILEPATH = "Assets/modelData/CowBoy.tkm", enModelUpAxis = enModelUpAxisZ;

	uint8_t CHRACTER_CONTROLLER_WIDTH = 25.0f;
	uint8_t CHRACTER_CONTROLLER_HIGHT = 75.0f;

}
Player::Player()
{

}

Player::~Player()
{
	DeleteGO(m_rope);
}

bool Player::Start()
{
	m_playerModelRender.Init(FILEPATH);

	m_playerModelRender.SetPosition(m_transform.GetPosition());
	m_characterController.Init(CHRACTER_CONTROLLER_WIDTH, CHRACTER_CONTROLLER_HIGHT, m_transform.GetPosition());

	m_rope = NewGO<Rope>(0, "rope");

	return true;
}

void Player::Update()
{
	Move();

	Rotation();

	ThrowRope();

	PullRope();

	m_playerModelRender.Update();
}

void Player::Move()
{
	if (m_rope->GetIsThrowRope() or m_rope->GetIsHitCow())
	{
		//ロープを投げているときとロープが牛に当たっているときは移動できないようにする
		return;
	}
	//xzの移動速度を初期化
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

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
	Vector3 moveDir = forward * stickL.z* 150.0f + right * stickL.x * 150.0f;
	m_moveSpeed.x = moveDir.x;
	m_moveSpeed.z = moveDir.z;

	//左スティックの入力量と120.0fを乗算
	right*= stickL.x * 150.0f;
	forward *= stickL.z * 150.0f;

	//移動速度にカメラの前方向と右方向を加算
	m_moveSpeed += right + forward;
	//キャラクターコントローラーを使って座標を移動させる
	m_transform.GetPosition() = m_characterController.Execute(m_moveSpeed,g_gameTime->GetFrameDeltaTime());

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

	if (m_rope->GetIsHitCow() && !m_rope)
	{
		//ロープが存在しない、ロープが牛に当たっているときはロープを投げられないようにする

		return;
	}

	//RB2ボタンが押されていて、ロープを投げていないとき
	if (g_pad[0]->IsTrigger(enButtonRB2) && !m_rope->GetIsThrowRope())
	{
		//ロープを投げる
		m_rope->SetIsThrowRope(true);
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

void Player::Render(RenderContext& rc)
{
	m_playerModelRender.Draw(rc);
}