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
	const char* LOADINGINSTRUCTION_FILEPATH = "Assets/sprite/LoadingUI/Loading_InstructionManual.dds";
	const char* LOADINGTEXT_FILEPATH =   "Assets/sprite/LoadingUI/LodingUI.dds";
	const char* BLACKLODING_FILEPATH =   "Assets/sprite/GameTransition/Black.dds";

	/** 画像の大きさ */
	/** 横幅 */
	constexpr int BLACKLOADING_WIDTH = 1920.0f;
	constexpr int LOADING_WIDTH = 1900.0f;
	constexpr int INSTRUCTION_LOADING_WIDTH = 1850.0f;
	constexpr int COWWALK_WIDTH = 300.0f;
	
	/** 縦幅 */
	constexpr int BLACKLOADING_HEIGHT = 1080.0f;
	constexpr int LOADING_HEIGHT = 850.0f;
	constexpr int INSTRUCTION_LOADING_HEIGHT = 850.0f;
	constexpr int COWWALK_HEIGHT = 400.0f;
	
	/** 牛のランダムスポーン範囲 */
	constexpr int RANDOM_SPAWN_RANGE = 500;
	constexpr int RANDOM_SPAWN_RANGE_DOUBLE = 800;

	/** 牛同士の最低距離 */
	constexpr float MIN_DISTANCE = 15.0f; // 牛同士の最低距離


	/** 歩く牛のローディングアニメーションのファイルパス*/
	const char* COWWALK_FILEPATH = "Assets/Gif/CowLoadingGif/anim_%02d.dds";
	
	/** 歩く牛のフレームの数*/
	constexpr int COWWALK_FREAM_COUNT = 25;
	/** 25枚を15fpsで再生 → 1周約1.67秒*/
	constexpr float COWWALK_FPS = 15.0f; 
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

	/** 2枚目に表示するGif画像をランダムに選ぶ*/
	m_useRopeGif = (rand() % 2 == 0);
	

	
	

	/** 最初は最初の画像を表示する */
	m_currentImage = 0;

	/** 最初に表示される画像は見たことにする*/
	m_hasSeenImage0 = true;
	m_hasSeenImage1 = false;
	m_isReadyToStart = false;
	/** 最初は0にする*/
	m_LoadingInstructionTime = 0.0f;

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
		/** 分割ロードするための関数*/
		LoadInitialSpritesStepByStep();

		/** αを黒背景だけ反映して抜ける（他のスプライトはまだ実体がない） */
		m_blackLoadingSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_SceneFadeAlpha));
		m_blackLoadingSpriteRender.Update();
		return;
	}

	/** ローディングシーンの更新処理 */
	InLoading();

	

	/** Sceneのフェード処理*/
	FadeOutLoadingScene();

	/** Gif画像が再生され終わったら自動的に画像を進める処理*/
	AutoAdvanceImage();
	

	/** 表示中の選ばれたGif画像のみ再生を進める*/
	if (m_currentImage == 1 && !m_hasSelectedGifReachedEnd)
	{
		if (m_useRopeGif)
		{
			m_ropeAnimSpriteRender.Update();
			/** ロープの画像の再生が終わったら*/
			if (m_ropeAnimSpriteRender.IsFinished())
			{
				/** フラグをtrueに変えて２枚目のGif画像にする*/
				m_hasSelectedGifReachedEnd = true;
			}
	     }
		else
		{
			m_foodAnimSpriteRender.Update();
			if (m_foodAnimSpriteRender.IsFinished())
			{
				/** フラグをtrueに変えて２枚目のGif画像にする*/
				m_hasSelectedGifReachedEnd = true;
			}
		}
	}

	
	/** 餌のアニメーションをバックグラウンドでロードする */
	LoadFoodAnimInBackground();
	
	/** 歩く牛のLoadingアニメーションを常にループ再生する*/
	m_cowWalkSpriteRender.Update();
	if (m_cowWalkSpriteRender.IsFinished())
	{
		m_cowWalkSpriteRender.Reset();
		m_cowWalkSpriteRender.Update();
	}

	
	/** αを反映させる*/
	m_blackLoadingSpriteRender.SetMulColor(
		Vector4(1.0f, 1.0f, 1.0f, m_SceneFadeAlpha)
	);

	/** Sceneの更新*/
	m_blackLoadingSpriteRender.Update();
	/** 説明画像と、選ばれたGifの背景画像だけ更新 */
	m_LoadingInstructionSpriteRender.Update();
	m_loadingSpriteRender[m_useRopeGif ? 0 : 1].Update();
	
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
		m_LoadingInstructionTime += g_gameTime->GetFrameDeltaTime();
		/** 選ばれたGif画像の読み込みが完了したら２枚目へ自動遷移する*/
		bool isSelectedGifReady = m_useRopeGif
			? (m_ropeLoadIndex >= static_cast<int>(m_ropeAnimLoadPaths.size()))
			: m_isFoodLoaded;
		/** 読み込み完了かつ最低表示時間が経過していたら切り替える*/
		bool isMinTimeElapsed = m_LoadingInstructionTime >= MIN_IMAGE0_DISPLAY_TIME;

		/** 一枚目のGif画像の読み込みが終わったかつまだ遷移していないかつ表示時間を越えていたら*/
		if (isSelectedGifReady &&isMinTimeElapsed&& !m_hasAutoAdvancedImage0)
		{
			m_hasAutoAdvancedImage0 = true;
			m_currentImage = 1;
			m_hasSeenImage1 = true;


			/** 選ばれたGif画像を最初から再生する*/
			if (m_useRopeGif)
			{
				m_ropeAnimSpriteRender.Reset();
				m_ropeAnimSpriteRender.Update();
			}
			else
			{
				m_foodAnimSpriteRender.Reset();
				m_foodAnimSpriteRender.Update();
			}
		}
	}

	else if (m_currentImage == 1)
	{
		/** ロード完了まで待機*/
		if (!m_isLoadingEnd)
		{
			return;
		}

		/** 選ばれたGif画像を最後まで再生させる*/
		if (!m_hasSelectedGifReachedEnd)
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

	/** 歩く牛のローディングアニメーションを常にループ再生する*/
	for (int i = 1; i <= COWWALK_FREAM_COUNT; i++)
	{
		char buf[256];
		sprintf(buf, COWWALK_FILEPATH, i);
		m_CowWalkAnimLoadPaths.push_back(buf);
	}

	/** ロープで引っ張るGifのフレーム数を準備する */
	m_ropeAnimSpriteRender.PrepareFrameCount(
		static_cast<int>(m_ropeAnimLoadPaths.size()), 610.0f, 590.0f, 10.0f);
	m_ropeAnimSpriteRender.SetPosition({ -535.0f, 10.0f, 0.0f });

	/** 餌のGifのフレーム数を準備する */
	m_foodAnimSpriteRender.PrepareFrameCount(
		static_cast<int>(m_foodAnimLoadPaths.size()), 610.0f, 590.0f, 10.0f);
	m_foodAnimSpriteRender.SetPosition({ -535.0f, 10.0f, 0.0f });

	/** 25枚程度なので一括ロードする*/
	m_cowWalkSpriteRender.Init(
		m_CowWalkAnimLoadPaths,
		static_cast<int>(m_CowWalkAnimLoadPaths.size()),
		COWWALK_WIDTH, COWWALK_HEIGHT, COWWALK_FPS);
	m_cowWalkSpriteRender.SetPosition(Vector3(800.0f, -400.0f, 0.0f));
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



void LoadingScene::LoadInitialSpritesStepByStep()
{
	switch (m_initLoadStep)
	{
		/** 操作説明の画像を生成*/
	case InitLoadStep::LoadingInstruction:
		m_LoadingInstructionSpriteRender.Init(LOADINGINSTRUCTION_FILEPATH, INSTRUCTION_LOADING_WIDTH, LOADING_HEIGHT);
		m_LoadingInstructionSpriteRender.SetPosition(Vector3(0.0f, 90.0f, 0.0f));
		m_LoadingInstructionSpriteRender.Update();
		m_initLoadStep = InitLoadStep::LoadingGifBackGroundImage;
		break;
		
	case InitLoadStep::LoadingGifBackGroundImage:
	{
		/** 選ばれたGifに対応する背景画像だけロードする */
		int bgIndex = m_useRopeGif ? 0 : 1;
		const char* bgPath = m_useRopeGif ? COWHOOKLOAD_FILEPATH : COWFOODLOAD_FILEPATH;

		m_loadingSpriteRender[bgIndex].Init(bgPath, LOADING_WIDTH, LOADING_HEIGHT);
		m_loadingSpriteRender[bgIndex].SetPosition(Vector3(0.0f, 100.0f, 0.0f));
		m_loadingSpriteRender[bgIndex].Update();
		m_initLoadStep = InitLoadStep::Num;
		break;
	}
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
		cow->SetUFOAttracted(cowIndex % 2 == 0);
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

	if (m_currentImage == 0)
	{
		m_LoadingInstructionSpriteRender.Draw(rc);
	}
	
	else if (m_currentImage == 1)
	{
		/** ランダムに選ばれたGifとその背景画像の描画*/
		int bgIndex = m_useRopeGif ? 0 : 1;
		m_loadingSpriteRender[bgIndex].Draw(rc);

		/**trueだったらロープのGif画像を描画する */
		if (m_useRopeGif)
		{
			m_ropeAnimSpriteRender.Draw(rc);
		}
		/** falseだったら餌のGif画像を描画する*/
		else
		{
			m_foodAnimSpriteRender.Draw(rc);
		}
	}
	m_cowWalkSpriteRender.Draw(rc);
}