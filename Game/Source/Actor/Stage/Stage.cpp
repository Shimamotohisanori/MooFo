#include "stdafx.h"
#include "Stage.h"

Stage::Stage()
{
}

Stage::~Stage()
{
}

bool Stage::Start()
{
	m_stageModelRender.Init("Assets/modelData/Stage/Stage.tkm");
	return true;
}

void Stage::Update()
{
}

void Stage::Render(RenderContext& rc)
{
	m_stageModelRender.Draw(rc);
}
