#include "stdafx.h"
#include "Source/Actor/Character/UFO/CowCaptureController.h"
#include "UFO.h"
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
}


bool CowCaptureController::Start()
{

	return true;
}


void CowCaptureController::Update()
{
	/** 光のエフェクトを更新できるか判断する関数 */
	if (!CanUFOLightUpdate())
	{
		return;
	}

	
	/** UFOに光を追従させる処理 */
	FollowUFO();
}

void CowCaptureController::FollowUFO()
{
	if (!m_ufo) return;

	Vector3 pos = m_ufo->GetPosition();
	pos.y -= 60.0f;

	/** 光エフェクトが存在する場合 */
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

		/** エフェクトが再生されていないときは再生する */
		if (!m_ufoLightEffect->IsPlay())
		{
			m_ufoLightEffect->Play();
		}
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

void CowCaptureController::SyncState(UFOLightState state) {
	/** 自分のUFOが牛を捕獲中なら
	 * グローバル状態を無視してCapture状態で固定する */
	if (m_ufo && m_ufo->GetIsCowTakeAwayed())
	{
		UFOLightState prevState = m_state;
		m_state = Capture;
		m_isEmitting = true;
		m_isCapturing = true;

		/** WaitからCaptureの遷移瞬間だけエフェクトを再生 */
		bool justStarted = (prevState == Wait || prevState == Emit);
		if (justStarted)
		{
			PlayLightEffect();
		}
		return;
	}


	UFOLightState prevState = m_state;
	m_state = state;

	/**  捕獲中フラグも状態に合わせて更新する*/
	m_isEmitting = (m_state == Emit || m_state == Capture);
	m_isCapturing = (m_state == Capture);


	/** WaitからEmitの際に新たに光を出す */
	bool justStartedEmitting =
		(prevState == Wait) && (m_state == Emit);
	if (justStartedEmitting)
	{
		PlayLightEffect();
	}

	/** CaptureからWaitまたはEmitに遷移した際にエフェクトを停止 */
	bool justStoppedCapture =
		(prevState == Capture) && (m_state == Wait || m_state == Emit);

	if (justStoppedCapture)
	{
		if (m_ufoLightEffect)
		{
			m_ufoLightEffect->Stop();
			DeleteGO(m_ufoLightEffect);
			m_ufoLightEffect = nullptr;
		}

		/** Emitなら即座にエフェクトを再生する
		 * (カウント途中でも光を出すタイミングなら合流) */
		if (m_state == Emit)
		{
			PlayLightEffect();
		}
	}

	/** EmitからWaitの際にエフェクトを消す */
	bool justStoppedEmitting =
		(prevState == Emit) && (m_state == Wait);
	if (justStoppedEmitting)
	{
		if (m_ufoLightEffect)
		{
			m_ufoLightEffect->Stop();
			DeleteGO(m_ufoLightEffect);
			m_ufoLightEffect = nullptr;
		}
	}
}
	
