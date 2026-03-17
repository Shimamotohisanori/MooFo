#include "stdafx.h"
#include "GameCamera.h"
#include "Rope/Rope.h"
#include "Source/Actor/Character/Cow/Cow.h"
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

	m_rope = FindGO<Rope>("rope");

	m_cow = FindGO<Cow>("cow");

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
	FollowRope();
	HitCow();
}


void GameCamera::Follow()
{

	if (m_rope->GetIsThrowRope())
	{
		return;
	}

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


void GameCamera::FollowRope()
{

	if (m_rope == nullptr)
	{
		return;
	}


	/** ロープが投げられたら*/
	if (m_rope->GetIsThrowRope())
	{
		// --- カメラの forward を取得
		Vector3 camForward = g_camera3D->GetForward();  // カメラの向き（3D方向）

		if (camForward.LengthSq() < 0.0001f)
		{
			camForward = Vector3(0, 0, 1); // 安全なデフォルト方向
		}
		
		else
		{
			camForward.Normalize();
		}
		// カメラを forward 方向へ前進
		m_CameraPos += camForward * 7.0f;

		Vector3 eye = m_player->GetPosition() + Vector3(0.0f, 80.0f, 0.0f) + m_CameraPos;
		
		// ターゲットはプレイヤー
		Vector3 target = m_player->GetPosition() + Vector3(0.0f, 80.0f, 0.0f) + camForward * 500.0f;
		g_camera3D->SetTarget(target);

		if ((eye - target).LengthSq() < 0.0001f)
		{
			target += Vector3(0.0f, 0.0f, -50.0f); // 適当な距離を確保
		}

		// カメラ位置更新
		g_camera3D->SetPosition(m_player->GetPosition() + Vector3(0.0f, 80.0f, 0.0f) + m_CameraPos);

		camForward.Normalize();

		// カメラを forward 方向へ前進
		m_CameraPos += camForward * 7.0f;

		// ターゲットはプレイヤー
		Vector3 target = m_player->GetPosition() + Vector3(0, 80, 0) + camForward * 500.0f;
		g_camera3D->SetTarget(target);

		// カメラ位置更新
		g_camera3D->SetPosition(m_player->GetPosition() + Vector3(0, 80, 0) + m_CameraPos);

		g_camera3D->Update();
	}
}


void GameCamera::HitCow()
{

	m_cow = FindGO<Cow>("cow");

	if (m_rope == nullptr or m_cow == nullptr)
	{
		return;
	}


	/** カメラのワールド座標を計算*/
	Vector3 cameraWorldPos = m_player->GetPosition() + Vector3(0, 80, 0) + m_CameraPos;

	/** 牛とカメラの距離を計算する*/
	Vector3 diff = cameraWorldPos - m_cow->GetPosition();
	
	/** 距離が近ければ*/
	if (diff.LengthSq() < 100.0f * 100.0f)
	{
		// カメラ(縄の視点)が牛に近づいたら、牛に当たったと判断する
		m_rope->SetIsHitCow(true);
	}

	/** 牛に当たったら*/
	if (m_rope->GetIsHitCow())
	{
		// ターゲットは牛
		Vector3 target = m_cow->GetPosition() + Vector3(0, 80, 0);
		g_camera3D->SetTarget(target);
		// カメラ位置更新
		g_camera3D->SetPosition(m_cow->GetPosition() + Vector3(0, 80, -200));
		g_camera3D->Update();
	}
}



