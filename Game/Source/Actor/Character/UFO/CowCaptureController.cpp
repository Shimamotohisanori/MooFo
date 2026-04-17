#include "stdafx.h"
#include "Source/Actor/Character/UFO/CowCaptureController.h"
#include"UFO.h"
#include"CountDown/CountDown.h"
#include"Pause/Pause.h"
namespace
{
	const char* FILEPATH = "Assets/modelData/UFO/UFOLight.tkm";
	const Vector3 SCALE = Vector3(2.0f, 6.0f, 2.0f);
}

CowCaptureController::CowCaptureController()
{

}

CowCaptureController::~CowCaptureController()
{

}


bool CowCaptureController::Start()
{
	m_countdown = FindGO<CountDown>("countdown");
	m_pause = FindGO<Pause>("pause");
	m_ufocontrollermodelRender.Init(FILEPATH);
	/**　最初は光が出ていない状態にする*/
	m_timer = m_waitTimer;
	m_ufocontrollermodelRender.SetScale(SCALE);
	return true;
}


void CowCaptureController::Update()
{
	if (m_countdown->GetCountDown())
	{
		return;
	}

	/** Pause中は処理を止める*/
	if (m_pause->GetIsPause())
	{
		return;
	}

	if (m_ufo == nullptr)
	{
		return;
	}

	CountTimer();

	CountText();

	FollowTheCow();

	m_ufocontrollermodelRender.Update();
}


void CowCaptureController::FollowTheCow()
{
	if (m_ufo != nullptr) 
	{
		Vector3 pos = m_ufo->GetPosition();
		pos.y -= 150.0f;
		m_ufocontrollermodelRender.SetPosition(pos);
		
	}
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
	case Capture:
		break;
	}
	// 状態に応じてフラグ更新
	m_isEmitting = (m_state == Emit || m_state == Capture);
}

void CowCaptureController::CountText()
{
	/** 光が出ているときは表示しない*/
	if (m_isEmitting)
	{
		return;
	}
	int seconds = (int)ceilf(m_timer);
	wchar_t timertext[32];
	swprintf_s(timertext, 32, L"次の光発射まで%d", seconds);
	m_countText.SetText(timertext);
	/**　文字のセット*/
	m_countText.SetPosition(400.0f, 530.0f, 0.0f);
	/**　文字の大きさのセット*/
	m_countText.SetScale(1.0f);
	/**　文字の色のセット*/
	m_countText.SetColor(g_vec4White);
}

void CowCaptureController::Render(RenderContext& rc)
{
	/** 光が出ていないときだけカウント*/
	if (!m_isEmitting)
	{
		/** Pause中は描画を止める*/
		if (m_pause->GetIsPause())
		{
			return;
		}
		m_countText.Draw(rc);
	}
	
	if (m_ufo == nullptr)
	{
		return;
	}

	/** 光が出ているときもしくは、UFOが牛を捕まえているときのみ描画*/
	if (m_isEmitting or m_ufo->GetIsCowTakeAwayed())
	{
		m_ufocontrollermodelRender.Draw(rc);
	}
}
	
