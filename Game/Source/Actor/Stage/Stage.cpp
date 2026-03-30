#include "stdafx.h"
#include "Stage.h"
namespace
{
	Vector3 STAGE_POS = { 0.0f, 160.0f, 0.0f };
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

	m_treeModelRender.Init("Assets/modelData/Stage/Tree.tkm");
	m_treeModelRender.SetPosition(STAGE_POS);
	m_treeModelRender.Update();

	m_groundModelRender.Init("Assets/modelData/Stage/Ground.tkm");
	m_groundModelRender.SetPosition(STAGE_POS);
	m_groundModelRender.Update();
	return true;
}

void Stage::Update()
{
}

void Stage::Render(RenderContext& rc)
{
	m_stageModelRender.Draw(rc);
	m_treeModelRender.Draw(rc);
	m_groundModelRender.Draw(rc);
}
