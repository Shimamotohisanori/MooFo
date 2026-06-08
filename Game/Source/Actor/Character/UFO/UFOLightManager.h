#pragma once
#include"Source/Actor/Character/UFO/CowCaptureController.h"
#include"Source/Actor/Character/UFO/UFO.h"
#include<vector>
class Game;
class CountDown;
class Pause;
class UFOLightManager : public IGameObject
{
public:
	UFOLightManager();
	~UFOLightManager();
	bool Start() override;
	void Update() override;
	/** UFOの管理を登録する*/
	void RegisterUFO(UFO* ufo)
	{
		m_ufos.push_back(ufo);
	}

	/** UFOの管理を解除する*/
	void UnregisterUFO(UFO* ufo)
	{
		m_ufos.erase(
			std::remove(m_ufos.begin(), m_ufos.end(), ufo),
			m_ufos.end()
		);
	}

	/** タイマーの取得関数 */
	float GetTimer() const
	{
		return m_timer;
	}

	/** 現在の状態を取得する関数*/
		CowCaptureController::UFOLightState GetState() const
		{
			return m_state;
		}
		/** 光が出ているかどうか*/
		bool IsEmitting() const
		{
			return m_state == CowCaptureController::Emit
			|| m_state == CowCaptureController::Capture;
		}

private:
	bool CanUpdate();
	void CountTimer();
private:
	/** 光が出るまでタイマー*/
	float m_waitTimer = 5.0f;
	/** 光が出ているタイマー*/
	float m_emitTimer = 5.0f;
	float m_timer = 0.0f;

	CowCaptureController::UFOLightState m_state = CowCaptureController::Wait;
	std::vector<UFO*> m_ufos;
	//std::vector<CowCaptureController*>m_Controllers;
	/** ゲームのポインタ*/
	Game* m_game = nullptr;
	/** カウントダウンのポインタ*/
	CountDown* m_countdown = nullptr;
	/** Pauseのポインタ*/
	Pause* m_pause = nullptr;
};

