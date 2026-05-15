#include "stdafx.h"
#include "LoadingScene.h"
#include "SoundManager/SoundManager.h"
#include "GameScene/Game.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Stage/Stage.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "Source/Actor/Character/UFO/UFO.h"
#include "GameCamera/GameCamera.h"
#include "nature/SkyCube.h"
#include "EffectManager/EffectManager.h"

namespace
{
	/** ローディングシーンで使用する画像のファイルパス */
	const char* COWHOOKLOAD_FILEPATH = "Assets/sprite/LoadingUI/CowhookLoad.dds";
	const char* COWRESCUELOAD_FILEPATH = "Assets/sprite/LoadingUI/CowRescueLoad.dds";
	const char* GAMECLEARLOAD_FILEPATH = "Assets/sprite/LoadingUI/GameClearLoad.dds";
	const char* LOADINGTEXT_FILEPATH = "Assets/sprite/LoadingUI/LodingUI.dds";
	const char* BLACKLODING_FILEPATH = "Assets/sprite/GameTransition/Black.dds";

	/** 画像の大きさ */
	/** 横幅 */
	const int BLACKLOADING_WIDTH = 1920.0f;
	const int LOADING_WIDTH = 1800.0f;
	const int LOADINGWARD_WIDTH = 400.0f;
	
	/** 縦幅 */
	const int BLACKLOADING_HEIGHT = 1080.0f;
	const int LOADING_HEIGHT = 800.0f;
	const int LOADINGWARD_HEIGHT = 100.0f;
	
	/** 牛のランダムスポーン範囲 */
	const int RANDOM_SPAWN_RANGE = 500;
	const int RANDOM_SPAWN_RANGE_DOUBLE = 800;
}

LoadingScene::LoadingScene()
{

}


LoadingScene::~LoadingScene()
{
	/** ローディング中の音源を削除する */
	DeleteGO(m_loadingSound);
}

bool LoadingScene::Start()
{
	/** 背景のスプライトの初期化 */
	m_blackLoadingSpriteRender.Init(BLACKLODING_FILEPATH, BLACKLOADING_WIDTH, BLACKLOADING_HEIGHT);
	m_blackLoadingSpriteRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_blackLoadingSpriteRender.Update();
	
	/** スプライトの初期化 */
	m_loadingSpriteRender[0].Init(COWHOOKLOAD_FILEPATH,LOADING_WIDTH,LOADING_HEIGHT);
	m_loadingSpriteRender[1].Init(COWRESCUELOAD_FILEPATH,LOADING_WIDTH,LOADING_HEIGHT);
	m_loadingSpriteRender[2].Init(GAMECLEARLOAD_FILEPATH,LOADING_WIDTH,LOADING_HEIGHT);
	
	/** スプライトの位置を設定 */
	for (int i = 0; i < 3; i++)
	{
		m_loadingSpriteRender[i].SetPosition(Vector3(0.0f, 100.0f, 0.0f));
	}
	
	/** スプライトの更新 */
	for (int i = 0; i < 3; i++)
	{
		m_loadingSpriteRender[i].Update();
	}
	
	/** Loading文字の初期化 */
	m_loadingTextSpriteRender.Init(LOADINGTEXT_FILEPATH, LOADINGWARD_WIDTH, LOADINGWARD_HEIGHT);
	
	/** Loading文字の位置を設定 */
	m_loadingTextSpriteRender.SetPosition(Vector3(750.0f, -450.0f, 0.0f));
	
	/** 最初はLoadingの文字は完全に表示しておく */
	m_loadingTextAlpha = 1.0f;
	
	/** 最初はフェードインしていない状態にする */
	m_isFadeIn = false;

	/** 最初は最初の画像を表示する */
	m_currentImage = 0;
	
	SoundManager* soundManager = FindGO<SoundManager>("soundmanager");

	/** ローディング中の音源を再生する */
	m_loadingSound = soundManager->PlayingBGM(SoundBGM::enGameLoadingBGM, true);

	return true;
}

void LoadingScene::Update()
{
	/** ローディングシーンの更新処理 */
	InLoading();

	/** Loadingの文字のフェード処理 */
	FadeLoadingText();

	/**スプライトの更新 */
	for (int i = 0; i < 3; i++)
	{
		m_loadingSpriteRender[i].Update();
	}

	/** αを反映させる */
	m_loadingTextSpriteRender.SetMulColor(
		Vector4(1.0f, 1.0f, 1.0f, m_loadingTextAlpha)
	);

	/** Loadingの文字の更新 */
	m_loadingTextSpriteRender.Update();
	
}


void LoadingScene::InLoading()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	m_timer += deltaTime;
	m_totalTime += deltaTime;

	/** 画像を切り替える処理 */
	if (m_timer >= m_changeTime)
	{
		/** 画像を切り替えるたびにタイマーを初期化する */
		m_timer = 0.0f;

		/** 画像を順番に切り替える */
		m_currentImage = (m_currentImage + 1) % 3;
	}

	/** 一定時間で次のシーンへ移行 */
	if (m_totalTime >= m_loadingTime)
	{
		if (m_loadType == LoadType::ToGameScene)
		{
			/** ゲームシーンに移行するタイプのロード処理 */
			LoadGameObjectsStepByStep(); 
		}

		else
		{
			/** タイトルシーンに移行するタイプのロード処理 */
			LoadTitleOnly();
		}
	}
}

void LoadingScene::FadeLoadingText()
{
	if (m_isFadeIn)
	{
		/** フェードイン(0 →1) */
		m_loadingTextAlpha += m_loadingFadeSpeed * g_gameTime->GetFrameDeltaTime();
		if (m_loadingTextAlpha >= 1.0f)
		{
			m_loadingTextAlpha = 1.0f;
			m_isFadeIn = false;
		}
	}
	else
	{
		/** フェードアウト(1 →0) */
		m_loadingTextAlpha -= m_loadingFadeSpeed * g_gameTime->GetFrameDeltaTime();
		if (m_loadingTextAlpha <= 0.0f)
		{
			m_loadingTextAlpha = 0.0f;
			/** 次はフェードイン */
			m_isFadeIn = true;
		}
	}
}

void LoadingScene::LoadGameObjectsStepByStep()
{
	switch (m_loadStep)
	{
		/** ロードするゲームオブジェクトをステップバイステップで生成する */

		/** プレイヤーを生成 */
	case 0: NewGO<Player>(0, "player"); break;

		/** ステージを生成 */
	case 1: NewGO<Stage>(0, "stage"); break;

		/** 牛を生成(10体分) */
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
		{
		Cow* cow = NewGO<Cow>(0, "cow");
		cow->SetPosition(RandomCowPos());
		m_tempCows.push_back(cow);
	} break;
	
		/** UFOを生成(4体分) */

	case 13:
	case 14:
	case 15:
	case 16:
	{
		UFO* ufo = NewGO<UFO>(0, UFO_INFOMATIONS[m_loadStep - 13].objectName.c_str());
		ufo->SetPosition(UFO_INFOMATIONS[m_loadStep - 13].pos);
		} break;

		/** ゲームカメラを生成 */
	case 17: NewGO<GameCamera>(0, "gameCamera"); break;

		/** エフェクトマネージャーを生成 */
	case 18: NewGO<EffectManager>(0, "effectManager"); break;

		/** スカイキューブを生成 */
	case 19:
	{
		
		/** SkyCube を生成 */
		SkyCube* sky = NewGO<SkyCube>(0, "skyCube");

		/** タイプ設定 */
		sky->SetType(EnSkyCubeType::enSkyCubeType_Night);

		/** スケール設定 */
		sky->SetScale(10000.0f);

		/** IBL 設定 */
		g_renderingEngine->SetAmbientByIBLTexture(sky->GetTextureFilePath(), 0.8f);
	} break;

		/** ゲーム本体を生成 */
	case 20:
		Game* game =NewGO<Game>(0, "game");

		/** ロードした牛をゲームに渡す */
		for (auto cow : m_tempCows)
		{
			game->GetAliveCows().push_back(cow);
		}

		DeleteGO(this);
		return;
	}

	m_loadStep++;
}

void LoadingScene::LoadTitleOnly()
{
	m_nextSceneLoading();
	DeleteGO(this);
}

Vector3 LoadingScene::RandomCowPos()
{
	Vector3 pos;
	pos.x = (rand() % RANDOM_SPAWN_RANGE_DOUBLE) - RANDOM_SPAWN_RANGE;
	pos.y = 0.0f;
	pos.z = (rand() % RANDOM_SPAWN_RANGE_DOUBLE) - RANDOM_SPAWN_RANGE;
	return pos;
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