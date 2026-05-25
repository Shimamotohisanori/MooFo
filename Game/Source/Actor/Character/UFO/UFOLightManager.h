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
	/** CowCaptureControllerの管理 */
	/*void RegisterCowCaptureController(CowCaptureController* controller)
	{
		m_Controllers.push_back(controller);
	}*/

	void RegisterUFO(UFO* ufo)
	{
		m_ufos.push_back(ufo);
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
	float m_waitTimer = 5.0f;
	float m_emitTimer = 5.0f;
	float m_timer = 0.0f;

	CowCaptureController::UFOLightState m_state = CowCaptureController::Wait;
	std::vector<UFO*> m_ufos;
	//std::vector<CowCaptureController*>m_Controllers;

	Game* m_game = nullptr;
	CountDown* m_countdown = nullptr;
	Pause* m_pause = nullptr;
};

