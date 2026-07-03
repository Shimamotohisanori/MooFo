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
	return true;
}


void Stage::Update()
{

}

bool  Stage::LoadStepByStep()
{
	switch (m_initStep)
	{
		/** 外周フェンスと牛舎のモデル*/
	case InitStep::PerimeterFence:
		m_perimeterFenceModelRender.Init(PERIMETER_FILE_PATH);
		m_perimeterFenceModelRender.SetPosition(STAGE_POS);
		m_perimeterFenceModelRender.Update();
		m_initStep = InitStep::InnerFence;
		break;

		/** 内周フェンスのモデル */
	case InitStep::InnerFence:
		m_innerFenceModelRender.Init(INNER_FILE_PATH);
		m_innerFenceModelRender.SetPosition(STAGE_POS);
		m_innerFenceModelRender.Update();
		m_initStep = InitStep::Mountain;
		break;

		/** 外周の山のモデル */
	case InitStep::Mountain:
		m_mountainModelRender.Init(MOUNTAIN_FILE_PATH);
		m_mountainModelRender.SetPosition(STAGE_POS);
		m_mountainModelRender.Update();
		m_initStep = InitStep::Ground;
		break;

		/** 地面のモデル */
	case InitStep::Ground:
		m_groundModelRender.Init(GROUND_FILE_PATH);
		m_groundModelRender.SetPosition(STAGE_POS);
		m_groundModelRender.Update();
		m_initStep = InitStep::Collision;
		break;

		/** 衝突判定 */
	case InitStep::Collision:
		/** 外周フェンス・内周フェンスに当たり判定をつける*/
		m_perimeterObject.CreateFromModel(
			m_perimeterFenceModelRender.GetModel(),
			m_perimeterFenceModelRender.GetModel().GetWorldMatrix());

		m_innerObject.CreateFromModel(
			m_innerFenceModelRender.GetModel(),
			m_innerFenceModelRender.GetModel().GetWorldMatrix());
		m_initStep = InitStep::Num;
		break;

	case InitStep::Num:
		break;
	}
	/** 全ステップが終わったらtrueを返す*/
	return m_initStep == InitStep::Num;
}

void Stage::Render(RenderContext& rc)
{
	/** 外周フェンスはPerimeterFenceステップ完了後(=InnerFence以降)にのみ描画可能 */
	if (m_initStep > InitStep::PerimeterFence)
	{
		m_perimeterFenceModelRender.Draw(rc);
	}

	/** 内周フェンスはInnerFenceステップ完了後(=Mountain以降)にのみ描画可能 */
	if (m_initStep > InitStep::InnerFence)
	{
		m_innerFenceModelRender.Draw(rc);
	}

	/** 山はMountainステップ完了後(=Ground以降)にのみ描画可能 */
	if (m_initStep > InitStep::Mountain)
	{
		m_mountainModelRender.Draw(rc);
	}

	/** 地面はGroundステップ完了後(=Collision以降)にのみ描画可能 */
	if (m_initStep > InitStep::Ground)
	{
		m_groundModelRender.Draw(rc);
	}
}
