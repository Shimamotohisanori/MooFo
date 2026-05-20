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
	/** 対象のUFOのセット関数 */
	void SetUFO(UFO* ufo)
	{
		m_ufo = ufo;
	}

	/** 光が出ているかどうかのフラグの取得関数 */
	bool GetIsEmitting() const
	{
		return m_isEmitting;
	}

	/** 牛が捕獲されているかどうかのフラグの取得関数 */
	void SetCapturing(bool flag)
	{
		m_isCapturing = flag;
		if (flag)
		{
			m_state = Capture;
		}
	}

	/** 捕獲終了の処理を行う関数 */
	void EndCaptured()
	{
		/** UFOの光のエフェクトが存在しない場合は何もしない */
		if (m_ufoLightEffect == nullptr)
		{
			return;
		}

		/** 既に死んでるなら何もしない */
		if (m_ufoLightEffect->IsDead())
		{
			m_ufoLightEffect = nullptr;
			return;
		}

		m_ufoLightEffect->Stop();

		DeleteGO(m_ufoLightEffect);

		m_isCapturing = false;

		m_ufoLightEffect = nullptr;

		m_state = Wait;

		m_timer = m_waitTimer;
	}

	enum UFOLightState
	{
		Wait,/** 光待ち */
		Emit,/** 光出てる */
		Capture/** 捕獲中 */
	};
	UFOLightState m_state = Wait;
		
	/** UFOが光を出すまでの時間をカウントする関数 */
	void CountTimer();

	/** タイマーの値を取得する関数 */
	float GetTimer() const
	{
		return m_timer;
	}

	/** 光が出ているかどうかのフラグの取得関数 */
	bool IsEmitting() const
	{
		return m_isEmitting;
	}


private:
	/** 光をUDOに追従させる関数 */
	void FollowUFO();

	/** UFOの光を出す関数 */
	void PlayLightEffect();

	/** アップデートできるかどうかを判断する関数 */
	bool CanUFOLightUpdate();

private:	
		
	/** 光が出るまでの時間 */
	float m_waitTimer = 5.0f;

	/** 光が出ている時間 */
	float m_emitTimer = 5.0f;
	
	/** タイマー */
	float m_timer = 0.0f;
	
	/** 光が出ているかどうかのフラグ */
	bool m_isEmitting = false;
	
	/** 牛を捕獲しているかどうかのフラグ */
	bool m_isCapturing = false;

	/** Emitに入った瞬間を記録するフラグ */
	bool m_prevIsEmitting = false;

	/** UFO */
	UFO* m_ufo = nullptr;

	/** ポーズ */
	Pause* m_pause = nullptr;

	/** カウントダウン */
	CountDown* m_countdown = nullptr;

	/** ゲーム */
	Game* m_game = nullptr;

	/** UFOの光のエフェクト */
	EffectEmitter* m_ufoLightEffect = nullptr;
};

