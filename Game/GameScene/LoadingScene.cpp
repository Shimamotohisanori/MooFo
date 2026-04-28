#include "stdafx.h"
#include "LoadingScene.h"

namespace
{
	/** ローディングシーンで使用する画像のファイルパス*/
	const char* COWHOOKLOAD_FILEPATH = "Assets/sprite/LoadingUI/CowhookLoad.dds";
	const char* COWRESCUELOAD_FILEPATH = "Assets/sprite/LoadingUI/CowRescueLoad.dds";
	const char* GAMECLEARLOAD_FILEPATH = "Assets/sprite/LoadingUI/GameClearLoad.dds";
	const char* LOADINGTEXT_FILEPATH = "Assets/sprite/LoadingUI/LodingUI.dds";
	const char* BLACKLODING_FILEPATH = "Assets/sprite/GameTransition/Black.dds";
	/** 画像の大きさ*/
	/** 横幅*/
	const int BLACKLOADING_WIDTH = 1920.0f;
	const int LOADING_WIDTH = 1800.0f;
	const int LOADINGWARD_WIDTH = 400.0f;
	
	/** 縦幅*/
	const int BLACKLOADING_HEIGHT = 1080.0f;
	const int LOADING_HEIGHT = 800.0f;
	const int LOADINGWARD_HEIGHT = 100.0f;
	
}
LoadingScene::LoadingScene()
{

}


LoadingScene::~LoadingScene()
{

}

bool LoadingScene::Start()
{
	/** 背景のスプライトの初期化*/
	m_blackLoadingSpriteRender.Init(BLACKLODING_FILEPATH, BLACKLOADING_WIDTH, BLACKLOADING_HEIGHT);
	m_blackLoadingSpriteRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_blackLoadingSpriteRender.Update();
	/** スプライトの初期化*/
	m_loadingSpriteRender[0].Init(COWHOOKLOAD_FILEPATH,LOADING_WIDTH,LOADING_HEIGHT);
	m_loadingSpriteRender[1].Init(COWRESCUELOAD_FILEPATH,LOADING_WIDTH,LOADING_HEIGHT);
	m_loadingSpriteRender[2].Init(GAMECLEARLOAD_FILEPATH,LOADING_WIDTH,LOADING_HEIGHT);
	/** スプライトの位置を設定*/
	for (int i = 0; i < 3; i++)
	{
		m_loadingSpriteRender[i].SetPosition(Vector3(0.0f, 100.0f, 0.0f));
	}
	for (int i = 0; i < 3; i++)
	{
		m_loadingSpriteRender[i].Update();
	}
	/** Loading文字の初期化*/
	m_loadingTextSpriteRender.Init(LOADINGTEXT_FILEPATH, LOADINGWARD_WIDTH, LOADINGWARD_HEIGHT);
	/** Loading文字の位置を設定*/
	m_loadingTextSpriteRender.SetPosition(Vector3(750.0f, -450.0f, 0.0f));
	m_loadingTextSpriteRender.Update();
	/** 最初はLoadingの文字は完全に表示しておく*/
	m_loadingTextAlpha = 1.0f;
	/** 最初はフェードインしていない状態にする*/
	m_isFadeIn = false;
	m_currentImage = 0;
	return true;
}

void LoadingScene::Update()
{
	/** ローディングシーンの更新処理 */
	InLoading();

	/** Loadingの文字のフェード処理*/
	FadeLoadingText();
	/**スプライトの更新 */
	for (int i = 0; i < 3; i++)
	{
		m_loadingSpriteRender[i].Update();
	}

	/** αを反映させる*/
	m_loadingTextSpriteRender.SetMulColor(
		Vector4(1.0f, 1.0f, 1.0f, m_loadingTextAlpha)
	);
	/** Loadingの文字の更新*/
	m_loadingTextSpriteRender.Update();
	
}


void LoadingScene::InLoading()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	m_timer += deltaTime;
	m_totalTime += deltaTime;

	/** 画像を切り替える処理*/
	if (m_timer >= m_changeTime)
	{
		/**画像を切り替えるたびにタイマーを初期化する*/
		m_timer = 0.0f;

		/** 画像を順番に切り替える*/
		m_currentImage = (m_currentImage + 1) % 3;
	}

	/** 一定時間で次のシーンへ移行*/
	if (m_totalTime >= m_loadingTime)
	{
		if (m_nextSceneLoading)
		{
			/** 次のシーンをロードする関数がセットされているなら呼び出す*/
			m_nextSceneLoading();
		}
		/** 現在のシーンを削除*/
		DeleteGO(this);
	}
}

void LoadingScene::FadeLoadingText()
{
	if (m_isFadeIn)
	{
		/** フェードイン(0 →1)*/
		m_loadingTextAlpha += m_loadingFadeSpeed * g_gameTime->GetFrameDeltaTime();
		if (m_loadingTextAlpha >= 1.0f)
		{
			m_loadingTextAlpha = 1.0f;
			m_isFadeIn = false;
		}
	}
	else
	{
		/** フェードアウト(1 →0)*/
		m_loadingTextAlpha -= m_loadingFadeSpeed * g_gameTime->GetFrameDeltaTime();
		if (m_loadingTextAlpha <= 0.0f)
		{
			m_loadingTextAlpha = 0.0f;
			/** 次はフェードイン*/
			m_isFadeIn = true;
		}
	}
}
void LoadingScene::SetNextScene(std::function<void()>next)
{
	m_nextSceneLoading = next;
}
void LoadingScene::Render(RenderContext& rc)
{
	m_blackLoadingSpriteRender.Draw(rc);
	m_loadingSpriteRender[m_currentImage].Draw(rc);
	m_loadingTextSpriteRender.Draw(rc);
}