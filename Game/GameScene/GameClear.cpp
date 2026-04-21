#include "stdafx.h"
#include "SoundManager/SoundManager.h"
#include "GameClear.h"
#include "Game.h"
#include "Title.h"
#include "Score/Score.h"
#include "CowNumberOfRescues/CowNumberOfRescues.h"

namespace
{
	const char* GAMECLEAR_FILEPATH = "Assets/sprite/GameTransition/GameClear.dds";
	const char* BLACK_SPRITE_FILEPATH = "Assets/sprite/GameTransition/Black.DDS";

	const float BLACK_SPRITE_WIDTH = 800.0f;
	const float BLACK_SPRITE_HIGHT = 350.0f;

	const int GAMECLEAR_WIDTH = 1920;
	const int GAMECLEAR_HEIGHT = 1080;

	const Vector3 BLACK_SPRITE_POSITION = Vector3(-550.0f, 0.0f, 0.0f);
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
	m_GameClearSpriteRender.Init(GAMECLEAR_FILEPATH, GAMECLEAR_WIDTH, GAMECLEAR_HEIGHT);
	//スコアを表示するためのスコアクラスを生成
	m_score = NewGO<Score>(0, "score");

	//救出数を表示するためにクラスを生成
	m_cowNumberOfRescues = NewGO<CowNumberOfRescues>(0, "cownumberofrescues");
	m_clearSound = FindGO<SoundManager>("soundmanager");
	p_clearBGM = m_clearSound->PlayingBGM(SoundBGM::enGameClearBGM, false);

	m_blackSpriteRender.Init(BLACK_SPRITE_FILEPATH, BLACK_SPRITE_WIDTH, BLACK_SPRITE_HIGHT);
	m_blackSpriteRender.SetPosition(BLACK_SPRITE_POSITION);
	m_blackSpriteRender.Update();
	return true;
}

void GameClear::Update()
{
	//まだスコアをセットしていないならスコアをセットする
	if(!m_isScoreSet&& m_score)
	{
		m_score->SetResult(true);
		m_score->SetResultType(Score::ResultType::GameClear);
		m_score->SetScore(m_finalScore);
		
		m_isScoreSet = true;
	}

	//まだ救出数をセットしていないなら救出数をセットする
	if (!m_isRescueSet && m_cowNumberOfRescues)
	{
		m_cowNumberOfRescues->SetResult(true);
		m_cowNumberOfRescues->SetResultType(CowNumberOfRescues::ResultType::GameClear);
		m_cowNumberOfRescues->SetNumber(m_finalRescue);
		m_isRescueSet = true;
	}

	InGameClear();
	if (m_score)
	{
		m_score->Update();
	}
}


void GameClear::InGameClear()
{
	if (g_pad[0]->IsPressAnyKey())
	{
		Game* game = FindGO<Game>("game");
		if (game)
		{
			DeleteGO(game);
		}
		//タイトルの画像を呼び出す
		NewGO<Title>(0, "title");
		//自信を削除するフラグを立てる
		m_isDeleteRequst = true;
	}
	//削除リクエストが来ているときに削除する
	if (m_isDeleteRequst == true)
	{
		DeleteGO(p_clearBGM);
		DeleteGO(this);
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
}