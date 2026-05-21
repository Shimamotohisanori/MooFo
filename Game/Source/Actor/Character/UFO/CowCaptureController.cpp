#include "stdafx.h"
#include "Source/Actor/Character/UFO/CowCaptureController.h"
#include "UFO.h"
#include "CountDown/CountDown.h"
#include "Pause/Pause.h"
#include "GameScene/Game.h"
#include "EffectManager/EffectManager.h"

CowCaptureController::CowCaptureController()
{

}


CowCaptureController::~CowCaptureController()
{
	if (m_ufoLightEffect)
	{
		if (!m_ufoLightEffect->IsDead())
		{
			m_ufoLightEffect->Stop();
			DeleteGO(m_ufoLightEffect);
		}

		m_ufoLightEffect = nullptr;
	}

	m_isCapturing = false;
	m_isEmitting = false;
	m_prevIsEmitting = false;
}


bool CowCaptureController::Start()
{

	/**　最初は光が出ていない状態にする */
	m_timer = m_waitTimer;

	return true;
}


void CowCaptureController::Update()
{
	/** 光のエフェクトを更新できるか判断する関数 */
	if (!CanUFOLightUpdate())
	{
		return;
	}

	/** タイマーのカウント処理 */
	CountTimer();
	
	/** UFOに光を追従させる処理 */
	FollowUFO();
}


void CowCaptureController::CountTimer()
{
	switch (m_state)
	{
	case Wait:
		m_timer -= g_gameTime->GetFrameDeltaTime();
		if (m_timer <= 0.0f)
		{
			m_state = Emit;
			m_timer = m_emitTimer;
		}
		break;
	
	case Emit:
		m_timer -= g_gameTime->GetFrameDeltaTime();
		if (m_timer <= 0.0f)
		{
			m_state = Wait;
			m_timer = m_waitTimer;
		}
		break;
	}

	/** 状態に応じてフラグ更新 */
	m_isEmitting = (m_state == Emit || m_state == Capture);

	/** Emitに入った瞬間にエフェクトを再生する */
	if (!m_prevIsEmitting && m_isEmitting)
	{
		PlayLightEffect();
	}

	m_prevIsEmitting = m_isEmitting;

}

void CowCaptureController::FollowUFO()
{
	if (!m_ufo) return;

	Vector3 pos = m_ufo->GetPosition();
	pos.y -= 60.0f;

	/** 探索の光エフェクトが存在する場合 */
	if (m_ufoLightEffect)
	{

		/** Emit状態でなく、かつ牛を捕獲していないときはエフェクトを削除する */
		if (!m_isEmitting && !m_isCapturing)
		{
			DeleteGO(m_ufoLightEffect);
			m_ufoLightEffect = nullptr;
			return;
		}

		/** UFOの位置に光エフェクトを追従させる */
		m_ufoLightEffect->SetPosition(pos);
	}
}


void CowCaptureController::PlayLightEffect()
{
	if (!m_ufo) return;

	/** Emit状態で
	 * かつ牛を捕獲していないときにエフェクトを生成する */
	if (m_state == Emit || m_state == Capture)
	{
		/** 既存のエフェクトを破棄する */
		if (m_ufoLightEffect && !m_ufoLightEffect->IsDead())
		{
			m_ufoLightEffect->Stop();
			DeleteGO(m_ufoLightEffect);
			m_ufoLightEffect = nullptr;
		}

		/** 探索中のエフェクトを生成 */
		m_ufoLightEffect = NewGO<nsK2EngineLow::EffectEmitter>(0);
		m_ufoLightEffect->Init((int)EffectID::EffectID_UFOLight);

		/** UFO のライトは大きい方が見える */
		m_ufoLightEffect->SetScale({ 2.0f, 3.8f, 2.0f });

		/** UFOの光エフェクトを再生 */
		m_ufoLightEffect->Play();
	}
	
}

bool CowCaptureController::CanUFOLightUpdate()
{
	m_game = FindGO<Game>("game");
	m_countdown = FindGO<CountDown>("countdown");
	m_pause = FindGO<Pause>("pause");

	/** ゲーム、カウントダウン、ポーズのオブジェクトが存在しないときは処理を止める */
	if (m_pause == nullptr || m_countdown == nullptr || m_game == nullptr)
	{
		return false;
	}

	/** タイムアウトしているときは処理を止める */
	if (m_game->GetIsTimeOut())
	{
		/** 捕獲中でも光を止める */
		if (m_ufoLightEffect)
		{
			DeleteGO(m_ufoLightEffect);
			m_ufoLightEffect = nullptr;
		}
		return false;
	}

	/** カウントダウン中は処理を止める */
	if (m_countdown->GetCountDown())
	{
		return false;
	}

	/** Pause中は処理を止める */
	if (m_pause->GetIsPause())
	{
		return false;
	}

	/** UFOが存在しない場合は処理を止める */
	if (m_ufo == nullptr)
	{
		return false;
	}

	return true;
}


void CowCaptureController::Render(RenderContext& rc)
{
	
}
	
