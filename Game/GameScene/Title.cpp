#include "stdafx.h"
#include "SoundManager/SoundManager.h"
#include "Title.h"
#include "Game.h"
#include"LoadingScene.h"

namespace
{
	const char* GAMECLEAR_FILEPATH = "Assets/sprite/GameTransition/GameTitle.dds";
	const int GAMECLEAR_WIDTH = 1920;
	const int GAMEOVER_HIGHT = 1080;
}
Title::Title()
{

}
		

Title::~Title()
{

}

bool Title::Start()
{
	m_TitleSpriteRender.Init(GAMECLEAR_FILEPATH,GAMECLEAR_WIDTH,GAMEOVER_HIGHT);

	m_soundManager = FindGO<SoundManager>("soundmanager");
	p_titleBGM = m_soundManager->PlayingBGM(SoundBGM::enTitleBGM, false);

	return true;
}


void Title::Update()
{
	InTitle();

	m_TitleSpriteRender.Update();
}


void Title::InTitle()
{
	if (g_pad[0]->IsPressAnyKey())
	{
		m_loadingScene = NewGO<LoadingScene>(0, "loading");
		p_titleSE = m_soundManager->PlayingSE(SoundSE::enDecisionSE, false);
		DeleteGO(p_titleBGM);
		m_loadingScene->SetNextScene([]()
			{
				NewGO<Game>(0, "game");
			});
		DeleteGO(this);
	}
}

void Title::Render(RenderContext& rc)
{
	m_TitleSpriteRender.Draw(rc);
}

