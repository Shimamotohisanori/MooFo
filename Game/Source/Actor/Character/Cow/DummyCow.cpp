#include "stdafx.h"
#include "DummyCow.h"
namespace
{
	/** 牛のモデルファイルパス*/
	const char* FILEPATH = "Assets/ModelData/Cow/Model/Cow4.tkm";

	/** 牛のジャンプアニメーション*/
	const char* JUMP_ANIMATIOM_PATH = "Assets/ModelData/Cow/Animation/Jump.tka";
}
DummyCow::DummyCow()
{
	/** ジャンプアニメーションのみロードする*/
	animationClips[EnAnimation_Jump].Load(JUMP_ANIMATIOM_PATH);
	animationClips[EnAnimation_Jump].SetLoopFlag(false);
}


DummyCow::~DummyCow()
{
	
}


bool DummyCow::Start()
{
	/** モデルの初期化*/
	m_dummyCowModelRender.Init(FILEPATH,animationClips,EnAnimation_Num, enModelUpAxisZ);
	/** 位置と回転を反映*/
	m_dummyCowModelRender.SetPosition(m_position);
	m_dummyCowModelRender.SetRotation(m_rotation);
	if (m_requestPlayJump)
	{
		/** 救出した後はY軸のみ0にする*/
		m_position.y = 0.0f;
		m_dummyCowModelRender.SetPosition(m_position);
		m_dummyCowModelRender.PlayAnimation(EnAnimation_Jump);
		m_isPlaying = true;
	}

	return true;
}


void DummyCow::Update()
{
	/** ジャンプアニメーション中なら*/
	if (m_isPlaying &&!m_dummyCowModelRender.IsPlayingAnimation())
	{
		DeleteGO(this);
		return;
	}
	m_dummyCowModelRender.Update();
}


void DummyCow::PlayJumpAnimtion()
{
	m_requestPlayJump = true;
}

void DummyCow::Render(RenderContext& rc)
{
	m_dummyCowModelRender.Draw(rc);
}