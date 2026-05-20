#include "stdafx.h"
#include"SoundManager/SoundManager.h"
#include "DummyCow.h"
#include "EffectManager/EffectManager.h"

namespace
{
	/** 牛のモデルファイルパス */
	const char* FILEPATH = "Assets/ModelData/Cow/Model/Cow4.tkm";

	/** 牛のジャンプアニメーション */
	const char* JUMP_ANIMATIOM_PATH = "Assets/ModelData/Cow/Animation/Jump.tka";
}


DummyCow::DummyCow()
{
	/** ジャンプアニメーションのみロードする */
	animationClips[EnAnimation_Jump].Load(JUMP_ANIMATIOM_PATH);
	animationClips[EnAnimation_Jump].SetLoopFlag(false);
}


DummyCow::~DummyCow()
{

}


bool DummyCow::Start()
{
	m_DummyCowSE = FindGO<SoundManager>("soundmanager");
	m_dummyCowModelRender.SetRaytracingWorld(false);
	/** モデルの初期化*/
	m_dummyCowModelRender.Init(FILEPATH,animationClips,EnAnimation_Num, enModelUpAxisZ);
	m_dummyCowModelRender.SetPosition(m_position);
	m_dummyCowModelRender.SetRotation(m_rotation);

	m_heartEffect = NewGO<nsK2EngineLow::EffectEmitter>(0);
	m_heartEffect->Init((int)EffectID::EffectID_HeartEffect);

	m_heartEffect->SetPosition(m_position);

	m_heartEffect->SetScale({ 20.0f,20.0f,20.0f });
	
	m_heartEffect->Play();
	m_heartEffect->Update();

	

	return true;
}


void DummyCow::Update()
{
	if (m_requestPlayJump && !m_isPlaying)
	{
		/** アニメーションを再生*/
		m_dummyCowModelRender.PlayAnimation(EnAnimation_Jump);
		/** 救出した後のSEを流す*/
		if (!m_RescueSE && m_DummyCowSE != nullptr)
		{
			m_RescueCowSE = m_DummyCowSE->PlayingSE(SoundSE::enRescueCowSE, false);
			m_RescueCowSE->SetVolume(5.0f);
			m_RescueSE = true;
		}
		m_isPlaying = true;
	}
	if(m_requestPlayJump)
	{
		/** 救出した後はY軸のみ0にする */
		m_position.y = 0.0f;
		m_dummyCowModelRender.SetPosition(m_position);
     }
	/** モデル更新*/
	m_dummyCowModelRender.Update();
  /** ジャンプアニメーション中なら */
	if (m_isPlaying &&!m_dummyCowModelRender.IsPlayingAnimation())
	{
		m_deleteDelay++;
		if (m_deleteDelay > 5)
		{
			m_isDelete = true;
		}
		
	   return;
	}
	
}

void DummyCow::PlayJumpAnimtion()
{
	m_requestPlayJump = true;
}


void DummyCow::Render(RenderContext& rc)
{
	m_dummyCowModelRender.Draw(rc);
}