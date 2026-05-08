#include "stdafx.h"
#include "Stage.h"
namespace
{
	Vector3 STAGE_POS = { 0.0f, 160.0f, 0.0f };

	Vector3 STAGEMAP_POS = { 0.0f,0.0f,0.0f };
}

Stage::Stage()
{
}

Stage::~Stage()
{
}

bool Stage::Start()
{
	m_stageModelRender.Init("Assets/modelData/Stage/Stage.tkm");
	m_stageModelRender.SetPosition(STAGE_POS);
	m_stageModelRender.Update();

	m_mapStageModelRender.Init("Assets/modelData/Stage/StageGround.tkm");
	m_mapStageModelRender.SetPosition(STAGEMAP_POS);
	m_mapStageModelRender.Update();
	
	return true;
}

void Stage::Update()
{
}

void Stage::Render(RenderContext& rc)
{
	m_stageModelRender.Draw(rc);
	m_mapStageModelRender.Draw(rc);
}
