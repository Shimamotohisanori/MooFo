#include "stdafx.h"
#include "Player.h"
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

}

bool Player::Start()
{
	m_playerModelRender.Init(FILEPATH);
	m_playerModelRender.SetPosition(m_transform.GetPosition());
	m_characterController.Init(CHRACTER_CONTROLLER_WIDTH, CHRACTER_CONTROLLER_HIGHT, m_transform.GetPosition());
	return true;
}

void Player::Update()
{
	Move();

	m_playerModelRender.Update();
}

void Player::Move()
{
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

	//y方向には移動させない
	forward.y = 0.0f;
	right.y = 0.0f;
	//左スティックの入力量と150.0fを乗算
	right *= stickL.x * 150.0f;
	forward *= stickL.z * 150.0f;

	//移動速度にカメラの前方向と右方向を加算
	m_moveSpeed += right + forward;

	//移動速度にスティックの入力量を掛ける
	m_moveSpeed.x += stickL.x * 150.0f;
	m_moveSpeed.z += stickL.z * 150.0f;

	//キャラクターコントローラーを使って座標を移動させる
	m_transform.GetPosition() = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//モデルの座標をキャラクターコントローラーの座標に合わせる
	m_playerModelRender.SetPosition(m_transform.GetPosition());

}

void Player::Render(RenderContext& rc)
{
	m_playerModelRender.Draw(rc);
}