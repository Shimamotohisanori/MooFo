#pragma once
class UFO;
class CountDown;
class Pause;
class CowCaptureController : public IGameObject
{
	public:
	CowCaptureController();
	~CowCaptureController();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	void SetUFO(UFO* ufo)
	{
		m_ufo = ufo;
	}
	bool GetIsEmitting() const
	{
		return m_isEmitting;
	}

	void SetCapturing(bool flag)
	{
		m_isCapturing = flag;
	}

	void StartCapture()
	{
		
	}

	void EndCapture()
	{
		
	}

	enum UFOLightState
	{
		Wait,/** 光待ち*/
		Emit,/** 光出てる*/
		Capture/** 捕獲中*/
	};
	UFOLightState m_state = Wait;
	//共有させる
	static int m_capturingCount;
		
	/** UFOが光を出すまでの時間をカウントする関数*/
	void CountTimer();
	/**UFOに光を追従させる関数*/
	void FollowTheCow();
	/** 秒数の描画処理*/
	void CountText();
private:
	ModelRender m_ufocontrollermodelRender;
	SpriteRender m_LightApperNumberSpriteRender[5];/** 光が出ているときに表示する数字のスプライトレンダー*/
	SpriteRender m_LightApperSpriteRender;/** 「光の発射まで」を表示するスプライトレンダー*/
	SpriteRender m_secondsSpriteRender;/** 「秒」を表示するスプライトレンダー*/

	float m_waitTimer = 5.0f;/**光が出るまでの時間*/
	float m_emitTimer = 10.0f;/** 光が出ている時間*/
	float m_timer = 0.0f;/**タイマー*/
	//FontRender m_countText;/** 秒数の描画用のフォントレンダラー*/
	bool m_isEmitting = false;/** 光が出ているかどうかのフラグ*/
	bool m_isCapturing = false;/** 牛を捕獲しているかどうかのフラグ*/
	UFO* m_ufo = nullptr;
	Pause* m_pause;
	CountDown* m_countdown;
};

