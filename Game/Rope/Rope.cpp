#include "stdafx.h"
#include "Rope.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "GameCamera/GameCamera.h"
#include "Source/Actor/Character/Player/Player.h"
#include "GameTimer/Timer.h"

namespace
{
	/** ロープのモデルファイルパス */
	const char* ROPE_MODEL_FILEPATH = "Assets/modelData/Rope/NewRope.tkm";

	/** 捕まった牛用のロープモデルファイルパス */
	const char* CAPTURED_COW_FILEPATH = "Assets/modelData/Rope/CapturedCowRope.tkm";

	/** ロープの初期の大きさ */
	const Vector3 ROPE_INITIAL_SCALE = { 1.0f, 1.0f, 5.0f };

	/** 牛に当たっていない時の巻かれたロープの大きさ */
	const Vector3 NO_HIT_COW_ROLL_ROPE_SCALE = { 0.5f, 0.5f, 0.5f };

	/** 牛に当たった時の巻かれたロープの大きさ */
	const Vector3 HIT_COW_ROLL_ROPE_SCALE = { 1.0f, 1.0f, 1.0f };

	/** プレイヤーの右手からロープが出る位置のオフセット */
	const float ROPE_OFFSET_RIGHT = 10.0f;

	/** プレイヤーの前からロープが出る位置のオフセット */
	const float ROPE_OFFSET_FORWARD = 0.5f;

	/** プレイヤーの上からロープが出る位置のオフセット */
	const float ROPE_OFFSET_UP = 30.0f;

	/** 牛の位置の少し上を狙うためのオフセット */
	const float ROPE_AIM_UP_OFFSET = 30.0f;

	/** ロープの伸び縮みのスケールを求めるための距離にかける係数 */
	const float ROPE_SCALE_FACTOR = 0.05f;

	/** ロープの伸び縮みのスケールの最小値 */
	const float ROPE_MIN_SCALE_Z = 0.002f;

	/** ロープが牛に当たったとみなす距離 */
	const float ROPE_HIT_DISTANCE = 50.0f;

}


Rope::Rope()
{
	

}


Rope::~Rope()
{

}


bool Rope::Start()
{
	m_ropeModelRender.Init(ROPE_MODEL_FILEPATH);
	m_rollModelRender.Init(CAPTURED_COW_FILEPATH);
	m_player = FindGO<Player>("player");
	m_ropeScale = ROPE_INITIAL_SCALE;
	m_ropeModelRender.SetScale(m_ropeScale);
	m_ropeRot = Quaternion::Identity;
	m_rollModelRender.SetScale(NO_HIT_COW_ROLL_ROPE_SCALE);
	return true;
}


void Rope::Update()
{
	m_timer = FindGO<Timer>("timer");
	
	/** タイマーが存在しないなら処理しない */
	if (m_timer == nullptr) return;

	/** タイマーが1秒未満なら処理しない */
	if (m_timer->GetTimer() < 1.0f)
	{
		if (m_hitCow != nullptr && m_hitCow->IsDead())
		{
			m_isHitCow = false;
			m_hitCow = nullptr;
		}
		return;
	}

	/** プレイヤーが存在しないなら処理しない */
	if (!m_player) return;

	/** Dead になった牛のポインタをリセット */
	if (m_hitCow != nullptr && m_hitCow->IsDead())
	{
		m_isHitCow = false;
		m_hitCow = nullptr;
	}

	/** プレイヤーがロープを投げる処理 */
	PlayerThrowsRope();

	/** ロープアニメーション(ロープを投げた時のフラグ)がfalseなら */
	if (!m_isThrowRope)
	{
		/** プレイヤーの右手の位置のロープが常にある関数 */
		FollowRightHand();
	}

	/** ロープの回転に関する関数 */
	RotateRope();

	if (m_isHitCow && m_hitCow != nullptr)
	{
		/** 伸び縮みするロープの回転に関する関数 */
		RotateStretchRope();
		
		/** ロープの伸び縮みに関する関数 */
		StretchRope();

		/** 牛の位置と回転を取得 */
		Vector3 cowPos = m_hitCow->GetPosition();
		cowPos.y += 30.0f;
		Quaternion cowRot = m_hitCow->GetRotation();

		/** 捕まった牛用のロープモデルに反映 */
		m_rollModelRender.SetPosition(cowPos);
		m_rollModelRender.SetRotation(cowRot);

	}

	/** 牛に当たっているかのフラグがtrueなら */
	if (m_isThrowRope or m_isHitCow) {
		m_rollModelRender.SetScale(HIT_COW_ROLL_ROPE_SCALE);
	}

	m_rollModelRender.Update();
	m_ropeModelRender.Update();
}


void Rope::OnHitCow(Cow* cow)
{
	m_isHitCow = true;
	m_hitCow = cow;

	cow->SetIsCaptured(true);
}


void Rope::PlayerThrowsRope()
{
	/** ロープが投げられていたら */
	if (m_isThrowRope && !m_isStartRopeAnimation)
	{
		/** ロープアニメーション開始のフラグを立てる */
		m_isStartRopeAnimation = true;

		/** プレイヤーが向いている方向にロープを出す */
		m_ropeModelRender.SetRotation(m_player->GetRotation());

		m_ropeRot = m_player->GetRotation();
	}

	/** ロープアニメーションが開始していたら */
	if (m_isStartRopeAnimation)
	{
		/** ロープアニメーション時間を加算する */
		m_ropeAnimationTime += g_gameTime->GetFrameDeltaTime();

		/** ロープアニメーションが1秒より大きかったら */
		if (m_ropeAnimationTime > 1.0f)
		{
			m_ropeAnimationTime = 0.0f;
			m_isStartRopeAnimation = false;
			m_isThrowRope = false;
		}
	}
}


void Rope::FollowRightHand()
{
	Vector3 playerPos = m_player->GetPosition();
	Quaternion playerRot = m_player->GetRotation();

	Vector3 right = Vector3::AxisX;
	playerRot.Apply(right);

	Vector3 forward = Vector3::AxisZ;
	playerRot.Apply(forward);
	
	Vector3 ropePos =
		playerPos
		+ right * ROPE_OFFSET_RIGHT
		+ forward * ROPE_OFFSET_FORWARD
		+ Vector3(0.0f, ROPE_OFFSET_UP, 0.0f);

	m_ropePos = ropePos;
	m_ropeModelRender.SetPosition(ropePos);
	m_rollModelRender.SetScale(NO_HIT_COW_ROLL_ROPE_SCALE);
	m_rollModelRender.SetPosition(ropePos);
	m_rollModelRender.SetRotation(playerRot);

}


void Rope::StretchRope()
{

	if (!m_isHitCow || m_hitCow == nullptr) return;

	/** ロープの位置と牛の位置から距離を求める */
	Vector3 ropePos = m_ropePos;
	Vector3 cowPos = m_hitCow->GetPosition();

	float distance = (cowPos - ropePos).Length();

	/** ロープの伸び縮みのスケールを求める */
	/** ロープの伸び縮みのスケールは距離に
	 * ROPE_SCALE_FACTORをかけた値とROPE_MIN_SCALE_Zのうち
	 * 大きい方にする */
	float ropeScaleZ = max(distance * ROPE_SCALE_FACTOR, ROPE_MIN_SCALE_Z);

	m_ropeScale = Vector3(1.0f, 1.0f, ropeScaleZ);

	/** 伸び縮みだけ */
	m_ropeModelRender.SetScale(m_ropeScale);

}


void Rope::RotateStretchRope()
{
	if (m_hitCow == nullptr) return;
	if (m_hitCow->IsDead())
	{
		m_hitCow = nullptr;
		return;
	}
	/** ロープの位置から牛の位置へのベクトルを求める */
	Vector3 start = m_ropePos;
	Vector3 end = m_hitCow->GetPosition();
	
	/** 牛の位置の少し上を狙う */
	end.y += ROPE_AIM_UP_OFFSET;

	/** ベクトルを正規化する */
	Vector3 dir = end - start;
	dir.Normalize();

	/** ロープの初期の向きはZ軸方向なので
	 *Z軸をベクトルdirの方向に回転させるクォータニオンを求める
	 */
	Vector3 forward = Vector3::AxisZ;

	Quaternion rot;
	rot.SetRotation(forward, dir);

	m_ropeRot = rot;
	m_ropeModelRender.SetRotation(m_ropeRot);

}


void Rope::RotateRope()
{
	/** カメラの前にモデルを出す */
	GameCamera* gameCamera = FindGO<GameCamera>("gameCamera");

	if(gameCamera == nullptr) return;

	/** 牛に当たっているかのフラグがtrueならこの処理はしない */
	if (gameCamera->GetIsCowCaptured())
	{
		return;
	}

	/** ロープを投げている最中なら */
	if (m_isThrowRope)
	{
		
		
		/** カメラの前方向を取得 */
		Vector3 camForward = gameCamera->GetCameraForward();

		/** カメラの前にロープを出す */
		Vector3 camPos = gameCamera->GetCameraPosition();
		Vector3 ropePos = camPos + camForward * 50.0f;
		m_rollModelRender.SetPosition(ropePos);

		/** モデルの回転補正90度回転 */
		Quaternion fixRot;
		fixRot.SetRotation(Vector3::AxisX, 90.0f);

		/** forwardから回転を作る */
		Quaternion camRot;
		camRot.SetRotation(Vector3::AxisZ, camForward);

		m_ropeRot = camRot;

		/** ロープをスピンさせるための回転 */
		Quaternion spinRot;
		spinRot.SetRotation(Vector3::AxisY, m_ropeAnimationTime * 10.0f);
		m_ropeRot *= spinRot * fixRot;

		m_rollModelRender.SetRotation(m_ropeRot);
	}
}


void Rope::Render(RenderContext& rc)
{
	
	m_rollModelRender.Draw(rc);

	/** ロープを投げている最中か
	牛に当たっているならロープモデルを描画する */
	if (m_isThrowRope or m_isHitCow)
	{
		m_ropeModelRender.Draw(rc);
	}
}