#include "stdafx.h"
#include "SoundManager/SoundManager.h"
#include "Title.h"
#include "Game.h"
#include"LoadingScene.h"

namespace
{
	const char* GAMETITLE_FILEPATH = "Assets/sprite/GameTransition/GameTitle.dds";
	const char* PRESSSTART_FILEPATH = "Assets/sprite/GameTransition/PressAnyButton.dds";

	const Vector3 PRESSSTART_POS = { 0.0f, -350.0f, 0.0f };

	const int GAMETITLE_WIDTH = 1920;
	const int GAMETITLE_HEIGHT = 1080;

	constexpr int PRESSSTART_WIDTH = 1400;
	constexpr int PRESSSTART_HEIGHT = 1700;

	/** タイトル画面での点滅の最後の間隔 */
	constexpr float TITLE_FINAL_BLINK_INTERVAL = 1.0f;
}
Title::Title()
{

}
		

Title::~Title()
{

}

bool Title::Start()
{
	m_titleSpriteRender.Init(GAMETITLE_FILEPATH, GAMETITLE_WIDTH, GAMETITLE_HEIGHT);

	m_pressStartSpriteRender.Init(PRESSSTART_FILEPATH, PRESSSTART_WIDTH, PRESSSTART_HEIGHT);
	m_pressStartSpriteRender.SetPosition(PRESSSTART_POS);
	m_pressStartSpriteRender.Update();

	m_soundManager = FindGO<SoundManager>("soundmanager");
	m_titleBGM = m_soundManager->PlayingBGM(SoundBGM::enTitleBGM, false);

	return true;
}


void Title::Update()
{
	InTitle();

	FadeTitle();

	m_titleSpriteRender.Update();
	m_pressStartSpriteRender.Update();
}


void Title::InTitle()
{
	if (g_pad[0]->IsPressAnyKey() && !m_isButtonPressed)
	{
		/* ボタンを押したときの処理 */
		m_isButtonPressed = true;

		m_titleSE = m_soundManager->PlayingSE(SoundSE::enDecisionSE, false);

	}

	if (m_isButtonPressed)
	{
		/* 点滅の間隔を減らしていく */
		m_blinkInterval -= g_gameTime->GetFrameDeltaTime() * m_blinkInterval;

		/* 点滅の間隔が最後の間隔以下になったときに、タイトルからローディングシーンに移行する */
		if (m_blinkInterval <= TITLE_FINAL_BLINK_INTERVAL)
		{
			m_isButtonPressed = false;
			m_loadingScene = NewGO<LoadingScene>(0, "loading");
			m_loadingScene->SetLoadType(LoadingScene::LoadType::ToGameScene);
			DeleteGO(m_titleBGM);
			m_loadingScene->SetNextScene([]()
				{
					NewGO<Game>(0, "game");
				});
			DeleteGO(this);
		}
	}	
}

void Title::FadeTitle()
{
	if (!m_isButtonPressed)
	{
		/** フェード処理*/
		switch (m_titleState)
		{
		case FadeIn:
			m_alpha += g_gameTime->GetFrameDeltaTime();
			if (m_alpha >= 1.0f)
			{
				m_alpha = 1.0f;
				m_titleState = FadeOut;
			}
			break;

		case FadeOut:
			m_alpha -= g_gameTime->GetFrameDeltaTime();
			if (m_alpha <= 0.0f)
			{
				m_alpha = 0.0f;
				m_titleState = FadeIn;
			}
			break;
		}
	}
}

void Title::Render(RenderContext& rc)
{
	m_titleSpriteRender.Draw(rc);

	/* ボタンが押されていないときはスタートを促すスプライトをフェード描画する */
	if (!m_isButtonPressed)
	{
		/** α値が0.0より大きいときに描画する */
		if (m_alpha > 0.0f)
		{
			m_pressStartSpriteRender.SetMulColor(
				Vector4(
					1.0f,
					1.0f,
					1.0f,
					m_alpha));

			m_pressStartSpriteRender.Draw(rc);
		}

		return;
	}

	/** 点滅処理 */
	/* 点滅の間隔が偶数のときは描画する */
	if(static_cast<int>(m_blinkInterval * 2) % 2 == 0)
	{
		m_pressStartSpriteRender.SetMulColor(
			Vector4(
				1.0f,
				1.0f,
				1.0f,
				1.0f));
		m_pressStartSpriteRender.Draw(rc);
	}

}



