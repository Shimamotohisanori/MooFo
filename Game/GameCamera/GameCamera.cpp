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

	m_cameraPos.Set(0.0f, 125.0f, -250.0f);
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

	Vector3 toCameraPosOld = m_cameraPos;

	//右スティック入力取得でカメラを回す
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	Quaternion rot;
	rot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
	rot.Apply(m_cameraPos);


	// --- 上下回転修正版 ---
	//「カメラの右方向」を現在の m_CameraPos から正しく計算し、
	//それを使ってX軸回転を行っている。
	Vector3 forward = m_cameraPos;
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
	rot.Apply(m_cameraPos);


	Vector3 dir = m_cameraPos;
	dir.Normalize();
	float limit = 0.95f; // cos角度による制限(= 約72°)
	if (fabsf(dir.Dot(Vector3::AxisY)) > limit)
	{
		// 上向きすぎ・下向きすぎを防止
		m_cameraPos = toCameraPosOld;
	}

	//視点の計算
	Vector3 pos = target + m_cameraPos;

	// ★ カメラ位置と注視点が一致しないようにする保険 ★
	if ((pos - target).LengthSq() < 0.0001f)
	{
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

	if (!m_rope) return;

	/** ロープが投げられたら一回だけ実行 */
	if (m_rope->GetIsThrowRope() && !m_isRopeCameraStarted)
	{
		m_isRopeCameraStarted = true;

		/** 現在のカメラ位置を保存 */
		m_savedCameraPos = m_cameraPos;

		/** プレイヤーのforwardをrotationから作る */
		Quaternion rot = m_player->GetRotation();
		Vector3 playerForward = Vector3::AxisZ;
		rot.Apply(playerForward);
		playerForward.Normalize();
		
		/** プレイヤーの少し前からカメラが出てくるようにする */
		m_cameraPos = playerForward * 80.0f + Vector3(0.0f, 40.0f, 0.0f);
	}

	/** ロープを投げている途中のカメラ処理 */
	if(m_rope->GetIsThrowRope())
	{
		/** カメラforwardを計算 */
		Vector3 camForward = g_camera3D->GetTarget() - g_camera3D->GetPosition();
		if (camForward.LengthSq() < 0.0001f)
		{
			/** forwardベクトルがゼロに近い場合
			 * 安全なデフォルト方向を設定 */
			camForward = Vector3::AxisZ;
		}

		else
		{
			/** forwardベクトルを正規化 */
			camForward.Normalize();
		}

		/** カメラを前進 */
		m_cameraPos += camForward * 3.0f;

		Vector3 eye = m_player->GetPosition() + Vector3(0.0f, 80.0f, 0.0f) + m_cameraPos;
		Vector3 target = m_player->GetPosition() + Vector3(0, 80, 0) + camForward * 500.0f;

		g_camera3D->SetTarget(target);
		g_camera3D->SetPosition(eye);
		g_camera3D->Update();
		return;

	}

	/** ロープを投げ終わったらカメラ位置を元に戻す */
	if (!m_rope->GetIsThrowRope() && m_isRopeCameraStarted)
	{
		m_isRopeCameraStarted = false;
		m_cameraPos = m_savedCameraPos;
	}
}


void GameCamera::HitCow()
{

	m_cow = FindGO<Cow>("cow");

	if (m_rope == nullptr || m_cow == nullptr)
	{
		return;
	}
	/** カメラのワールド座標を計算*/
	Vector3 cameraWorldPos = m_player->GetPosition() + Vector3(0.0f, 80.0f, 0.0f) + m_cameraPos;

	/** 牛とカメラの距離を計算する*/
	Vector3 diff;
	diff = cameraWorldPos - m_cow->GetPosition();

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
		Vector3 target = m_cow->GetPosition() + Vector3(0.0f, 80.0f, 0.0f);
		g_camera3D->SetTarget(target);
		// カメラ位置更新
		g_camera3D->SetPosition(m_cow->GetPosition() + Vector3(0.0f, 80.0f, -200.0f));
		g_camera3D->Update();
	}
}


