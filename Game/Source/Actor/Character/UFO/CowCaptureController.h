#pragma once
class UFO;
class CowCaptureController : public IGameObject
{
public:
	CowCaptureController();
	~CowCaptureController();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	enum UFOLightState
	{
		Wait,/** 光待ち */
		Emit,/** 光出てる */
		Capture/** 捕獲中 */
	};
	UFOLightState m_state = Wait;
	/** 対象のUFOのセット関数 */
	void SetUFO(UFO* ufo)
	{
		m_ufo = ufo;
	}

	
	/** マネージャーから状態を同期する関数 */
	void SyncState(UFOLightState state);

	/** 捕獲中かどうかのフラグの取得関数 */
	bool GetIsCapturing() const
	{
		return m_isCapturing;
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
	}

	

	/** 光が出ているかどうかのフラグの取得関数 */
	bool GetIsEmitting() const
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
	
	/** 光が出ているかどうかのフラグ */
	bool m_isEmitting = false;
	
	/** 牛を捕獲しているかどうかのフラグ */
	bool m_isCapturing = false;

	/** UFO */
	UFO* m_ufo = nullptr;


	/** UFOの光のエフェクト */
	EffectEmitter* m_ufoLightEffect = nullptr;
};

