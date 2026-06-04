#include "stdafx.h"
#include "CowFood.h"
#include "EffectManager/EffectManager.h"
namespace
{
	/** 牛の餌のモデルファイルパス */
	const char* COWFOOD_FILE_PATH = "Assets/modelData/Stage/CowFood.tkm";

	/** 牛の餌の座標 */
	Vector3 COWFOOD_POS = { -1260.0f,-5.0f,-350.0f };

	Vector3 COWFOOD_SCALE = { 40.0f,40.0f,40.0f };

}

bool CowFood::Start()
{
	/** 牛の餌のモデル */
	m_cowFoodModelRender.Init(COWFOOD_FILE_PATH);
	m_cowFoodModelRender.SetPosition(COWFOOD_POS);
	m_cowFoodModelRender.Update();

	/** 牛の餌の当たり判定をつける */
	m_FoodObject.CreateFromModel(m_cowFoodModelRender.GetModel(), m_cowFoodModelRender.GetModel().GetWorldMatrix());

	m_cowFoodEffect = NewGO<nsK2EngineLow::EffectEmitter>(0);

	m_cowFoodEffect->Init((int)EffectID::EffectID_CowFoodEffect);

	Vector3 cowFoodPos = { COWFOOD_POS.x, COWFOOD_POS.y + 30.0f, COWFOOD_POS.z + 10.0f };

	m_cowFoodEffect->SetPosition(cowFoodPos);
	m_cowFoodEffect->SetScale(COWFOOD_SCALE);
	

	return true;
}

void CowFood::Update()
{
	if (!m_cowFoodEffect->IsPlay())
	{
		m_cowFoodEffect->Play();
	}
	
}

void CowFood::Render(RenderContext& rc)
{
	/** 牛の餌のモデルを描画する。 */
	m_cowFoodModelRender.Draw(rc);
}