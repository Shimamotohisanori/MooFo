#pragma once
class SoundManager;
class DummyCow : public IGameObject
{
public:
	DummyCow();
	~DummyCow();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/** ポジションをセットする関数*/
	void SetPosition(Vector3 pos)
	{
		m_position = pos;
	}
	/** 回転をセットする関数*/
	void SetRotation(Quaternion rot)
	{
		m_rotation = rot;
	}

	/** 表示するモデルのパスを設定する関数(牛の見た目を引き継ぐため) */
	void SetModelPath(const char* modelPath)
	{
		m_modelPath = modelPath;
	}

	/** ジャンプアニメーションを再生する関数 */
	void PlayJumpAnimtion();

private:
	/** ダミーの牛モデルレンダラー */
	ModelRender m_dummyCowModelRender;
  
	/** 座標*/
	Vector3 m_position = Vector3::Zero;
	/** 回転*/
	Quaternion m_rotation = Quaternion::Identity;
	/** アニメーション*/
	enum EnCowAnimation
	{
		EnAnimation_Jump,/** ジャンプ*/
		EnAnimation_Num,
	}; 
	AnimationClip animationClips[EnAnimation_Num];

	/** アニメーションが再生中かを表すフラグ*/
	bool m_isPlaying = false;

	/** 満腹のエフェクトを出すかどうか */
	bool m_isOverdose = false;

	/** ジャンプアニメーションの再生を要求するフラグ */
	bool m_requestPlayJump = false;
	
	/** 牛の救出SEを流すためのフラグ*/
	bool m_RescueSE = false;

	/** 縮小中フラグ */
	bool m_isShrinking = false;

	/** 縮小スケール */
	float m_shrinkScale = 1.0f;

	const char* m_modelPath = nullptr;

	/** サウンドマネージャー*/
	SoundManager* m_DummyCowSE = nullptr;

	/**牛を救出した後のSE */
	SoundSource* m_RescueCowSE = nullptr;

	/** ハートのエフェクト */
	EffectEmitter* m_heartEffect = nullptr;
};

