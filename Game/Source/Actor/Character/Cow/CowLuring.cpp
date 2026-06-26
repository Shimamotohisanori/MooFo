#include "stdafx.h"
#include "CowLuring.h"

namespace
{
	/** 干し草のモデルファイルパス */
	const char* COW_FOOD_FILEPATH = "Assets/modelData/CowFood/hay.tkm";
}

bool CowLuring::Start()
{
	/** 干し草のモデルを初期化する */
	m_hayModelRender.Init(COW_FOOD_FILEPATH);

	/** 干し草の初期座標を設定する */
	m_hayModelRender.SetPosition(m_position);
	return true;
}

void CowLuring::Update()
{
    /** スケールアニメーション中なら */
    if (m_isScaling)
    {
        /** スケールを徐々に1に近づける */
        m_scale += g_gameTime->GetFrameDeltaTime() * 2.0f;

        /** 1を超えたらアニメーション終了 */
        if (m_scale >= 1.0f)
        {
            m_scale = 1.0f;
            m_isScaling = false;
        }
    }

    else if (m_isShrinking)
    {
        m_scale -= g_gameTime->GetFrameDeltaTime() * 3.0f;
        if (m_scale <= 0.0f)
        {
            m_scale = 0.0f;
            m_isShrinking = false;
        }
    }

    /** モデルにスケールを反映する */
    m_hayModelRender.SetScale({ m_scale, m_scale, m_scale });
    m_hayModelRender.Update();
}

void CowLuring::PutHay()
{

}

void CowLuring::SetPosition(const Vector3& pos)
{
	/** 座標をメンバ変数に保存する */
	m_position = pos;

	/** モデルの座標を更新する */
	m_hayModelRender.SetPosition(pos);
}

void CowLuring::Render(RenderContext& rc)
{

	m_hayModelRender.Draw(rc);
}
