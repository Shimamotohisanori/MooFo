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
	if (m_heartEffect)
	{
		DeleteGO(m_heartEffect);
	}
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

	/** 牛の前方向を取得する。 */
	Vector3 forward = Vector3::AxisZ;

	/** 回転の値を位置に変換することで、前方向を常に見る。 */
	m_rotation.Apply(forward);

	/** 方向を正規化する。 */
	forward.Normalize();

	Vector3 heartPos = Vector3{ m_position.x,m_position.y + 50.0f,m_position.z } + forward * 80.0f;

	m_heartEffect->SetPosition(heartPos);

	m_heartEffect->SetScale({ 10.0f,10.0f,10.0f });
	
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