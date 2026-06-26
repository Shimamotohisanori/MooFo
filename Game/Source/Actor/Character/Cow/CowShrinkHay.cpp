#include "stdafx.h"
#include "CowShrinkHay.h"

namespace
{
	const char* HAY_FILEPATH = "Assets/modelData/CowFood/hay.tkm";
}

bool CowShrinkHay::Start()
{
    m_hayModelRender.Init(HAY_FILEPATH);
    m_hayModelRender.SetPosition(m_position);
    m_hayModelRender.Update();
    return true;
}

void CowShrinkHay::Update()
{
    m_scale -= g_gameTime->GetFrameDeltaTime() * 3.0f;

    if (m_scale <= 0.0f)
    {
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