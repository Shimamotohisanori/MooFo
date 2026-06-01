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
	ModelRender m_groundModelRender;
	ModelRender m_perimeterFenceModelRender;
	ModelRender m_innerFenceModelRender;
	ModelRender m_mountainModelRender;
};

