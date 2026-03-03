#include "stdafx.h"
#include "GameCamera.h"

#include"Source/Actor/Character/Player/Player.h"

GameCamera::GameCamera()
{
}
	
GameCamera::~GameCamera()
{
}


bool GameCamera::Start()
{
	m_player = FindGO<Player>("player");

	m_CameraPos.Set(0.0f, 125.0f, -250.0f);
	//近平面を設定
	g_camera3D->SetNear(1.0f);
	//円平面を設定
	g_camera3D->SetFar(100000.0f);
	return true;
}
			
void GameCamera::Update()
{
	Follow();
}


void GameCamera::Follow()
{
	Vector3 target;
	//注視点をプレイヤーの座標に設定
	target = m_player->GetPosition();
	//プレイヤーの足元より少し上に注視点を設定
	target.y += 80.0f;

	Vector3 toCameraPosOld = m_CameraPos;

	//右スティック入力取得でカメラを回す
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	Quaternion rot;
	rot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
	rot.Apply(m_CameraPos);


	// --- 上下回転修正版 ---
	//「カメラの右方向」を現在の m_CameraPos から正しく計算し、
	//それを使ってX軸回転を行っている。
	Vector3 forward = m_CameraPos;
	if (forward.LengthSq() > 0.0001f)
		forward.Normalize();
	else
		forward = Vector3(0, 0, 1);   // 安全なデフォルト方向
	forward.Normalize();

	// ワールド上方向
	Vector3 up = Vector3::AxisY;

	// カメラの右方向を算出
	Vector3 right;
	right.Cross(up, forward);
	right.Normalize();

	// 上下回転
	rot.SetRotationDeg(right, 1.3f * y);
	rot.Apply(m_CameraPos);


	Vector3 dir = m_CameraPos;
	dir.Normalize();
	float limit = 0.95f; // cos角度による制限(= 約72°)
	if (fabsf(dir.Dot(Vector3::AxisY)) > limit)
	{
		// 上向きすぎ・下向きすぎを防止
		m_CameraPos = toCameraPosOld;
	}

//視点の計算
Vector3 pos = target + m_CameraPos;

// ★ カメラ位置と注視点が一致しないようにする保険 ★
if ((pos - target).LengthSq() < 0.0001f) {
	pos = target + Vector3(0.0f, 0.0f, -50.0f); // 適当な距離を確保
}
//メインカメラに注視点と視点を設定
g_camera3D->SetTarget(target);
g_camera3D->SetPosition(pos);
//カメラの更新
g_camera3D->Update();
}



