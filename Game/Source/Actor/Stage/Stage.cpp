#include "stdafx.h"
#include "Stage.h"
namespace
{

	/** ステージのモデルファイルパス */
	const char* STAGE_MODEL_FILE_PATH = "Assets/modelData/Stage/Stage.tkm";

	/** ステージマップのモデルファイルパス */
	const char* STAGEMAP_MODEL_FILE_PATH = "Assets/modelData/Stage/StageGround.tkm";

	/** ステージの座標 */
	Vector3 STAGE_POS = { 0.0f, 160.0f, 0.0f };
	
	/** ステージマップの座標 */
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
	m_stageModelRender.Init(STAGE_MODEL_FILE_PATH);
	m_stageModelRender.SetPosition(STAGE_POS);
	m_stageModelRender.Update();

	m_mapStageModelRender.Init(STAGEMAP_MODEL_FILE_PATH);
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
