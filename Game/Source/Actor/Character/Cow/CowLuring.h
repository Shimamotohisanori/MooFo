#pragma once
class CowFood;
class Player;
class Cow;
class CowLuring : public IGameObject
{
public:
	CowLuring(){}
	~CowLuring(){}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** 干し草の座標を設定する関数 */
	void SetPosition(const Vector3& pos);

	/** 干し草の座標を取得する関数 */
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	void StartShrink()
	{
		m_isShrinking = true;
	}

	bool IsShrinkDone() const
	{
		return !m_isScaling && !m_isShrinking && m_scale <= 0.0f;
	}
private:
	/** 干し草を置く処理 */
	void PutHay();

	/** 現在のスケール */
	float m_scale = 0.0f;

	/** スケールアニメーション中かどうか */
	bool m_isScaling = true;

	/** 縮小中かどうかのフラグ */
	bool m_isShrinking = false;

	/** 干し草の座標 */
	Vector3 m_position;

	/** 干し草のモデルレンダラー */
	ModelRender m_hayModelRender;
};

