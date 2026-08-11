#include "stdafx.h"
#include "TutorialManager.h"
#include"Source/Actor/Character/Cow/Cow.h"
#include"Source/Actor/Character/UFO/UFO.h"
#include"FadeManager/FadeManager.h"
#include"GameScene/Game.h"
#include"GameScene/LoadingScene.h"
#include"GameScene/Title.h"
#include"Source/Actor/Character/Player/Player.h"
#include"GameCamera/GameCamera.h"
#include"Rope/Rope.h"
#include"Pause/Pause.h"
#include"Source/Actor/Stage/CowFoodManager.h"
#include"Source/Actor/Stage/CowFood.h"
namespace
{
	/** 各チュートリアルで出すUIスプライト*/

	/** ロープ投げチュートリアルUI */
	const char* ROPE_TUTORIAL_UI_FILEPATH = "Assets/sprite/Tutorial/RopeTutorialUI.dds";
	/** 牛救助チュートリアルUI */
	const char* RESCUE_TUTORIAL_UI_FILEPATH = "Assets/sprite/Tutorial/RescueTutorialUI.dds";
	/** 餌を置くチュートリアルUI */
	const char* COWFOOD_TUTORIAL_UI_FILEPATH = "Assets/sprite/Tutorial/CowFoodTutorialUI.dds";
	/** 牛舎誘導チュートリアルUI */
	const char* GUIDE_TUTORIAL_UI_FILEPATH = "Assets/sprite/Tutorial/GuideCowShedTutorialUI.dds";

	/** UI画像のサイズ*/
	const int TUTORIAL_UI_WIDTH = 400;
	const int TUTORIAL_UI_HEIGHT = 500;


	/** 数字UIのファイルパス*/
	const char* NUMBER_FILEPATH = "Assets/sprite/NumberUI/";
	const char* NUMBER_FORMAT = ".dds";
	const char* NUMBER_FILENAME_LIST[10] =
	{
		"MooFoNumberUI0", "MooFoNumberUI1", "MooFoNumberUI2", "MooFoNumberUI3", "MooFoNumberUI4",
		"MooFoNumberUI5", "MooFoNumberUI6", "MooFoNumberUI7", "MooFoNumberUI8", "MooFoNumberUI9"
	};
	/** 「/」画像のファイルパス*/
	const char* SLASH_FILEPATH = "Assets/sprite/CowNumberOfRescuesUI/slash.DDS";

	/** 数字・スラッシュのサイズ*/
	const Vector2 NUMBER_SPRITE_SIZE = Vector2(60.0f, 60.f);
	const Vector2 SLASH_SPRITE_SIZE = Vector2(100.0f, 100.0f);

	/** チュートリアルUI画像を基準にしたカウント表示の相対座標(要調整)*/
	const Vector3 COUNT_CURRENT_OFFSET = Vector3(-10.0f, -150.0f, 0.0f);
	const Vector3 COUNT_SLASH_OFFSET = Vector3(-10.0f, -150.0f, 0.0f);
	const Vector3 COUNT_REQUIRED_OFFSET = Vector3(70.0f, -150.0f, 0.0f);


}
TutorialManager::TutorialManager()
{

}
TutorialManager::~TutorialManager()
{

}

bool TutorialManager::Start()
{
	/** 数字画像を読み込む*/
	for (int i = 0; i < 10; ++i)
	{
		std::string filepath = std::string(NUMBER_FILEPATH )+ std::string(NUMBER_FILENAME_LIST[i]) + NUMBER_FORMAT;
		m_numberSprite[i].Init(filepath.c_str(), NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
	}

	/** 「/」の画像を読み込む*/
	m_slashSprite.Init(SLASH_FILEPATH, SLASH_SPRITE_SIZE.x, SLASH_SPRITE_SIZE.y);

	/** チュートリアルUIスプライトを更新 */
	UpdateTutorialUISprite();
	return true;
}
void TutorialManager::Update()
{
	if (m_fadeManager == nullptr)
	{
		m_fadeManager = FindGO<FadeManager>("fadeManager");
	}
	/** フェードのポインタが何もなかったらこの下の処理を飛ばす*/
	if (m_fadeManager == nullptr)
	{
		return;
	}
	/** 完了していたらクリア演出だけ更新する*/
	if (IsComplete())
	{
		UpdateCompleteStep();
		return;
	}
	/** RopeThrowステップ中は牛・UFOのセットアップを行わない(何も出現させない)*/
	if (m_cuurentStep != EnTutorialStep::RopeThrow)
	{
		if (m_setupStep != EnSetupStep::Done)
		{
			SetupTutorialActors();
			return;
		}
	}
	else
	{
		/** ★RopeThrow中でも、プレイヤーの初期位置記憶だけは行っておく */
		CaptureInitialTransformIfNeeded();
	}

	/** CowRescue中、UFOに連れ去られて救出に失敗したら新しい牛を生成する
	    既に成功したら何もしない*/
	if (m_cuurentStep == EnTutorialStep::CowRescue
		&& !m_isTransitioning
		&& m_successCount < GetRequiredCount(m_cuurentStep))
	{
		CheckAndRespawnCowTutorial();
	}

	UpdateStepTransition();
}





void TutorialManager::AddSuccessCount(EnTutorialStep step)
{
	/** 現在のステップと一致しないなら無視する(遷移中の誤カウント防止) */
	if (step != m_cuurentStep)
	{
		return;
	}

	/** 遷移中(フェード中)は新たなカウントを受け付けない */
	if (m_isTransitioning)
	{
		return;
	}

	m_successCount++;
}


void TutorialManager::UpdateCompleteStep()
{
	/** チュートリアルが終わったらタイトルに戻る*/
		if(!m_hasEndedTutorial)
		{
			TransitionToTitle();
		}
}




void TutorialManager::CheckAndRespawnCowTutorial()
{
	/** 牛がまだ生きている(削除予約されていたら)何もしない*/
	if (m_tutorialCow != nullptr
		&& !m_tutorialCow->GetIsDeadFlag()
		&& !m_tutorialCow->GetIsPendingKill())
	{
		return;
	}

	/** UFOが存在しなければ再生成できないので何もしない*/
	if (m_tutorialUFO == nullptr || m_tutorialUFO->IsDead())
	{
		return;
	}

	Game* game = FindGO<Game>("game");
	if (game == nullptr)
	{
		return;
	}
	/** 救出に失敗して連れ去られた牛が消えたら光に向かって進む牛を再生成する*/
	m_tutorialCow = NewGO<Cow>(0, "cow");
	m_tutorialCow->SetPosition(Vector3(0.0f, 0.0f, 200.0f));
	m_tutorialCow->SetCowType(Cow::EnCowType::en_Light);
	game->AddTutorialCow(m_tutorialCow);

	/** UFOの光が消えている可能性があるので、念のため再度強制的に光を出す */
	m_tutorialUFO->ForceEmitLightForTutorial();

}
void TutorialManager::TransitionToTitle()
{
/** タイトルシーンへの遷移処理 */

	if (m_hasEndedTutorial)
	{
		return;
	}
	m_hasEndedTutorial = true;
/** LoadingSceneを経由してTitleに戻る*/
LoadingScene* loadingScene = NewGO<LoadingScene>(0, "loadingScene");
loadingScene->SetLoadType(LoadingScene::LoadType::ToTitleScene);
loadingScene->SetNextScene([]()
	{
		NewGO<Title>(0, "title");
	});
Game* game = FindGO<Game>("game");
if (game)
  {
	game->EndTutorial();
  }
}



void TutorialManager::UpdateTutorialUISprite()
{
	const char* texPath = nullptr;
	Vector3 uipos = Vector3::Zero;

	/** ステップごとの、UI画像基準のオフセット(要調整)*/
	Vector3 currentOffset = Vector3::Zero;
	Vector3 slashOffset = Vector3::Zero;
	Vector3 requiredOffset = Vector3::Zero;
	switch (m_cuurentStep)
	{
		/** ロープ投げステップ */
	case EnTutorialStep::RopeThrow:
		texPath = ROPE_TUTORIAL_UI_FILEPATH;
		uipos = Vector3(30.0f, 430.0f, 0.0f);
		currentOffset = Vector3(-90.0f, -150.0f, 0.0f);
		slashOffset = Vector3(-10.0f, -150.0f, 0.0f);
		requiredOffset = Vector3(50.0f, -150.0f, 0.0f);
		break;
		/** 牛救助ステップ */
	case EnTutorialStep::CowRescue:
		texPath = RESCUE_TUTORIAL_UI_FILEPATH;
		uipos = Vector3(0.0f, 430.0f, 0.0f);
		currentOffset = Vector3(-60.0f, -150.0f, 0.0f);
		slashOffset = Vector3(10.0f, -150.0f, 0.0f);
		requiredOffset = Vector3(70.0f, -150.0f, 0.0f);

		break;
		/** 餌を置くステップ */
	case EnTutorialStep::PlaceFood:
		texPath = COWFOOD_TUTORIAL_UI_FILEPATH;
		uipos = Vector3(-40.0f, 430.0f, 0.0f);
		currentOffset = Vector3(-20.0f, -150.0f, 0.0f);
		slashOffset = Vector3(50.0f, -150.0f, 0.0f);
		requiredOffset = Vector3(100.0f, -150.0f, 0.0f);

		break;
		/** 牛舎誘導ステップ */
	case EnTutorialStep::GuideToBarn:
		texPath = GUIDE_TUTORIAL_UI_FILEPATH;
		uipos = Vector3(0.0f, 430.0f, 0.0f);
		currentOffset = Vector3(-40.0f, -150.0f, 0.0f);
		slashOffset = Vector3(20.0f, -150.0f, 0.0f);
		requiredOffset = Vector3(70.0f, -150.0f, 0.0f);
		break;

	default:
		m_isTutorialUIVisible = false;
		return;
	}

	/** テクスチャを切り替えて初期化しなおす*/
	m_tutorialUISprite.Init(texPath, TUTORIAL_UI_WIDTH, TUTORIAL_UI_HEIGHT);
	/** 位置を設定 */
	m_tutorialUISprite.SetPosition(uipos);
	/** 更新 */
	m_tutorialUISprite.Update();

	/** カウント表示の位置計算用に基準位置を保存*/
	m_tutorialUIPos = uipos;

	/** ステップごとの数字・スラッシュの絶対位置を計算して保存*/
	m_currentCountPos = uipos + currentOffset;
	m_slashPos = uipos + slashOffset;
	m_requiredCountPos = uipos + requiredOffset;

	m_isTutorialUIVisible = true;
}


void TutorialManager::UpdateStepTransition()
{
	/** まだ遷移を開始しておらず、閾値に到達していたら暗転を開始する */
	if (!m_isTransitioning && m_successCount >= GetRequiredCount(m_cuurentStep))
	{
		m_fadeManager->SetFadeIn();
		m_isTransitioning = true;
		m_hasSwitchedStep = false;
		return;
	}

	if (!m_isTransitioning)
	{
		return;
	}

	/** 暗転が完了した瞬間(画面が真っ黒)にして裏側でプレイヤーの状態などをリセットして次のステップに進む*/
	if (m_fadeManager->IsFadeInComplete() && !m_hasSwitchedStep)
	{
		/** プレイヤー・カメラ・ロープの状態をリセット*/
		ResetPlayerForNextStep();
		/** 次のステップに進む*/
		AdvanceStep();

		m_hasSwitchedStep = true;

		m_fadeManager->SetFadeOut();
	}

	/** フェードアウトの処理が終わったら遷移状態を解除して、次のステップの受付を開始する*/
	if (m_hasSwitchedStep && m_fadeManager->IsFadeOutComplete())
	{
		m_isTransitioning = false;
		m_hasSwitchedStep = false;
	}
}


void TutorialManager::ResetPlayerForNextStep()
{
	/** 初期変換がキャプチャされているか確認 */
	if (!m_hasCapturedInitialTransform)
	{
		return;
	}
	/** プレイヤの状態をリセットする*/
	Player* player = FindGO<Player>("player");
	if (player)
	{
		player->SetPosition(m_playerInitialPos);
		player->SetRotation(m_playerInitialRot);
	}

	/** ゲームカメラの状態をリセットする*/
	GameCamera* gameCamera = FindGO<GameCamera>("gameCamera");
	if (gameCamera)
	{
		gameCamera->ResetCameraPosition();
	}
	
	/** ロープの状態をリセットする*/
	Rope* rope = FindGO<Rope>("rope");
	if (rope)
	{
		rope->ResetRopeState();
	}

}


void TutorialManager::CaptureInitialTransformIfNeeded()
{
	/** 初期変換をキャプチャする必要があるか確認 */
	if (m_hasCapturedInitialTransform)
	{
		return;
	}
	Player* player = FindGO<Player>("player");
	if (player)
	{
		m_playerInitialPos = player->GetPosition();
		m_playerInitialRot = player->GetRotation();
		m_hasCapturedInitialTransform = true;
	}
}

void TutorialManager::AdvanceStep()
{
	m_cuurentStep = static_cast<EnTutorialStep>(static_cast<int>(m_cuurentStep) + 1);
	m_successCount = 0;

	/*ステップごとの牛・餌・UFOの再セットアップをここで行う*/

	switch (m_cuurentStep)
	{
		/** CowRescueに切り替わった瞬間に牛とUFOのセットアップ開始*/
	case EnTutorialStep::CowRescue:
	{
		CowFoodManager* foodManager = FindGO<CowFoodManager>("cowfoodmanager");
		if (foodManager)
		{
			foodManager->ClearAllFood();
		}

		/** 餌の所持数もリセットしておく*/
		CowFood* cowFood = FindGO<CowFood>("cowfood");
		if (cowFood)
		{
			cowFood->SetFoodCount(2);
		}

		m_setupStep = EnSetupStep::NotStarted;
	}
		break;
		/** 救出成功→餌を置くに切り替わった瞬間に、新しい牛を出す*/
	case EnTutorialStep::PlaceFood:
	{
		/** 先にGame側のスロット参照を切ってから削除する*/
		if (m_tutorialUFO && !m_tutorialUFO->IsDead())
		{
			Game* game = FindGO<Game>("game");
			if (game)
			{
				game->ClearUFOSlot(0);
			}
			DeleteGO(m_tutorialUFO);
			m_tutorialUFO = nullptr;
		}
		break;

		CowFoodManager* foodManager = FindGO<CowFoodManager>("cowfoodmanager");
		if (foodManager)
		{
			foodManager->ClearAllFood();
		}

		/** 餌の所持数もリセットしておく*/
		CowFood* cowFood = FindGO<CowFood>("cowfood");
		if (cowFood)
		{
			cowFood->SetFoodCount(2);
		}
	}
		break;
		/**PlaceFoodで置いた牛の餌をすべて削除してから牛舎に誘導するチュートリアルを始める*/ 
	case EnTutorialStep::GuideToBarn:
	{
		CowFoodManager* foodManager = FindGO<CowFoodManager>("cowfoodmanager");
		if (foodManager)
		{
			foodManager->ClearAllFood();
		}

		/** 餌の所持数もリセットしておく*/
		CowFood* cowFood = FindGO<CowFood>("cowfood");
		if (cowFood)
		{
			cowFood->SetFoodCount(2);
		}
		/** 餌をチュートリアル中に牛舎へ届けられてすでに消滅している場合は
		GuideToBarn用に牛を再生成する*/

		if (m_foodTutorialCow == nullptr || m_foodTutorialCow->IsDead())
		{
			Game* game = FindGO<Game>("game");
			if (game)
			{
				m_foodTutorialCow = NewGO<Cow>(0, "cow");
				m_foodTutorialCow->SetPosition(Vector3(400.0f, 0.0f, 0.0f));
				m_foodTutorialCow->SetCowType(Cow::EnCowType::en_Random);
				game->AddTutorialCow(m_foodTutorialCow);
			}
		}
	}
	break;
	default:
		break;
	}


	/** ステップが切り替わったらUIも更新 */
	UpdateTutorialUISprite();
}





int TutorialManager::GetRequiredCount(EnTutorialStep step) const
{
	switch (step)
	{
	case EnTutorialStep::RopeThrow:   return REQUIRED_ROPE_THROW;
	case EnTutorialStep::CowRescue:   return REQUIRED_COW_RESCUE;
	case EnTutorialStep::PlaceFood:   return REQUIRED_PLACE_FOOD;
	case EnTutorialStep::GuideToBarn: return REQUIRED_GUIDE_BARN;
	default: return 0;
	}
}


void TutorialManager::SetupTutorialActors()
{
	Game* game = FindGO<Game>("game");
	if (game == nullptr)return;

	switch(m_setupStep)
	{
	case EnSetupStep::NotStarted:
		/** プレイヤー初期位置の記憶は共通関数に任せる */
		CaptureInitialTransformIfNeeded();
		m_setupStep = EnSetupStep::SpawnCow;
		break;
		
		/** チュートリアル用の牛を一体生成*/
	case EnSetupStep::SpawnCow:
	{
		m_tutorialCow = NewGO<Cow>(0, "cow");
		/** 座標を設定*/
		m_tutorialCow->SetPosition(Vector3(200.0f, 0.0f, 200.0f));
		/** 牛の種類を設定*/
		m_tutorialCow->SetCowType(Cow::EnCowType::en_Light);

		/** Gameのaliveリストに登録しないとロープの当たり判定の対象にならない*/
		game->AddTutorialCow(m_tutorialCow);

		m_setupStep = EnSetupStep::SpawnUFO;
		break;
	}
	
	/** UFOを生成する(スポーン演出はスキップして即座に稼働状態にする)*/
	case EnSetupStep::SpawnUFO:
	{
		m_tutorialUFO = NewGO<UFO>(0, "UFO");
		/** 座標を設定*/
		m_tutorialUFO->SetPosition(Vector3(0.0f, 70.0f, 400.0f));
		/** 演出無しで即座に動かす*/
		m_tutorialUFO->SetUFOMoveState();

		game->SetTutorialUFO(0, m_tutorialUFO);
		m_setupStep = EnSetupStep::WaitUFOReady;
		break;
	}

	/** UFOのStartが完了(CowCaptureControllerが有効になる)のを待つ*/
	case EnSetupStep::WaitUFOReady:
	{
		if (m_tutorialUFO->GetCowCaptureController() != nullptr)
		{
			m_setupStep = EnSetupStep::ForceLight;
		}
		break;
	}

	/** 強制的に光を出して牛を狙わせる*/
	case EnSetupStep::ForceLight:
	{
		m_tutorialUFO->ForceEmitLightForTutorial();
		m_setupStep = EnSetupStep::Done;
		break;
	}

	case EnSetupStep::Done:
		break;
	}
}


void TutorialManager::Render(RenderContext& rc)
{
	/**フェード完了までUIの表示を遅らす*/
	LoadingScene* lodingScene = FindGO<LoadingScene>("loading");
	if (lodingScene != nullptr && !lodingScene->GetLoadingEnd())
	{
		return;
	}
	/** チュートリアルUIを表示する*/
	if (!m_isTutorialUIVisible)
	{
		return;
	}

	Pause* pause = FindGO<Pause>("pause");
	if (pause&&pause->GetIsPause())
	{
		return;
	}
	

	/** ステップ遷移中(暗転〜次のステップへの切り替え完了まで)はUIを描画しない */
	if (m_isTransitioning)
	{
		return;
	}

	m_tutorialUISprite.Draw(rc);

	/** 完了演出中はカウントを出さない*/
	if (IsComplete())
	{
		return;
	}

	

	/** 現在数/必要数を毎フレーム取得して描画(達成すると自動でカウントアップ表示される)*/
	int currentOnes = m_successCount % 10;
	int requiredOnes = GetRequiredCount(m_cuurentStep) % 10;

	

	m_numberSprite[currentOnes].SetPosition(m_currentCountPos);
	m_numberSprite[currentOnes].Update();
	m_numberSprite[currentOnes].Draw(rc);

	m_slashSprite.SetPosition(m_slashPos);
	m_slashSprite.Update();

	m_slashSprite.Draw(rc);

	m_numberSprite[requiredOnes].SetPosition(m_requiredCountPos);
	m_numberSprite[requiredOnes].Update();
	m_numberSprite[requiredOnes].Draw(rc);

}