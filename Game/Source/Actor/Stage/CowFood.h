#pragma once

class CowFood : public IGameObject
{
public:
	CowFood(){}
	~CowFood(){}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_cowFoodModelRender;

	PhysicsStaticObject m_FoodObject;
};