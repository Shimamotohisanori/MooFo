#include "stdafx.h"
#include "UFOLightManager.h"
#include"CountDown/CountDown.h"
#include"Pause/Pause.h"
#include"GameScene/Game.h"
UFOLightManager::UFOLightManager()
{
	/** 初期化*/
	m_timer = m_waitTimer;
}

UFOLightManager::~UFOLightManager()
{

}

bool UFOLightManager::Start()
{
	return true;
}

void UFOLightManager::Update()
{
	/** アップデートできるかどうかを判断する関数 */
	if (!CanUpdate())
	{
		return;
	}
	/** タイマーのカウント処理 */
	CountTimer();
}

void UFOLightManager::CountTimer()
{
	m_timer -= g_gameTime->GetFrameDeltaTime();

	switch (m_state)
	{
	case CowCaptureController::Wait:
		if (m_timer <= 0.0f)
		{
			m_state = CowCaptureController::Emit;
			m_timer = m_emitTimer;
		}
		break;
	case CowCaptureController::Emit:
		if (m_timer <= 0.0f)
		{
			m_state = CowCaptureController::Wait;
			m_timer = m_waitTimer;
		}
		break;
	}
	
	for (auto ufo : m_ufos)
	{
		ufo->GetCowCaptureController()->SyncState(m_state);
	}
}

bool UFOLightManager::CanUpdate()
{
	m_game = FindGO<Game>("game");
	m_countdown = FindGO<CountDown>("countdown");
	m_pause = FindGO<Pause>("pause");
	/** ゲーム、カウントダウン、ポーズのオブジェクトが存在しないときは処理を止める */
	if (!m_game || !m_countdown || !m_pause)
	{
		return false;
	}
	/** ゲームがタイムアウトしている場合は処理を止める */
	if (m_game->GetIsTimeOut())
	{
		return false;
	}
	/** カウントダウン中のときは処理を止める */
	if (m_countdown->GetIsCountDown())
	{
		return false;
	}
	if (m_pause->GetIsPause())
	{
		return false;
	}
	return true;
}