#include "stdafx.h"
#include "SoundManager/SoundManager.h"
#include "GameOver.h"
#include"Game.h";
#include"Title.h";
namespace
{
	const char* FILEPATH = "Assets/sprite/GameTransition/GameOver.dds";
	const int WIDTH = 1920.0f;
	const int HIGHT = 1080.0f;
}

GameOver::GameOver()
{

}

GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	//画像を読み込む
	m_GameOverspriteRender.Init(FILEPATH, WIDTH, HIGHT);

	m_deathSound = FindGO<SoundManager>("soundmanager");
	p_deathBGM = m_deathSound->PlayingBGM(SoundBGM::enGameOverBGM, false);

	return true;
}


void GameOver::Update()
{
	//GameOver時に行う処理
	InGameOver();
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
void GameOver::Render(RenderContext& rc)
{
	m_GameOverspriteRender.Draw(rc);
}


