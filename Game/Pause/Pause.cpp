#include "stdafx.h"
#include "Pause/Pause.h"
#include "Game.h"
#include "SoundManager/SoundManager.h"
//#include "Title.h"
namespace
{
	//const std::string PAUSE_SPRITE_FILE_PASS = "Assets/sptite/";
	const Vector3 QUITEPOS = { 0.0f,-100.0f,0.0f };
	const Vector3 RESUMEPOS = { 0.0f,150.0f,0.0f };
	const Vector3 SOUNDPOS = { 0.0f,-300.0f,0.0f };
	const Vector3 ARROWPOS = { -350.0f,150.0f,0.0f };
	const Vector3 SECONDARROWPOS = { -350.0f,-105.0f,0.0f };
	const Vector3 THIRDARROWPOS = { -170.0f,-300.0f,0.0f };
	const Vector3 ARROWSCALE = { 1.0f,1.0f,1.0f };
	const Vector3 ARROWSOUNDSCALE = { 0.5f,0.5f,1.0f };
}
bool Pause::Start()
{
	m_pauseBackGround.Init("Assets/pauseBackGround.dds",1980.0f,1080.0f);
	m_pauseBackGround.Update();

	m_quiteSprite.Init("Assets/quiteGame.dds",750.0f,650.0f);
	m_quiteSprite.SetPosition(QUITEPOS);
	m_quiteSprite.Update();

	m_resumeSprite.Init("Assets/resumeGame.dds",650.0f,550.0f);
	m_resumeSprite.SetPosition(RESUMEPOS);
	m_resumeSprite.Update();

	m_soundSprite.Init("Assets/sound.dds",350.0f,250.0f);
	m_soundSprite.SetPosition(SOUNDPOS);
	m_soundSprite.Update();

	m_arrowSprite.Init("Assets/Arrow.dds", 150.0f, 150.0f);

	//m_settingSprite.Init("Assets/setting.dds",300.0f,200.0f);
	//m_bgmSprite.Init("Assets/BGM.dds",100.0f,150.0f);
	//m_sePrite.Init("Assets/SE.dds",100.0f,150.0f);
	//m_volumeSprite.Init("Assets/volume.dds",100.0f,100.0f);
	//m_notVolumeSprite.Init("Assets/notVolume.dds",100.0f,100.0f);

	return true;
}

void Pause::Update()
{
	Choose();
	Select();
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		m_countNumber++;
	}

}

void Pause::Choose()
{
	if (m_countNumber == 0)
	{
		m_arrowSprite.SetPosition(ARROWPOS);
		m_arrowSprite.SetScale(ARROWSCALE);
	}
	else if (m_countNumber == 1)
	{
		m_arrowSprite.SetPosition(SECONDARROWPOS);
	}
	else if (m_countNumber == 2)
	{
		m_arrowSprite.SetPosition(THIRDARROWPOS);
		m_arrowSprite.SetScale(ARROWSOUNDSCALE);
	}
	else if (m_countNumber == 3)
	{
		m_countNumber = 0;
	}
	m_arrowSprite.Update();
}

void Pause::Select()
{
	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		if (m_countNumber == 0)
		{

		}
		if (m_countNumber == 1)
		{

		}
		if (m_countNumber == 2)
		{
			NewGO<SoundManager>(0, "soundmanager");
			DeleteGO(this);
		}
	}
}

void Pause::Render(RenderContext& rc)
{
	m_pauseBackGround.Draw(rc);
	m_quiteSprite.Draw(rc);
	m_resumeSprite.Draw(rc);
	m_soundSprite.Draw(rc);
	m_arrowSprite.Draw(rc);
}