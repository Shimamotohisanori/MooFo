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

	
	/** ジャンプアニメーションを再生する関数*/
	void PlayJumpAnimtion();

private:
	ModelRender m_dummyCowModelRender;
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	enum EnCowAnimation
	{
		EnAnimation_Jump,
		EnAnimation_Num,

	};
	AnimationClip animationClips[EnAnimation_Num];
	/** アニメーションが再生中かを表すフラグ*/
	bool m_isPlaying = false;

	bool m_requestPlayJump = false;
};

