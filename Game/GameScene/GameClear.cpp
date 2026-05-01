#include "stdafx.h"
#include "SoundManager/SoundManager.h"
#include "GameClear.h"
#include "Title.h"
#include "Score/Score.h"
#include "CowNumberOfRescues/CowNumberOfRescues.h"
#include"LoadingScene.h"

namespace
{
	/** ゲームクリアの画像のファイルパス */
	const char* GAMECLEAR_FILEPATH = "Assets/sprite/GameTransition/GameClear.dds";

	/** 黒いスプライトのファイルパス */
	const char* BLACK_SPRITE_FILEPATH = "Assets/sprite/GameTransition/Black.DDS";

	/** ゲームクリアのタイトルを促す画像のファイルパス */
	const char* GAMECLEAR_PRESS_TITLE_FILEPATH = "Assets/sprite/GameTransition/PressAnyButton.dds";

	/** ゲームクリアのタイトルを促す画像の座標 */
	const Vector3 GAMECLEAR_PRESS_TITLE_POS = { 0.0f, -350.0f, 0.0f };

	/** 黒いスプライトの座標 */
	const Vector3 BLACK_SPRITE_POSITION = Vector3(-550.0f, 0.0f, 0.0f);

	/** 黒いスプライトの大きさ */
	constexpr float BLACK_SPRITE_WIDTH = 800.0f;
	constexpr float BLACK_SPRITE_HIGHT = 350.0f;

	/** ゲームクリアの画像の大きさ */
	constexpr int GAMECLEAR_WIDTH = 1920;
	constexpr int GAMECLEAR_HEIGHT = 1080;

	/** ゲームクリアのタイトルを促す画像の大きさ */
	constexpr int GAMECLEAR_PRESS_TITLE_WIDTH = 1400;
	constexpr int GAMECLEAR_PRESS_TITLE_HEIGHT = 1700;
}

GameClear::GameClear()
{

}


GameClear::~GameClear()
{
	DeleteGO(m_score);
	DeleteGO(m_cowNumberOfRescues);
}


bool GameClear::Start()
{
	/** 画像を読み込む */
	m_GameClearSpriteRender.Init(GAMECLEAR_FILEPATH, GAMECLEAR_WIDTH, GAMECLEAR_HEIGHT);
	
	/** スコアを表示するためのスコアクラスを生成 */
	m_score = NewGO<Score>(0, "score");

	/** 救出数を表示するためにクラスを生成 */
	m_cowNumberOfRescues = NewGO<CowNumberOfRescues>(0, "cownumberofrescues");
	m_clearSound = FindGO<SoundManager>("soundmanager");
	m_clearBGM = m_clearSound->PlayingBGM(SoundBGM::enGameClearBGM, false);

	/** 黒いスプライトの初期化 */
	m_blackSpriteRender.Init(BLACK_SPRITE_FILEPATH, BLACK_SPRITE_WIDTH, BLACK_SPRITE_HIGHT);
	m_blackSpriteRender.SetPosition(BLACK_SPRITE_POSITION);
	m_blackSpriteRender.Update();

	/** ゲームクリアのタイトルを促すスプライトの初期化 */
	m_gameClearPressTitleSpriteRender.Init(GAMECLEAR_PRESS_TITLE_FILEPATH, GAMECLEAR_PRESS_TITLE_WIDTH, GAMECLEAR_PRESS_TITLE_HEIGHT);
	m_gameClearPressTitleSpriteRender.SetPosition(GAMECLEAR_PRESS_TITLE_POS);
	m_gameClearPressTitleSpriteRender.Update();

	return true;
}

void GameClear::Update()
{
	/** まだスコアをセットしていないならスコアをセットする */
	if(!m_isScoreSet&& m_score)
	{
		m_score->SetResult(true);
		m_score->SetResultType(Score::ResultType::GameClear);
		m_score->SetScore(m_finalScore);
		
		m_isScoreSet = true;
	}

	/** まだ救出数をセットしていないなら救出数をセットする */
	if (!m_isRescueSet && m_cowNumberOfRescues)
	{
		m_cowNumberOfRescues->SetResult(true);
		m_cowNumberOfRescues->SetResultType(CowNumberOfRescues::ResultType::GameClear);
		m_cowNumberOfRescues->SetNumber(m_finalRescue);
		m_isRescueSet = true;
	}

	/** ゲームクリアの処理 */
	InGameClear();

	/** ゲームクリアのフェード処理 */
	FadeGameClear();

	if (m_score)
	{
		m_score->Update();
	}

	m_gameClearPressTitleSpriteRender.Update();
}


void GameClear::InGameClear()
{
	if (g_pad[0]->IsPressAnyKey() && !m_isGameClearToTitleButtonPressed)
	{
		m_isGameClearToTitleButtonPressed = true;

		m_decisionSE = m_clearSound->PlayingSE(SoundSE::enDecisionSE, false);
	}

	if(m_isGameClearToTitleButtonPressed)
	{

		m_gameClearFinalBlinkTime -= g_gameTime->GetFrameDeltaTime();

		m_gameClearBlinkInterval -= g_gameTime->GetFrameDeltaTime() * m_gameClearBlinkInterval;

		if (m_gameClearFinalBlinkTime <= 0.0f)
		{
			m_isGameClearToTitleButtonPressed = false;
			/* ゲームクリアからタイトルシーンに移行する */
			m_loadingScene = NewGO<LoadingScene>(0, "loading");
			m_loadingScene->SetLoadType(LoadingScene::LoadType::ToTitleScene);
			DeleteGO(m_clearBGM);
			DeleteGO(m_decisionSE);
			m_loadingScene->SetNextScene([]()
				{
					/** タイトルの画像を呼び出す*/
					NewGO<Title>(0, "title");
				});

			DeleteGO(this);
		}
	}
}

void GameClear::SetFinalClearScore(int score)
{
	m_finalScore = score;
}

void GameClear::SetFinalClearRescue(int rescue)
{
	m_finalRescue = rescue;
}

void GameClear::FadeGameClear()
{
	if (!m_isGameClearToTitleButtonPressed)
	{
		switch (m_gameClearState)
		{
		case FadeIn:
		{
			m_gameClearAlpha -= g_gameTime->GetFrameDeltaTime();
			if (m_gameClearAlpha <= 0.0f)
			{
				m_gameClearAlpha = 0.0f;
				m_gameClearState = FadeOut;
			}
		}
		break;

		case FadeOut:
		{
			m_gameClearAlpha += g_gameTime->GetFrameDeltaTime();
			if (m_gameClearAlpha >= 1.0f)
			{
				m_gameClearAlpha = 1.0f;
				m_gameClearState = FadeIn;
			}
		}
		break;


		default:
			break;
		}
	}
}
void GameClear::Render(RenderContext& rc)
{
	
	m_GameClearSpriteRender.Draw(rc);

	m_blackSpriteRender.SetMulColor(
		Vector4(
			1.0f,
			1.0f,
			1.0f,
			0.5f));

	m_blackSpriteRender.Draw(rc);

	if(m_score)
	{
		m_score->Render(rc);
	}

	if (!m_isGameClearToTitleButtonPressed)
	{
		if (m_gameClearAlpha > 0.0f)
		{
			m_gameClearPressTitleSpriteRender.SetMulColor(
				Vector4(
					1.0f,
					1.0f,
					1.0f,
					m_gameClearAlpha));
			m_gameClearPressTitleSpriteRender.Draw(rc);
		}

		return;
	}

	if (static_cast<int>(m_gameClearBlinkInterval * 2) % 2 == 0)
	{
		m_gameClearPressTitleSpriteRender.SetMulColor(
			Vector4(
				1.0f,
				1.0f,
				1.0f,
				1.0f));
		m_gameClearPressTitleSpriteRender.Draw(rc);
	}
}