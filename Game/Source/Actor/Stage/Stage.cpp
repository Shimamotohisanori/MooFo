#include "stdafx.h"
#include "Stage.h"
#include "CowFood.h"
namespace
{

	/** 地面のモデルファイルパス */
	const char* GROUND_FILE_PATH = "Assets/modelData/Stage/Ground.tkm";

	/** 外周フェンスのモデルファイルパス */
	const char* PERIMETER_FILE_PATH = "Assets/modelData/Stage/PerimeterFence.tkm";

	/** 内周フェンスのモデルファイルパス */
	const char* INNER_FILE_PATH = "Assets/modelData/Stage/InnerFence.tkm";

	/** 外周の山のモデルファイルパス */
	const char* MOUNTAIN_FILE_PATH = "Assets/modelData/Stage/Mountain.tkm";

	/** ステージの座標 */
	Vector3 STAGE_POS = { 0.0f, 0.0f, 0.0f };
	
}

bool Stage::Start()
{
	/** 外周フェンスと牛舎のモデル */
	m_perimeterFenceModelRender.Init(PERIMETER_FILE_PATH);
	m_perimeterFenceModelRender.SetPosition(STAGE_POS);
	m_perimeterFenceModelRender.Update();

	/** 内周フェンスのモデル */
	m_innerFenceModelRender.Init(INNER_FILE_PATH);
	m_innerFenceModelRender.SetPosition(STAGE_POS);
	m_innerFenceModelRender.Update();

	/** 外周の山のモデル */
	m_mountainModelRender.Init(MOUNTAIN_FILE_PATH);
	m_mountainModelRender.SetPosition(STAGE_POS);
	m_mountainModelRender.Update();

	/** 地面のモデル */
	m_groundModelRender.Init(GROUND_FILE_PATH);
	m_groundModelRender.SetPosition(STAGE_POS);
	m_groundModelRender.Update();

	/** 外周フェンス・内周フェンス・牛の餌に当たり判定をつける */
	m_perimeterObject.CreateFromModel(m_perimeterFenceModelRender.GetModel(), m_perimeterFenceModelRender.GetModel().GetWorldMatrix());
	m_innerObject.CreateFromModel(m_innerFenceModelRender.GetModel(), m_innerFenceModelRender.GetModel().GetWorldMatrix());
	
	return true;
}


void Stage::Update()
{

}


void Stage::Render(RenderContext& rc)
{
	m_perimeterFenceModelRender.Draw(rc);
	m_innerFenceModelRender.Draw(rc);
	m_mountainModelRender.Draw(rc);
	m_groundModelRender.Draw(rc);
}
