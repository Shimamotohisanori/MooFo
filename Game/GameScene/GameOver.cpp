#include "stdafx.h"
#include "SoundManager/SoundManager.h"
#include "GameOver.h"
#include"LoadingScene.h"
#include "Title.h"
#include "Score/Score.h"
#include "CowNumberOfRescues/CowNumberOfRescues.h"

namespace
{
	/** ゲームオーバーの画像のファイルパス */
	const char* GAMEOVER_FILEPATH = "Assets/sprite/GameTransition/GameOver.dds";

	/** 黒いスプライトのファイルパス */
	const char* BLACK_SPRITE_FILEPATH = "Assets/sprite/GameTransition/Black.DDS";

	/** ゲームオーバーのタイトルを促す画像のファイルパス */
	const char* GAMEOVER_PRESS_TITLE_FILEPATH = "Assets/sprite/GameTransition/PressAnyButton.dds";

	/** ゲームオーバーのタイトルを促す画像の座標 */
	const Vector3 GAMEOVER_PRESS_TITLE_POS = { 0.0f, -350.0f, 0.0f };

	/** 黒いスプライトの座標 */
	const Vector3 BLACK_SPRITE_POSITION = Vector3(-550.0f, 0.0f, 0.0f);

	/** 黒いスプライトの大きさ */
	const float BLACK_SPRITE_WIDTH = 800.0f;
	const float BLACK_SPRITE_HIGHT = 350.0f;

	/** ゲームオーバーのタイトルを促す画像の大きさ */
	constexpr int GAMEOVER_PRESS_TITLE_WIDTH = 1400;
	constexpr int GAMEOVER_PRESS_TITLE_HEIGHT = 1700;

	/** ゲームオーバーの画像の大きさ */
	constexpr int GAMEOVER_WIDTH = 1920;
	constexpr int GAMEOVER_HIGHT = 1080;

	/** ゲームオーバー画面での点滅の最後の間隔 */
	constexpr float GAMEOVER_FINAL_BLINK_INTERVAL = 1.0f;
}

GameOver::GameOver()
{

}

GameOver::~GameOver()
{
	DeleteGO(m_score);

	DeleteGO(m_cowNumberOfRescues);
}

bool GameOver::Start()
{
	/** 画像を読み込む */
	m_GameOverspriteRender.Init(GAMEOVER_FILEPATH, GAMEOVER_WIDTH, GAMEOVER_HIGHT);

	/** スコアを表示するためのスコアクラスを生成 */
	m_score = NewGO<Score>(0, "score");

	/** 救出数を表示するためにクラスを生成 */
	m_cowNumberOfRescues = NewGO<CowNumberOfRescues>(0, "cownumberofrescues");
	m_cowNumberOfRescues->SetResult(true);

	/** ゲームオーバーのBGMを流す */
	m_deathSound = FindGO<SoundManager>("soundmanager");
	m_deathBGM = m_deathSound->PlayingBGM(SoundBGM::enGameOverBGM, false);

	/** 黒いスプライトの初期化 */
	m_blackSpriteRender.Init(BLACK_SPRITE_FILEPATH, BLACK_SPRITE_WIDTH, BLACK_SPRITE_HIGHT);
	m_blackSpriteRender.SetPosition(BLACK_SPRITE_POSITION);
	m_blackSpriteRender.Update();

	/** ゲームオーバーのタイトルを促す画像の初期化 */
	m_gameOverPressTitleSpriteRender.Init(GAMEOVER_PRESS_TITLE_FILEPATH, GAMEOVER_PRESS_TITLE_WIDTH, GAMEOVER_PRESS_TITLE_HEIGHT);
	m_gameOverPressTitleSpriteRender.SetPosition(GAMEOVER_PRESS_TITLE_POS);
	m_gameOverPressTitleSpriteRender.Update();

	return true;
}


void GameOver::Update()
{
	/** まだスコアをセットしていないならスコアをセットする */
	if (!m_isScoreSet && m_score)
	{
		m_score->SetResult(true);
		m_score->SetResultType(Score::ResultType::GameOver);
		m_score->SetScore(m_finalScore);
		m_isScoreSet = true;
	}

	/** まだ救出数をセットしていないなら救出数をセットする */
	if (!m_isRescueSet && m_cowNumberOfRescues)
	{
		m_cowNumberOfRescues->SetResult(true);
		m_cowNumberOfRescues->SetResultType(CowNumberOfRescues::ResultType::GameOver);
		m_cowNumberOfRescues->SetNumber(m_finalRescue);
		m_isRescueSet = true;
	}

	/** GameOver時に行う処理 */
	InGameOver();

	/** ゲームオーバーのフェード処理 */
	FadeGameOver();

	if (m_score)
	{
		m_score->Update();
	}

	m_gameOverPressTitleSpriteRender.Update();
}


void GameOver::InGameOver()
{
	if (g_pad[0]->IsPressAnyKey() && !m_isGameOverToTitleButtonPressed)
	{
		m_isGameOverToTitleButtonPressed = true;

		m_decisionSE = m_deathSound->PlayingSE(SoundSE::enDecisionSE, false);
	}

	if (m_isGameOverToTitleButtonPressed)
	{
		/* 点滅の間隔を減らしていく */
		m_gameOverBlinkInterval -= g_gameTime->GetFrameDeltaTime() * m_gameOverBlinkInterval;

		/* 点滅の間隔が最後の間隔以下になったときに、ゲームオーバーからタイトルシーンに移行する */
		if (m_gameOverBlinkInterval <= GAMEOVER_FINAL_BLINK_INTERVAL)
		{
			m_isGameOverToTitleButtonPressed = false;
			m_loadingScene = NewGO<LoadingScene>(0, "loading");
			m_loadingScene->SetLoadType(LoadingScene::LoadType::ToTitleScene);
			DeleteGO(m_decisionSE);
			DeleteGO(m_deathBGM);
			m_loadingScene->SetNextScene([]()
				{
					/** タイトルの画像を呼び出す*/
					NewGO<Title>(0, "title");
				});
			DeleteGO(this);
		}
	}
}

/** スコアをゲームオーバーの画像にセットする関数 */
void GameOver::SetFinalScore(int score)
{
	m_finalScore = score;
}

/** 救出数をゲームオーバーの画像にセットする関数 */
void GameOver::SetFinalRescue(int rescue)
{
	m_finalRescue = rescue;
}

void GameOver::FadeGameOver()
{
	if(!m_isGameOverToTitleButtonPressed)
	{
		/** フェード処理*/
		switch (m_gameOverState)
		{
		case FadeIn:
			m_gameOverAlpha += g_gameTime->GetFrameDeltaTime();
			if (m_gameOverAlpha >= 1.0f)
			{
				m_gameOverAlpha = 1.0f;
				m_gameOverState = FadeOut;
			}
			break;

		case FadeOut:
			m_gameOverAlpha -= g_gameTime->GetFrameDeltaTime();
			if (m_gameOverAlpha <= 0.0f)
			{
				m_gameOverAlpha = 0.0f;
				m_gameOverState = FadeIn;
			}
			break;
		}
	}
}

/** ゲームオーバーの画像を描画する関数 */
void GameOver::Render(RenderContext& rc)
{
	m_GameOverspriteRender.Draw(rc);
	
	m_blackSpriteRender.SetMulColor(
		Vector4(
			1.0f, 
			1.0f, 
			1.0f, 
			0.5f));
	m_blackSpriteRender.Draw(rc);
	
	if (m_score)
	{
		m_score->Render(rc);
	}

	if (!m_isGameOverToTitleButtonPressed)
	{
		if (m_gameOverAlpha > 0.0f)
		{
			m_gameOverPressTitleSpriteRender.SetMulColor(
				Vector4(
					1.0f,
					1.0f,
					1.0f,
					m_gameOverAlpha));
			m_gameOverPressTitleSpriteRender.Draw(rc);
		}

		return;
	}
	
	if (static_cast<int>(m_gameOverBlinkInterval * 2) % 2 == 0)
	{
		m_gameOverPressTitleSpriteRender.SetMulColor(
			Vector4(
				1.0f,
				1.0f,
				1.0f,
				1.0f));
		m_gameOverPressTitleSpriteRender.Draw(rc);
	}

}


