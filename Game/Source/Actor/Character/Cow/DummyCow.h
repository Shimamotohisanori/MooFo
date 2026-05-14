#pragma once
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

	
	/** ジャンプアニメーションを再生する関数 */
	void PlayJumpAnimtion();

private:
	/** ダミーの牛モデルレンダラー */
	ModelRender m_dummyCowModelRender;

	/** 座標 */
	Vector3 m_position = Vector3::Zero;

	/** 回転 */
	Quaternion m_rotation = Quaternion::Identity;

	enum EnCowAnimation
	{
		EnAnimation_Jump,
		EnAnimation_Num,

	}; 
	AnimationClip animationClips[EnAnimation_Num];

	/** アニメーションが再生中かを表すフラグ*/
	bool m_isPlaying = false;

	/** ジャンプアニメーションの再生を要求するフラグ */
	bool m_requestPlayJump = false;
};

