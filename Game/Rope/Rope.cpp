#include "stdafx.h"
#include "Rope.h"
#include "Source/Actor/Character/Player/Player.h"
namespace
{
	const char* FILEPATH = "Assets/modelData/Rope/Rope.tkm";
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

	m_ropeModelRender.Init(FILEPATH/*, m_ropeAnimationClips, EnRopeAnimation_Num, enModelUpAxisZ*/);

	m_player = FindGO<Player>("player");

	/** ロープは常にプレイヤーの少し右前に付いてくる */
	m_ropeModelRender.SetPosition(m_player->GetPosition().x + 5.0f, m_player->GetPosition().y + 5.0f, m_player->GetPosition().z + 5.0f);

	return true;
}

void Rope::Update()
{
	/** ロープをプレイヤーの少し右前に付いてくるようにする */
	m_ropeModelRender.SetPosition(m_player->GetPosition().x + 5.0f, m_player->GetPosition().y + 5.0f, m_player->GetPosition().z + 5.0f);

	/** プレイヤーがロープを投げる処理 */
	PlayerThrowsRope();

	m_ropeModelRender.Update();
}

void Rope::PlayerThrowsRope()
{
	/** ロープが投げられていたら */
	if (m_isThrowRope)
	{
		/** ロープのアニメーションを再生する */
		//m_ropeModelRender.PlayAnimation(EnRopeAnimation_Throw);

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

void Rope::Render(RenderContext& rc)
{
	/** ロープが投げられていたらロープを表示する */
	if (m_isThrowRope)
	{
		m_ropeModelRender.Draw(rc);
	}
}


