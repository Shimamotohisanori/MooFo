#include "stdafx.h"
#include "Stage.h"
namespace
{

	/** 地面のモデルファイルパス */
	const char* GROUND_FILE_PATH = "Assets/modelData/Stage/Ground.tkm";

	/** 外周フェンスのモデルファイルパス */
	const char* PERIMETER_FILE_PATH = "Assets/modelData/Stage/PerimeterFence.tkm";

	/** 内周フェンスのモデルファイルパス */
	const char* INNER_MODEL_FILE_PATH = "Assets/modelData/Stage/InnerFence.tkm";

	/** 外周の山のモデルファイルパス */

	/** ステージの座標 */
	Vector3 STAGE_POS = { 0.0f, 0.0f, 0.0f };
	
}


Stage::Stage()
{
}


Stage::~Stage()
{
}


bool Stage::Start()
{
	m_groundModelRender.Init(GROUND_FILE_PATH);
	m_groundModelRender.SetPosition(STAGE_POS);
	m_groundModelRender.Update();

	m_perimeterFenceModelRender.Init(PERIMETER_FILE_PATH);
	m_perimeterFenceModelRender.SetPosition(STAGE_POS);
	m_perimeterFenceModelRender.Update();

	m_innerFenceModelRender.Init(INNER_MODEL_FILE_PATH);
	m_innerFenceModelRender.SetPosition(STAGE_POS);
	m_innerFenceModelRender.Update();
	
	return true;
}


void Stage::Update()
{
}


void Stage::Render(RenderContext& rc)
{
	m_groundModelRender.Draw(rc);
	m_perimeterFenceModelRender.Draw(rc);
	m_innerFenceModelRender.Draw(rc);
}
