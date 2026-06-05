#include "stdafx.h"
#include "CowFood.h"
#include "EffectManager/EffectManager.h"
#include "Source/Actor/Character/Player/Player.h"
namespace
{
	/** 牛の餌のモデルファイルパス */
	const char* COWFOOD_FILE_PATH = "Assets/modelData/Stage/CowFood.tkm";

	const char* COWFOOD_UI = "Assets/sprite/CowFoodUI/test.dds";

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

	m_Abutton.Init(COWFOOD_UI,1280.0f,1080.0f);

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
	Player* player = FindGO<Player>("player");

	if (!m_cowFoodEffect->IsPlay())
	{
		m_cowFoodEffect->Play();
	}

	if (player == nullptr)
	{
		return;
	}

	m_position = COWFOOD_POS;
	m_cowFoodModelRender.SetPosition(m_position);

	Vector3 dir = player->GetPosition() - m_position;
	dir.y = 0.0f;

	float distance = dir.Length();

	if (distance <= 100.0f)
	{
		m_iseffect = true;
		m_Abutton.Update();
	}
	else
	{
		m_iseffect = false;
	}
	
}

void CowFood::Render(RenderContext& rc)
{
	/** 牛の餌のモデルを描画する。 */
	m_cowFoodModelRender.Draw(rc);

	if (m_iseffect)
	{
		m_Abutton.Draw(rc);
	}
	
}