#include "stdafx.h"
#include "SoundManager/SoundManager.h"
#include "GameOver.h"
#include"Game.h";
#include"Title.h";
#include"Score.h"
#include"CowNumberOfRescues/CowNumberOfRescues.h"
namespace
{
	const char* FILEPATH = "Assets/sprite/GameTransition/GameOver2.dds";
	const int WIDTH = 1920.0f;
	const int HIGHT = 1080.0f;
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
	m_GameOverspriteRender.Init(FILEPATH, WIDTH, HIGHT);
	//スコアを表示するためのスコアクラスを生成
	m_score = NewGO<Score>(0, "score");

	//救出数を表示するためにクラスを生成
	m_cowNumberOfRescues = NewGO<CowNumberOfRescues>(0, "cownumberofrescues");
	m_cowNumberOfRescues->SetResult(true);

	m_deathSound = FindGO<SoundManager>("soundmanager");
	p_deathBGM = m_deathSound->PlayingBGM(SoundBGM::enGameOverBGM, false);
	return true;
}


void GameOver::Update()
{
	//まだスコアをセットしていないならスコアをセットする
	if (!m_isScoreSet && m_score)
	{
		m_score->SetResult(true);
		m_score->SetResultType(Score::ResultType::GameOver);
		m_isScoreSet = true;
	}

	//まだ救出数をセットしていないなら救出数をセットする
	if (!m_isRescueSet && m_cowNumberOfRescues)
	{
		m_cowNumberOfRescues->SetResult(true);
		m_cowNumberOfRescues->SetResultType(CowNumberOfRescues::ResultType::GameOver);
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
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//Gameを削除
		Game* game = FindGO<Game>("game");
		if (game)
		{
			DeleteGO(game);
		}
		NewGO<Title>(0, "title");
		m_isDeleteRequest = true;
	}
	//削除リクエストが来ているときに削除する
		if (m_isDeleteRequest == true)
	{
		DeleteGO(this);

		DeleteGO(p_deathBGM);

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

	if (m_score)
	{
		m_score->Render(rc);
	}
}


