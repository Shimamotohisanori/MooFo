#include "stdafx.h"
#include "CowShrinkHay.h"
#include "EffectManager/EffectManager.h"

namespace
{
	const char* HAY_FILEPATH = "Assets/modelData/CowFood/hay.tkm";

    const Vector3 EFFECT_SCALE = { 5.0f,5.0f,5.0f };
}

bool CowShrinkHay::Start()
{
    m_hayModelRender.Init(HAY_FILEPATH);
    m_hayModelRender.SetPosition(m_position);
    m_hayModelRender.Update();

    m_afterMealEffect = NewGO<nsK2EngineLow::EffectEmitter>(0);
    m_afterMealEffect->Init((int)EffectID::EffectID_AfterMeal);
    m_afterMealEffect->SetPosition(m_position);
    m_afterMealEffect->SetScale(EFFECT_SCALE);
    m_afterMealEffect->Play();
    m_afterMealEffect->Update();

    return true;
}

void CowShrinkHay::Update()
{

    m_scale -= g_gameTime->GetFrameDeltaTime() * 3.0f;

    if (m_scale <= 0.0f)
    {
        m_scale = 0.0f;
        /** エフェクトが再生中ならエフェクトの終了を待つ */
        if (m_afterMealEffect && m_afterMealEffect->IsPlay())
        {
            m_hayModelRender.SetScale({ 0.0f, 0.0f, 0.0f });
            m_hayModelRender.Update();
            return;
        }
        DeleteGO(this);
        return;
    }

    m_hayModelRender.SetScale({ m_scale, m_scale, m_scale });
    m_hayModelRender.SetPosition(m_position);
    m_hayModelRender.Update();
}

void CowShrinkHay::Render(RenderContext& rc)
{
    m_hayModelRender.Draw(rc);
}