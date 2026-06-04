#include "stdafx.h"
#include "CowFood.h"
namespace
{
	/** 牛の餌のモデルファイルパス */
	const char* COWFOOD_FILE_PATH = "Assets/modelData/Stage/CowFood.tkm";

	/** 牛の餌の座標 */
	Vector3 COWFOOD_POS = { -1260.0f,-5.0f,-350.0f };

}

bool CowFood::Start()
{
	/** 牛の餌のモデル */
	m_cowFoodModelRender.Init(COWFOOD_FILE_PATH);
	m_cowFoodModelRender.SetPosition(COWFOOD_POS);
	m_cowFoodModelRender.Update();

	/** 牛の餌の当たり判定をつける */
	m_FoodObject.CreateFromModel(m_cowFoodModelRender.GetModel(), m_cowFoodModelRender.GetModel().GetWorldMatrix());

	return true;
}

void CowFood::Update()
{

}

void CowFood::Render(RenderContext& rc)
{
	m_cowFoodModelRender.Draw(rc);
}