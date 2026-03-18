#include "stdafx.h"
#include "GameClear.h"
#include"Game.h"
#include"Title.h"

namespace
{
	const char* FILEPATH = "Assets/sprite/GameTransition/GameClear.dds";
	const int WIDTH = 1920;
	const int HIGHT = 1080;
}

GameClear::GameClear()
{

}


GameClear::~GameClear()
{

}


bool GameClear::Start()
{
	m_GameClearSpriteRender.Init(FILEPATH, WIDTH, HIGHT);
	return true;
}

void GameClear::Update()
{
	InGameClear();
}


void GameClear::InGameClear()
{
	if (g_pad[0]->IsTrigger(enButtonA))
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
		DeleteGO(this);
	}
}
void GameClear::Render(RenderContext& rc)
{
	m_GameClearSpriteRender.Draw(rc);
}