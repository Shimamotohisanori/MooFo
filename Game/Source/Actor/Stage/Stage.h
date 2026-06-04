#pragma once
#include "Source/Actor/Actor.h"

/**
 * ステージクラス
 */
class Stage : public Actor
{
public:
	Stage();
	~Stage();

private:
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;


private:

	/** モデルの表示 */
	ModelRender m_perimeterFenceModelRender;
	ModelRender m_innerFenceModelRender;
	ModelRender m_mountainModelRender;
	ModelRender m_groundModelRender;

	/** 当たり判定を追加する */
	PhysicsStaticObject m_perimeterObject;
	PhysicsStaticObject m_innerObject;

};

