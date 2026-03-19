#include "stdafx.h"
#include "Rope.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "Source/Actor/Character/Player/Player.h"
namespace
{
	const char* FILEPATH = "Assets/modelData/Rope/NewRope.tkm";
	const char* THROW_ROPE_ANIMATION_FILE_PATH = "Assets/animData/Rope/ThrowRope.tka";
}
Rope::Rope()
{
}

Rope::~Rope()
{
}

bool Rope::Start()
{

	m_ropeAnimationClips[EnRopeAnimation_Throw].Load(THROW_ROPE_ANIMATION_FILE_PATH);
	m_ropeAnimationClips[EnRopeAnimation_Throw].SetLoopFlag(false);

	m_ropeModelRender.Init(FILEPATH);

	m_player = FindGO<Player>("player");

	/** ロープは常にプレイヤーの少し右前に付いてくる */
	m_ropeModelRender.SetPosition(m_player->GetPosition().x + 5.0f, m_player->GetPosition().y + 5.0f, m_player->GetPosition().z + 5.0f);

	m_ropeModelRender.SetScale(Vector3(1.0f, 1.0f, 5.0f));

	return true;
}

void Rope::Update()
{
	/** ロープをプレイヤーの少し右前に付いてくるようにする */
	//m_ropeModelRender.SetPosition(m_player->GetPosition().x + 5.0f, m_player->GetPosition().y + 5.0f, m_player->GetPosition().z + 5.0f);
	
	m_cow = FindGO<Cow>("cow");

	/** プレイヤーがロープを投げる処理 */
	PlayerThrowsRope();

	/** プレイヤーの右手の位置のロープが常にある関数 */
	FollowRightHand();

	if (m_isHitCow && m_cow != nullptr)
	{
		/** 伸び縮みするロープの回転に関する関数 */
		RotateStretchRope();
		
		/** ロープの伸び縮みに関する関数 */
		StretchRope();
	}

	

	
	
	m_ropeModelRender.Update();
}

void Rope::PlayerThrowsRope()
{
	/** ロープが投げられていたら */
	if (m_isThrowRope)
	{
		/** ロープアニメーション開始のフラグを立てる */
		m_isStartRopeAnimation = true;

		/** プレイヤーが向いている方向にロープを出す */
		m_ropeModelRender.SetRotation(m_player->GetRotation());
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
		+ right * 10.0f
		+ forward * 0.5f
		+ Vector3(0.0f, 30.0f, 0.0f);

	m_ropePos = ropePos;
	m_ropeModelRender.SetPosition(ropePos);

	//m_ropeModelRender.SetRotation(playerRot);
}

void Rope::StretchRope()
{

	if (!m_isHitCow || m_cow == nullptr) return;

	Vector3 ropePos = m_ropePos;
	Vector3 cowPos = m_cow->GetPosition();

	float distance = (cowPos - ropePos).Length();

	float ropeScaleZ = max(distance * 0.05f, 0.002f);

	// 伸び縮みだけ
	m_ropeModelRender.SetScale(Vector3(1.0f, 1.0f, ropeScaleZ));

}

void Rope::RotateStretchRope()
{
	if (m_cow == nullptr) return;

	Vector3 start = m_ropePos;
	Vector3 end = m_cow->GetPosition();

	Vector3 dir = end - start;
	dir.Normalize();

	Vector3 forward = Vector3::AxisZ;   // ロープが +Z に伸びてる前提

	Quaternion rot;
	rot.SetRotation(forward, dir);      // from → to の回転を作る

	m_ropeModelRender.SetRotation(rot);

}

void Rope::Render(RenderContext& rc)
{
	/** ロープが投げられていて
	 *牛に当たっていたらロープを表示する */
	if (m_isThrowRope or m_isHitCow)
	{
		m_ropeModelRender.Draw(rc);
	}
}


