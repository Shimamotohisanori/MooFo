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
#include "Source/Actor/Character/UFO/UFOLightManager.h"
#include "Source/Actor/Stage/CowFood.h"
#include <chrono>  
#include "Source/Actor/Stage/CowFoodManager.h"
#include"Title.h"
namespace
{
	/** ローディングシーンで使用する画像のファイルパス */
	const char* COWHOOKLOAD_FILEPATH =   "Assets/sprite/LoadingUI/NonePictureCowhookLoad.dds";
	const char* COWFOODLOAD_FILEPATH =   "Assets/sprite/LoadingUI/NonePictureFoodLoading.dds";
	const char* LOADINGTEXT_FILEPATH =   "Assets/sprite/LoadingUI/LodingUI.dds";
	const char* BLACKLODING_FILEPATH =   "Assets/sprite/GameTransition/Black.dds";

	/** 画像の大きさ */
	/** 横幅 */
	constexpr int BLACKLOADING_WIDTH = 1920.0f;
	constexpr int LOADING_WIDTH = 1900.0f;
	constexpr int COWHOOK_LOADING_WIDTH = 1800.0f;
	constexpr int COWRESCUE_LOADING_WIDTH = 1700.0f;
	constexpr int LOADINGWARD_WIDTH = 400.0f;
	
	/** 縦幅 */
	constexpr int BLACKLOADING_HEIGHT = 1080.0f;
	constexpr int LOADING_HEIGHT = 850.0f;
	constexpr int COWRESCUE_LOADING_HEIGHT = 750.0f;
	constexpr int LOADINGWARD_HEIGHT = 100.0f;
	
	/** 牛のランダムスポーン範囲 */
	constexpr int RANDOM_SPAWN_RANGE = 500;
	constexpr int RANDOM_SPAWN_RANGE_DOUBLE = 800;

	/** 牛同士の最低距離 */
	constexpr float MIN_DISTANCE = 15.0f; // 牛同士の最低距離

	/** ゲーム開始直後(初期10体)に追いかける牛の出現確率 */
	constexpr int INITIAL_CHASE_COW_RATE = 20;

	/** Gif終了とロード完了が揃ってから、実際にフェードアウトを始めるまでの
	止め絵を見せる時間(秒) */
	//constexpr float FINISHED_HOLD_DURATION = 2.0f;
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
	/** 最初は完全に表示しておく*/
	m_SceneFadeAlpha = 1.0f;
	/** 最初はフェードインしていない状態にしておく*/
	m_isSceneFadeOut= false;


	
	/** 最初はLoadingの文字は完全に表示しておく */
	m_loadingTextAlpha = 1.0f;
	
	/** 最初はフェードインしていない状態にする */
	m_isFadeIn = false;

	/** 最初は最初の画像を表示する */
	m_currentImage = 0;

	/** 最初に表示される画像は見たことにする*/
	m_hasSeenImage0 = true;
	m_hasSeenImage1 = false;
	m_isReadyToStart = false;

	/** Gif画像のパスリストとフレーム数を準備する */
	PrepareAnimResources();

	/** サウンドマネージャーを検索する */
	SoundManager* soundManager = FindGO<SoundManager>("soundmanager");

	/** ローディング中の音源を再生する */
	m_loadingSound = soundManager->PlayingBGM(SoundBGM::enGameLoadingBGM, true);

	return true;
}

void LoadingScene::Update()
{
	/** 初期スプライト（ロード画像・テキスト）のロードが終わるまではここだけ進める */
	if (m_initLoadStep != InitLoadStep::Num)
	{
		LoadInitialSpritesStepByStep();

		/** αを黒背景だけ反映して抜ける（他のスプライトはまだ実体がない） */
		m_blackLoadingSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_SceneFadeAlpha));
		m_blackLoadingSpriteRender.Update();
		return;
	}

	/** ローディングシーンの更新処理 */
	InLoading();

	/** Loadingの文字のフェード処理 */
	FadeLoadingText();

	/** Sceneのフェード処理*/
	FadeOutLoadingScene();

	/** Gif画像が再生され終わったら自動的に画像を進める処理*/
	AutoAdvanceImage();
	/** アニメーションのフレームを先に進めておく(遅延をなくすため)*/
	if (m_ropeLoadIndex > 0)
	{
		m_ropeAnimSpriteRender.Update();
	}
	/**最終フレームに到達したらUpdateを止めて静止させる*/
	if (!m_hasFoodAnimReachedEnd)
	{
		m_foodAnimSpriteRender.Update();

		if (m_foodAnimSpriteRender.IsFinished())
		{
			m_hasFoodAnimReachedEnd = true;
		}
	}
	
	/** 餌のアニメーションをバックグラウンドでロードする */
	LoadFoodAnimInBackground();
	
	/**スプライトの更新 */
	for (int i = 0; i < 2; i++)
	{
		m_loadingSpriteRender[i].Update();
	}

	/** αを反映させる */
	m_loadingTextSpriteRender.SetMulColor(
		Vector4(1.0f, 1.0f, 1.0f, m_loadingTextAlpha)
	);
	/** αを反映させる*/
	m_blackLoadingSpriteRender.SetMulColor(
		Vector4(1.0f, 1.0f, 1.0f, m_SceneFadeAlpha)
	);
	/** Loadingの文字の更新 */
	m_loadingTextSpriteRender.Update();

	/** Sceneの更新*/
	m_blackLoadingSpriteRender.Update();

	
	
}


void LoadingScene::InLoading()
{
	if (m_isSceneFadeOut)
	{
		return;
	}

	/** ロードが完了していたら、ロード処理を回さない*/
	if (m_isLoadingEnd)
	{
		return;
	}

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

void LoadingScene::AutoAdvanceImage()
{
	/** 既に開始確定していたら何もしない */
	if (m_isSceneFadeOut || m_isReadyToStart)
	{
		return;
	}

	if (m_currentImage == 0)
	{
		/** １枚目の画像表示中はGifが終わったら２枚目に自動遷移させる*/
		if (m_ropeAnimSpriteRender.IsFinished()
			&& !m_hasAutoAdvancedImage0
			&& m_isFoodLoaded)
		{
			m_hasAutoAdvancedImage0 = true;
			m_currentImage = 1;
			m_hasSeenImage1 = true;

			/** 2枚目の画像を最初から再生させる*/
			m_foodAnimSpriteRender.Reset();
			/** リセットした状態を反映させる*/
			m_foodAnimSpriteRender.Update();
		}
	}

	else if (m_currentImage == 1)
	{
		/** ロード完了まで待機*/
		if (!m_isLoadingEnd)
		{
			return;
		}

		/** ロードが終わっていても、Gifがまだ最後まで再生し終わっていない場合は
			区切りが良くなるまで待つ（再生の途中でぶつ切りにしないため）*/
		if (!m_foodAnimSpriteRender.IsFinished())
		{
			return;
		}

		/** ここに来た時点で「ロード完了」かつ「Gifが最後のフレームで静止」している。
			そのまま即座にフェードアウトを開始する*/
		if (m_hasSeenImage0 && m_hasSeenImage1)
		{
			m_isReadyToStart = true;
			m_isSceneFadeOut = true;
			m_hasFinishedImage2 = true;

			/** ゲーム開始時のみBGMを有効にする*/
			if (m_loadType == LoadType::ToGameScene)
			{
				Game* game = FindGO<Game>("game");
				if (game)
				{
					game->ActivateGameBGM();
				}
			}
			else if (m_loadType == LoadType::ToTitleScene)
			{
				/** フェード開始と同時にTitleを生成しておく。
					これによりフェード中、黒背景の裏で既にTitleが描画され始める */
				if (!m_nextSceneCreated && m_nextSceneLoading)
				{
					m_nextSceneLoading();
					m_nextSceneCreated = true;
				}
			}
		}
	}
}


void LoadingScene::PrepareAnimResources()
{
	/** 既に準備されている場合は何もしない */
	if (!m_foodAnimLoadPaths.empty())
	{
		return;
	}

	/** ロープで引っ張るGifのパスリストを準備する */
	for (int h = 10; h <= 135; h += 2)
	{
		char buf[256];
		sprintf(buf, "Assets/Gif/PullRope/anim_%02d.DDS", h);
		m_ropeAnimLoadPaths.push_back(buf);
	}
	/** 餌のGifのパスリストを準備する */
	for (int i = 1; i <= 200; i += 2)
	{
		char buf[256];
		sprintf(buf, "Assets/Gif/Food/anim_%02d.DDS", i);
		m_foodAnimLoadPaths.push_back(buf);
	}

	/** ロープで引っ張るGifのフレーム数を準備する */
	m_ropeAnimSpriteRender.PrepareFrameCount(
		static_cast<int>(m_ropeAnimLoadPaths.size()), 610.0f, 590.0f, 10.0f);
	m_ropeAnimSpriteRender.SetPosition({ -535.0f, 10.0f, 0.0f });

	/** 餌のGifのフレーム数を準備する */
	m_foodAnimSpriteRender.PrepareFrameCount(
		static_cast<int>(m_foodAnimLoadPaths.size()), 610.0f, 590.0f, 10.0f);
	m_foodAnimSpriteRender.SetPosition({ -535.0f, 10.0f, 0.0f });
}


void LoadingScene::LoadFoodAnimInBackground()
{
	if (m_isFoodLoaded)
	{
		return;
	}

	/** 1フレームでロードに使ってよい時間(ms) */
	constexpr double FOOD_LOAD_TIME_BUDGET_MS = 1.5;

	auto loadStart = std::chrono::high_resolution_clock::now();

	while (m_foodLoadIndex < static_cast<int>(m_foodAnimLoadPaths.size()))
	{
		m_foodAnimSpriteRender.AddFrame(m_foodAnimLoadPaths[m_foodLoadIndex]);
		m_foodLoadIndex++;

		double elapsedMs = std::chrono::duration<double, std::milli>(
			std::chrono::high_resolution_clock::now() - loadStart).count();

		if (elapsedMs >= FOOD_LOAD_TIME_BUDGET_MS)
		{
			break;
		}
	}

	/** 全部ロードし終わったらフラグを立てる（これが無いとm_isFoodLoadedが一生trueにならない） */
	if (m_foodLoadIndex >= static_cast<int>(m_foodAnimLoadPaths.size()))
	{
		m_isFoodLoaded = true;
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


void LoadingScene::LoadInitialSpritesStepByStep()
{
	switch (m_initLoadStep)
	{
	case InitLoadStep::LoadingImage0:
		m_loadingSpriteRender[0].Init(COWHOOKLOAD_FILEPATH, LOADING_WIDTH, LOADING_HEIGHT);
		m_loadingSpriteRender[0].SetPosition(Vector3(0.0f, 100.0f, 0.0f));
		m_loadingSpriteRender[0].Update();
		m_initLoadStep = InitLoadStep::LoadingImage1;
		break;

	case InitLoadStep::LoadingImage1:
		m_loadingSpriteRender[1].Init(COWFOODLOAD_FILEPATH, LOADING_WIDTH, LOADING_HEIGHT);
		m_loadingSpriteRender[1].SetPosition(Vector3(0.0f, 100.0f, 0.0f));
		m_loadingSpriteRender[1].Update();
		m_initLoadStep = InitLoadStep::LoadingText;
		break;

	case InitLoadStep::LoadingText:
		m_loadingTextSpriteRender.Init(LOADINGTEXT_FILEPATH, LOADINGWARD_WIDTH, LOADINGWARD_HEIGHT);
		m_loadingTextSpriteRender.SetPosition(Vector3(750.0f, -450.0f, 0.0f));
		m_initLoadStep = InitLoadStep::Num;
		break;

	case InitLoadStep::Num:
		break;
	}
}

void LoadingScene::FadeOutLoadingScene()
{
	if (!m_isSceneFadeOut)
	{
		return;
	}

	/** アルファがまだ下がりきっていなければフェード処理を進める */
	if (m_SceneFadeAlpha > 0.0f)
	{
		m_SceneFadeAlpha -= m_SceneFadeSpeed * g_gameTime->GetFrameDeltaTime();
		if (m_SceneFadeAlpha < 0.0f)
		{
			m_SceneFadeAlpha = 0.0f;
		}
		return;
	}

	/** ここに来た時点でアルファは0（完全に透明＝黒背景が消えた状態）*/
	m_isFadeComplete = true;

	if (m_loadType == LoadType::ToTitleScene)
	{
		/** Titleはもうフェード開始時に生成済み。念のため初期化完了を確認するだけ */
		Title* title = FindGO<Title>("title");
		if (!title || !title->IsReady())
		{
			return;
		}
	}

	m_isSceneFadeOut = false;
	DeleteGO(this);
	
}


void LoadingScene::LoadGameObjectsStepByStep()
{
	switch (m_loadStep)
	{
		/** ロードするゲームオブジェクトをステップバイステップで生成する */


	case 0:
	{


		/** 1フレームでロードに使ってよい時間(ms) */
		constexpr double ROPE_LOAD_TIME_BUDGET_MS = 2.0;

		/** ロープを最優先でロードし切るまではこのステップに留まる*/
		if (m_ropeLoadIndex < static_cast<int>(m_ropeAnimLoadPaths.size()))
		{
			auto loadStart = std::chrono::high_resolution_clock::now();

			/** 予算内に収まる限り、1枚ずつロードを続ける */
			while (m_ropeLoadIndex < static_cast<int>(m_ropeAnimLoadPaths.size()))
			{
				m_ropeAnimSpriteRender.AddFrame(m_ropeAnimLoadPaths[m_ropeLoadIndex]);
				m_ropeLoadIndex++;

				double elapsedMs = std::chrono::duration<double, std::milli>(
					std::chrono::high_resolution_clock::now() - loadStart).count();

				if (elapsedMs >= ROPE_LOAD_TIME_BUDGET_MS)
				{
					break;
				}
			}
			return; // ロープのロードが終わるまで次のステップに進まない
		}

		// ロープのロードが完了した時点でこのcaseを抜ける
		break;
	}
	/** プレイヤーを生成 */
	case 1:NewGO<Player>(0, "player");
		break;

	case 2:/** ステージを生成 */
	{
		if (m_stage == nullptr)
		{
			m_stage = NewGO<Stage>(0, "stage");
		}
		if (!m_stage->LoadStepByStep())
		{
			/** ステージのロードが終わるまで次のステップに進まない*/
			return;
		}
		break;
	}
	/** 牛を生成(10体分) */
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
		/** 0～9の数字を割り当てる*/
		int cowIndex = m_loadStep - 2;

		/** 牛の性格をランダムに割り当てる */
		if (rand() % 100 < INITIAL_CHASE_COW_RATE)
		{
			cow->SetCowType(Cow::EnCowType::en_Chase);
		}
		else
		{
			cow->SetCowType(cowIndex % 2 == 0 ? Cow::EnCowType::en_Light : Cow::EnCowType::en_Random);
		}

		m_tempCows.push_back(cow);
	} break;

	/** もしUFOが消えていなかったら残っているUFOを消す */
	case 13:
	{
		auto ufos = FindGOs<UFO>("UFO");
		for (auto ufo : ufos)
		{
			if (ufo && !ufo->IsDead())
			{
				DeleteGO(ufo);
			}
		}
		break;
	}

	/** UFOLightManagerを生成 */
	case 14:
		NewGO<UFOLightManager>(0, "ufolightmanager");
		break;

		/** UFOを生成(4体分) */
	case 15:
	case 16:
	case 17:
	case 18:
	{

		int index = m_loadStep - 15;
		if (index >= 0 && index < 4)
		{
			UFO* ufo = NewGO<UFO>(0, "UFO");
			ufo->SetPosition(UFO_INFOMATIONS[index].pos);
			ufo->SetUFOMoveState();
			ufo->SetSlotIndex(index);
			m_tempUFOs.push_back(ufo);
		}
	} break;

	/** ゲームカメラを生成 */

	case 19: NewGO<GameCamera>(0, "gameCamera");
		break;
	case 20:
	{
		/** 牛の餌を生成 */
		NewGO<CowFood>(0, "cowfood");

		NewGO<CowFoodManager>(0, "cowfoodmanager");
	}
	break;
	/** スカイキューブを生成 */
	case 21:
	{
		/** SkyCube を生成 */
		m_skyCube = NewGO<SkyCube>(0, "skyCube");

		/** タイプ設定 */
		m_skyCube->SetType(EnSkyCubeType::enSkyCubeType_Day);

		/** スケール設定 */
		m_skyCube->SetScale(10000.0f);
	}
	break;

	/** 方向光・IBL設定（ここが一番重いはず） */
	case 22:
	{
		/** 方向光(ほぼ真上から差し込む光) */
		Vector3 sunDir(0.0f, -1.0f, 0.0f);
		sunDir.Normalize();
		g_renderingEngine->SetDirectionLight(0, sunDir, Vector3(5.0f, 5.0f, 5.0f));

		/** IBL 設定 */
		g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.95f);
	}
	break;
	case 23:
		/** ブルームを抑制 */
		g_renderingEngine->SetBloomThreshold(3.0f);
		break;

	case 24:
	{
		if (m_game == nullptr)
		{
			m_game = NewGO<Game>(0, "game");
		}
		if (!m_game->LoadStepByStep())
		{
			/** Gameの初期化が終わるまで次に進まない */
			return;
		}
		for (auto cow : m_tempCows)
		{
			m_game->GetAliveCows().push_back(cow);
		}

		m_game->SetUFOList(m_tempUFOs);
	
		/** ロード完了フラグを立てる */
		m_isLoadingEnd = true;
		return;
	}
	}
	m_loadStep++;
}
void LoadingScene::LoadTitleOnly()
{
	/* 1フレームでロードに使ってよい時間*/
	constexpr double ROPE_LOAD_TIME_BUDGET_MS = 2.0;	


	/** ロープGifのロードが終わるまではここに留まる(ToGameSceneのcase 0と同じロジック) */
	if (m_ropeLoadIndex < static_cast<int>(m_ropeAnimLoadPaths.size()))
	{
		auto loadStart = std::chrono::high_resolution_clock::now();

		while (m_ropeLoadIndex < static_cast<int>(m_ropeAnimLoadPaths.size()))
		{
			m_ropeAnimSpriteRender.AddFrame(m_ropeAnimLoadPaths[m_ropeLoadIndex]);
			m_ropeLoadIndex++;

			double elapsedMs = std::chrono::duration<double, std::milli>(
				std::chrono::high_resolution_clock::now() - loadStart).count();

			if (elapsedMs >= ROPE_LOAD_TIME_BUDGET_MS)
			{
				break;
			}
		}
		return;
	}

	/** ここに来た時点でロープGifのロードは完了している。
		タイトル復帰の場合、これ以上ロードするものは無いのでロード完了扱いにする */
	m_isLoadingEnd = true;
}

Vector3 LoadingScene::RandomCowPos()
{
	Vector3 pos;

	while (true)
	{
		pos.x = (rand() % RANDOM_SPAWN_RANGE_DOUBLE) - RANDOM_SPAWN_RANGE;
		pos.y = 0.0f;
		pos.z = (rand() % RANDOM_SPAWN_RANGE_DOUBLE) - RANDOM_SPAWN_RANGE;

		/** 牛同士の距離が近すぎないようにする */
		bool isTooClose = false;

		for (auto cow : m_tempCows)
		{
			/** 牛同士の距離が近すぎる場合は
			再度ランダムな位置を生成する */
			if ((cow->GetPosition() - pos).Length() < MIN_DISTANCE)
			{
				isTooClose = true;
				break;
			}
		}

		/** 牛同士の距離が近すぎない場合はループを抜ける */
		if (!isTooClose)
		{
			return pos;;
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

	if (m_initLoadStep != InitLoadStep::Num)
	{
		return;
	}
	/** フェードアウト中は画像を描画しない */
	if (m_isReadyToStart)
	{
		return;
	}
	

	m_loadingSpriteRender[m_currentImage].Draw(rc);

	if (m_currentImage == 0)
	{
		m_ropeAnimSpriteRender.Draw(rc);
	}
	if (m_currentImage == 1)
	{
		m_foodAnimSpriteRender.Draw(rc);
		
	}
	m_loadingTextSpriteRender.Draw(rc);
}