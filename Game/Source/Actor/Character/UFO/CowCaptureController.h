#pragma once
class UFO;
class CountDown;
class Pause;
class Game;
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

	/** 捕獲終了の処理を行う関数*/
	void EndCaptured()
	{
		if (m_ufoLightEffect)
		{
			m_ufoLightEffect->Stop();
			m_ufoLightEffect = nullptr;
		}
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

	/** 秒数の描画処理*/
	void CountSpriteUI();


private:

	/** 光をUDOに追従させる関数 */
	void FollowUFO();

	/** UFOの光を出す関数*/
	void PlayLightEffect();

private:
	
	/** 光が出ているときに表示する数字のスプライトレンダー*/
	SpriteRender m_LightApperNumberSpriteRender[5];
	/** 「光の発射まで」を表示するスプライトレンダー*/
	SpriteRender m_LightApperSpriteRender;
	/** 「秒」を表示するスプライトレンダー*/
	SpriteRender m_secondsSpriteRender;
	/** 現在の秒数*/
	int m_currentCount = 0;
	/**光が出るまでの時間*/
	float m_waitTimer = 5.0f;
	/** 光が出ている時間*/
	float m_emitTimer = 5.0f;
	/**タイマー*/
	float m_timer = 0.0f;
	/** 光が出ているかどうかのフラグ*/
	bool m_isEmitting = false;
	/** 牛を捕獲しているかどうかのフラグ*/
	bool m_isCapturing = false;

	/** Emitに入った瞬間を記録するフラグ*/
	bool m_prevIsEmitting = false;

	UFO* m_ufo = nullptr;
	Pause* m_pause;
	CountDown* m_countdown;
	Game* m_game;

	/** UFOの光のエフェクト*/
	EffectEmitter* m_ufoLightEffect;
};

