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
#include"Source/Actor/Stage/CowFoodManager.h"
#include"Source/Actor/Stage/CowFood.h"
TutorialManager::TutorialManager()
{

}
TutorialManager::~TutorialManager()
{

}

bool TutorialManager::Start()
{
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
		m_setupStep = EnSetupStep::NotStarted;
		break;
		/** 救出成功→餌を置くに切り替わった瞬間に、新しい牛を出す*/
	case EnTutorialStep::PlaceFood: 
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
		
		SpawnTutorialCowFoodStep();
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

	}
		break;
	default:
		break;
	}
}


void TutorialManager::SpawnTutorialCowFoodStep()
{
	Game* game = FindGO<Game>("game");
	if (game == nullptr) return;

	/** 牛舎(-1320, 0, 10)から離れた位置に出し、餌で誘導させる */
	m_foodTutorialCow = NewGO<Cow>(0, "cow");
	m_foodTutorialCow->SetPosition(Vector3(400.0f, 0.0f, 0.0f));
	m_foodTutorialCow->SetCowType(Cow::EnCowType::en_Random);

	game->AddTutorialCow(m_foodTutorialCow);
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
		m_tutorialCow->SetCowType(Cow::EnCowType::en_Random);

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