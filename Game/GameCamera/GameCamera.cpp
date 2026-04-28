#include "stdafx.h"
#include "GameCamera.h"
#include "Rope/Rope.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "Source/Actor/Character/Player/Player.h"
#include "SoundManager/SoundManager.h"

namespace
{
	/** カメラ基本設定 */
	constexpr float CAMERA_DEFAULT_Y = 125.0f;
	constexpr float CAMERA_DEFAULT_Z = -250.0f;

	/** プレイヤー注視点 */
	constexpr float PLAYER_LOOK_OFFSET_Y = 80.0f;

	/** ロープ視点 */
	constexpr float ROPE_START_FORWARD_DIST = 80.0f;
	constexpr float ROPE_START_UP_OFFSET = 40.0f;
	constexpr float ROPE_FLY_SPEED = 5.0f;
	constexpr float ROPE_TARGET_DISTANCE = 500.0f;

	/** 牛判定 */
	constexpr float COW_HIT_DISTANCE = 100.0f;

	/** 牛視点 */
	constexpr float COW_CAMERA_UP = 80.0f;
	constexpr float COW_CAMERA_BACK = -200.0f;

	/** カメラの最低高さ */
	constexpr float MIN_CAMERA_HEIGHT = 5.0f;

}

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
	DeleteGO(m_cowCatchSE);
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
	g_camera3D->SetFar(1000000.0f);
	return true;
}

void GameCamera::Update()
{
	Follow();
	FollowRope();
	CheckCameraHitCow();
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

	if (dir.LengthSq() > 0.00001f)
	{
		dir.Normalize();
	}
	else
	{
		dir = Vector3(0, 0, -1);
	}
	float limit = 0.95f; // cos角度による制限(= 約72°)
	if (fabsf(dir.Dot(Vector3::AxisY)) > limit)
	{
		// 上向きすぎ・下向きすぎを防止
		m_cameraPos = toCameraPosOld;
	}

	//視点の計算
	Vector3 pos = target + m_cameraPos;


	// カメラ位置と注視点が一致しないようにする保険
	if ((pos - target).LengthSq() < 0.0001f)
	{
		pos = target + Vector3(0.0f, 0.0f, -50.0f); // 適当な距離を確保
	}

	// 地面付近にカメラがあった場合はそれ以上下に行かないようにする
	if (pos.y < MIN_CAMERA_HEIGHT) {
		pos.y = MIN_CAMERA_HEIGHT;
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

		/** カメラの forward を保存（上下方向も含む）*/
		Vector3 camForward = g_camera3D->GetTarget() - g_camera3D->GetPosition();
		if (camForward.LengthSq() < 0.0001f) {
			camForward = Vector3::AxisZ;
		}
		camForward.Normalize();
		m_ropeForward = camForward;

		/** プレイヤーの少し前からカメラが出てくるようにする */
		m_cameraPos = camForward 
			* ROPE_START_FORWARD_DIST
			+ Vector3(0.0f, ROPE_START_UP_OFFSET, 0.0f);
	}

	/** ロープを投げている途中のカメラ処理 */
	if(m_rope->GetIsThrowRope())
	{
		/** カメラforwardを計算 */
		Vector3 camForward = m_ropeForward;

		/** カメラを前進する */
		m_cameraPos += camForward * ROPE_FLY_SPEED;

		/** 地面に近すぎたら下に行かないようにする */
		if (m_cameraPos.y < MIN_CAMERA_HEIGHT) {
			m_cameraPos.y = MIN_CAMERA_HEIGHT;
		}

		Vector3 eye = m_player->GetPosition()
			+ Vector3(0.0f, 0.0f, 0.0f)
			+ m_cameraPos;

		Vector3 target = eye + camForward * ROPE_TARGET_DISTANCE;

		/** カメラの注視点と位置を設定 */
		g_camera3D->SetPosition(eye);
		g_camera3D->SetTarget(target);
		
		/** カメラの更新 */
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

void GameCamera::CheckCameraHitCow()
{
	/** ロープが投げられていない場合は牛に当たる処理を行わない */
	if (!m_rope->GetIsThrowRope()) return;

	/** 牛を捕まえた後は牛に当たる処理を行わない */
	if (m_isCowCaptured) return;

	/** カメラ位置 */
	Vector3 camPos = g_camera3D->GetPosition();

	/** 全ての牛を取得する */
	auto cows = FindGOs<Cow>("cow");

	for (auto cow : cows)
	{
		if (!cow) continue;

		float dist = (cow->GetPosition() - camPos).Length();

		/** 牛とカメラの距離が一定以下で牛がUFOに連れて行かれた状態ならロープが当たった扱いにする */
		if (dist < COW_HIT_DISTANCE && cow->GetIsTakeAwayed())
		{
			/** ロープが当たった扱いにする */
			m_rope->OnHitCow(cow);

			/** 牛を捕まえたフラグを立てる */
			m_isCowCaptured = true;

			/** 牛を捕まえた音を再生 */
			SoundManager* soundManager = FindGO<SoundManager>("soundmanager");
			if (soundManager)
			{
				m_cowCatchSE = soundManager->PlayingSE(SoundSE::enCowCatchSE, false);
			}

			return;
		}
	}
}


void GameCamera::HitCow()
{

	/** ロープが牛に当たっていなければ何もしない */
	if (!m_rope->GetIsHitCow())
		return;

	/** 当たった牛を取得 */
	Cow* hitCow = m_rope->GetHitCow();

	/** 牛が存在しない場合は
	 * 牛を捕まえたフラグを下ろして
	 * 処理を終える */
	if (!hitCow)
	{
		m_isCowCaptured = false;
		return;
	}

	/** ターゲットは牛 */
	Vector3 target = hitCow->GetPosition() + Vector3(0.0f, 80.0f, 0.0f);
	g_camera3D->SetTarget(target);

	/** カメラ位置更新 */
	Vector3 pos =
		hitCow->GetPosition() +
		Vector3(0.0f, COW_CAMERA_UP, COW_CAMERA_BACK);

	/** カメラ位置と注視点が一致しないようにする保険 */
	if ((pos - target).LengthSq() < 0.0001f)
	{
		pos.z -= 50.0f;
	}

	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);
	g_camera3D->Update();

}


