#pragma once
class CowShrinkHay : public IGameObject
{
public:
	CowShrinkHay(){}
	~CowShrinkHay(){}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
private:
	float m_scale = 1.0f;

	/** 牛が餌を食べ終わった後のエフェクト */
	EffectEmitter* m_afterMealEffect;
	
	Vector3 m_position;
	ModelRender m_hayModelRender;
};

