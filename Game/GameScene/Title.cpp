#include "stdafx.h"
#include "SoundManager/SoundManager.h"
#include "Title.h"
#include "Game.h"
#include"LoadingScene.h"

namespace
{
	/** タイトルの画像のファイルパス */
	const char* GAMETITLE_FILEPATH = "Assets/sprite/GameTransition/GameTitle.dds";

	/** タイトルを促す画像のファイルパス */
	const char* TITLE_PRESSSTART_FILEPATH = "Assets/sprite/GameTransition/PressAnyButton.dds";

	/** タイトルを促す画像の座標 */
	const Vector3 TITLE_PRESSSTART_POS = { 0.0f, -350.0f, 0.0f };

	/** タイトルの画像の大きさ(横幅、高さ) */
	const int GAMETITLE_WIDTH = 1920;
	const int GAMETITLE_HEIGHT = 1080;

	/** タイトルを促す画像の大きさ(横幅、高さ) */
	constexpr int TITLE_PRESSSTART_WIDTH = 1400;
	constexpr int TITLE_PRESSSTART_HEIGHT = 1700;

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

	m_titlePressStartSpriteRender.Init(TITLE_PRESSSTART_FILEPATH, TITLE_PRESSSTART_WIDTH, TITLE_PRESSSTART_HEIGHT);
	m_titlePressStartSpriteRender.SetPosition(TITLE_PRESSSTART_POS);
	m_titlePressStartSpriteRender.Update();

	m_soundManager = FindGO<SoundManager>("soundmanager");
	m_titleBGM = m_soundManager->PlayingBGM(SoundBGM::enTitleBGM, false);

	return true;
}


void Title::Update()
{
	/** タイトルでの処理 */
	InTitle();

	/** タイトルのフェード処理 */
	FadeTitle();

	m_titleSpriteRender.Update();
	m_titlePressStartSpriteRender.Update();
}


void Title::InTitle()
{
	if (g_pad[0]->IsPressAnyKey() && !m_isStartButtonPressed)
	{
		/** ボタンを押したときの処理 */
		m_isStartButtonPressed = true;

		m_titleSE = m_soundManager->PlayingSE(SoundSE::enDecisionSE, false);

	}

	if (m_isStartButtonPressed)
	{
		m_titleFinalBlinkTime -= g_gameTime->GetFrameDeltaTime();

		/** 点滅の間隔を減らしていく */
		m_titleBlinkInterval -= g_gameTime->GetFrameDeltaTime() * m_titleBlinkInterval;

		/** 点滅の間隔が最後の間隔以下になったときに、タイトルからローディングシーンに移行する */
		if (m_titleFinalBlinkTime <= 0.0f)
		{
			m_isStartButtonPressed = false;
			m_loadingScene = NewGO<LoadingScene>(0, "loading");
			m_loadingScene->SetLoadType(LoadingScene::LoadType::ToGameScene);
			DeleteGO(m_titleBGM);
			DeleteGO(m_titleSE);
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
	if (!m_isStartButtonPressed)
	{
		/** フェード処理*/
		switch (m_titleState)
		{
		case FadeIn:
			m_titleAlpha += g_gameTime->GetFrameDeltaTime();
			if (m_titleAlpha >= 1.0f)
			{
				m_titleAlpha = 1.0f;
				m_titleState = FadeOut;
			}
			break;

		case FadeOut:
			m_titleAlpha -= g_gameTime->GetFrameDeltaTime();
			if (m_titleAlpha <= 0.0f)
			{
				m_titleAlpha = 0.0f;
				m_titleState = FadeIn;
			}
			break;
		}
	}
}

void Title::Render(RenderContext& rc)
{
	m_titleSpriteRender.Draw(rc);

	/** ボタンが押されていないときはスタートを促すスプライトをフェード描画する */
	if (!m_isStartButtonPressed)
	{
		/** α値が0.0より大きいときに描画する */
		if (m_titleAlpha > 0.0f)
		{
			m_titlePressStartSpriteRender.SetMulColor(
				Vector4(
					1.0f,
					1.0f,
					1.0f,
					m_titleAlpha));

			m_titlePressStartSpriteRender.Draw(rc);
		}

		return;
	}

	/** 点滅処理 */
	/* 点滅の間隔が偶数のときは描画する */
	if(static_cast<int>(m_titleBlinkInterval * 2) % 2 == 0)
	{
		m_titlePressStartSpriteRender.SetMulColor(
			Vector4(
				1.0f,
				1.0f,
				1.0f,
				1.0f));
		m_titlePressStartSpriteRender.Draw(rc);
	}

}



