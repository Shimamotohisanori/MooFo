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

	void SetPosition(Vector3 pos)
	{
		m_position = pos;
	}

	void SetRotation(Quaternion rot)
	{
		m_rotation = rot;
	}

	bool GetDelete()const 
	{
		return m_isDelete;
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
		EnAnimation_Jump,
		EnAnimation_Num,
	}; 
	AnimationClip animationClips[EnAnimation_Num];

	/** アニメーションが再生中かを表すフラグ*/
	bool m_isPlaying = false;
  
	/** SEの削除がアニメーションの削除と重ならないように遅らすための変数*/
	int m_deleteDelay = 0;

	/** ジャンプアニメーションの再生を要求するフラグ */
	bool m_requestPlayJump = false;
	/** 牛の救出SEを流すためのフラグ*/
	bool m_RescueSE = false;
	/**牛の削除予約*/
	bool m_isDelete = false;
	/** サウンドマネージャー*/
	SoundManager* m_DummyCowSE = nullptr;

	/**牛を救出した後のSE */
	SoundSource* m_RescueCowSE = nullptr;
};

