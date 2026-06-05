#pragma once

class CowFood : public IGameObject
{
public:
	CowFood(){}
	~CowFood();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

	bool m_iseffect = false; 

	/** 牛の餌のモデル */
	ModelRender m_cowFoodModelRender;

	/** 牛の餌の当たり判定 */
	PhysicsStaticObject m_FoodObject;

	/** 牛の餌のエフェクト */
	EffectEmitter* m_cowFoodEffect;
};