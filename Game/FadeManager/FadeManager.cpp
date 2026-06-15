#include "stdafx.h"
#include "FadeManager.h"
namespace
{
	const char * BLACK_FILEPATHA = "Assets/sprite/GameTransition/Black.dds"; 
	const int BLACK_WIDTH = 1920;
	const int BLACK_HEIGHT = 1080;
}
bool FadeManager::Start()
{
	m_BlackSpriteRender.Init(BLACK_FILEPATHA, BLACK_WIDTH, BLACK_HEIGHT);
	m_BlackSpriteRender.SetPosition(Vector3{ 0.0f,0.0f,0.0f });
	m_BlackSpriteRender.Update();
	return true;
}



void FadeManager::Update()
{
	FadeIn();

	FadeOut();
	/** α値を計算*/
	m_BlackSpriteRender.SetMulColor(Vector4{ 1.0f,1.0f,1.0f,m_FadeAlpha });
	/** 更新*/
	m_BlackSpriteRender.Update();
}


void FadeManager::FadeIn()
{
	if (!m_isFadeIn)
	{
		return;
	}
		m_FadeAlpha += m_FadeSpeed * g_gameTime->GetFrameDeltaTime();
		if(m_FadeAlpha >=1.0f)
	    {
			m_FadeAlpha = 1.0f;
			m_isFadeIn = false;
		}
	}


void FadeManager::FadeOut()
{
	if (!m_isFadeOut)
	{
		return;
	}

	m_FadeAlpha -= m_FadeSpeed * g_gameTime->GetFrameDeltaTime();
	if (m_FadeAlpha <= 0.0f)
	{
		m_FadeAlpha = 0.0f;
		m_isFadeOut = false;
	}
}


void FadeManager::Render(RenderContext& rc)
{
	m_BlackSpriteRender.Draw(rc);
}


