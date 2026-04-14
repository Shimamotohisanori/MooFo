#include "stdafx.h"
#include "SoundManager/SoundManager.h"
#include "Title.h"
#include "Game.h"

namespace
{
	const char* FILEPATH = "Assets/sprite/GameTransition/GameTitle.dds";
	const int WIDTH = 1920;
	const int HIGHT = 1080;
}
Title::Title()
{

}
		

Title::~Title()
{

}

bool Title::Start()
{
	m_TitleSpriteRender.Init(FILEPATH,WIDTH,HIGHT);

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
		NewGO<Game>(0, "game");
		p_titleSE = m_soundManager->PlayingSE(SoundSE::enDecisionSE, false);
		DeleteGO(p_titleBGM);
		DeleteGO(this);
	}
}

void Title::Render(RenderContext& rc)
{
	m_TitleSpriteRender.Draw(rc);
}

