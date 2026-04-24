#include "stdafx.h"
#include "SoundManager/SoundManager.h"
#include "GameOver.h"
#include"LoadingScene.h"
#include "Title.h"
#include "Score/Score.h"
#include "CowNumberOfRescues/CowNumberOfRescues.h"

namespace
{
	const char* GAMECLEAR_FILEPATH = "Assets/sprite/GameTransition/GameOver.dds";
	const char* BLACK_SPRITE_FILEPATH = "Assets/sprite/GameTransition/Black.DDS";

	const float BLACK_SPRITE_WIDTH = 800.0f;
	const float BLACK_SPRITE_HIGHT = 350.0f;

	const int GAMECLEAR_WIDTH = 1920.0f;
	const int GAMEOVER_HIGHT = 1080.0f;

	const Vector3 BLACK_SPRITE_POSITION = Vector3(-550.0f, 0.0f, 0.0f);
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
	//画像を読み込む
	m_GameOverspriteRender.Init(GAMECLEAR_FILEPATH, GAMECLEAR_WIDTH, GAMEOVER_HIGHT);
	//スコアを表示するためのスコアクラスを生成
	m_score = NewGO<Score>(0, "score");

	//救出数を表示するためにクラスを生成
	m_cowNumberOfRescues = NewGO<CowNumberOfRescues>(0, "cownumberofrescues");
	m_cowNumberOfRescues->SetResult(true);

	m_deathSound = FindGO<SoundManager>("soundmanager");
	p_deathBGM = m_deathSound->PlayingBGM(SoundBGM::enGameOverBGM, false);

	m_blackSpriteRender.Init(BLACK_SPRITE_FILEPATH, BLACK_SPRITE_WIDTH, BLACK_SPRITE_HIGHT);
	m_blackSpriteRender.SetPosition(BLACK_SPRITE_POSITION);
	m_blackSpriteRender.Update();
	return true;
}


void GameOver::Update()
{
	//まだスコアをセットしていないならスコアをセットする
	if (!m_isScoreSet && m_score)
	{
		m_score->SetResult(true);
		m_score->SetResultType(Score::ResultType::GameOver);
		m_score->SetScore(m_finalScore);
		m_isScoreSet = true;
	}

	//まだ救出数をセットしていないなら救出数をセットする
	if (!m_isRescueSet && m_cowNumberOfRescues)
	{
		m_cowNumberOfRescues->SetResult(true);
		m_cowNumberOfRescues->SetResultType(CowNumberOfRescues::ResultType::GameOver);
		m_cowNumberOfRescues->SetNumber(m_finalRescue);
		m_isRescueSet = true;
	}
	//GameOver時に行う処理
	InGameOver();

	if (m_score)
	{
		m_score->Update();
	}
}


void GameOver::InGameOver()
{
	if (g_pad[0]->IsPressAnyKey())
	{
		m_loadingScene = NewGO<LoadingScene>(0, "loading");
		m_loadingScene->SetNextScene([]()
			{
				/** タイトルの画像を呼び出す*/
				NewGO<Title>(0, "title");
			});
		//削除要求フラグを立てる
		m_isDeleteRequest = true;


		//削除リクエストが来ているときに削除する
		if (m_isDeleteRequest == true)
		{
			DeleteGO(p_deathBGM);
			DeleteGO(this);
		}
	}
}
//スコアをゲームオーバーの画像にセットする関数
void GameOver::SetFinalScore(int score)
{
	m_finalScore = score;
}
//救出数をゲームオーバーの画像にセットする関数
void GameOver::SetFinalRescue(int rescue)
{
	m_finalRescue = rescue;
}

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
}


