#include "stdafx.h"
#include "SoundManager.h"
#include "Pause/Pause.h"
namespace
{
	const Vector3 RETURNPOS = { 0.0f,-300.0f,0.0f };
	const Vector3 BLACKPOS = { 0.0f,-295.0f,0.0f };
	const Vector3 VOLUMEPOS = { 480.0f,75.0f,0.0f };
	const Vector3 NOT_VOLUMEPOS = { -480.0f,75.0f,0.0f };
	const Vector3 SE_VOLUMEPOS = { 480.0f,-120.0f,0.0f };
	const Vector3 SE_NOT_VOLUMEPOS = { -480.0f,-120.0f,0.0f };
	const Vector3 BGMPOS = { 0.0f,170.0f,0.0f };
	const Vector3 SEPOS = { 0.0f,-20.0f,0.0f };
	const Vector3 SOUNDICONPOS = { -380.0f,75.0f,0.0f };
	const Vector3 SE_ICONPOS = { -380.0f,-120.0f,0.0f };
	const Vector3 BLOWNBARPOS = { -395.0f,75.0f,0.0f };
	const Vector3 SE_BLOWNBARPOS = { -395.0f,-122.0f,0.0f };
	const Vector3 BLOWNBARINITIALSCALE = { 0.0f,0.0f,0.0f };
	const Vector2 BLOWN_BAR = { 0.0f,0.5 };
}

bool SoundManager::Start()
{
	m_backGroundSprite.Init("Assets/pauseBackGround.dds", 1980.0f, 1080.0f);
	m_backGroundSprite.Update();

	m_settingSprite.Init("Assets/setting.dds", 1200.0f, 500.0f);
	m_settingSprite.Update();

	m_buttonReturnSprite.Init("Assets/buttonReturn.dds", 450.0f, 200.0f);
	m_buttonReturnSprite.SetPosition(RETURNPOS);
	m_buttonReturnSprite.Update();

	m_volumeSprite.Init("Assets/volume.dds", 230.0f, 230.0f);
	m_volumeSprite.SetPosition(VOLUMEPOS);
	m_volumeSprite.Update();

	m_notVolumeSprite.Init("Assets/notVolume.dds", 245.0f, 245.0f);
	m_notVolumeSprite.SetPosition(NOT_VOLUMEPOS);
	m_notVolumeSprite.Update();

	m_seVolumeSprite.Init("Assets/volume.dds", 230.0f, 230.0f);
	m_seVolumeSprite.SetPosition(SE_VOLUMEPOS);
	m_seVolumeSprite.Update();

	m_seNotVolumeSprite.Init("Assets/notVolume.dds", 245.0f, 245.0f);
	m_seNotVolumeSprite.SetPosition(SE_NOT_VOLUMEPOS);
	m_seNotVolumeSprite.Update();

	m_bgmSprite.Init("Assets/BGM.dds", 250.0f, 300.0f);
	m_bgmSprite.SetPosition(BGMPOS);
	m_bgmSprite.Update();

	m_seSprite.Init("Assets/SE.dds", 250.0f, 300.0f);
	m_seSprite.SetPosition(SEPOS);
	m_seSprite.Update();

	m_soundIconSprite.Init("Assets/soundIcon.dds", 290.0f, 240.0f);
	m_soundIconSprite.SetPosition(SOUNDICONPOS);
	m_soundIconSprite.Update();

	m_seIconSprite.Init("Assets/seIcon.dds", 290.0f, 240.0f);
	m_seIconSprite.SetPosition(SE_ICONPOS);
	m_seIconSprite.Update();

	m_blownBarSprite.Init("Assets/BlownBar.dds", 790.0f, 220.0f);
	m_blownBarSprite.SetPosition(BLOWNBARPOS);
	m_blownBarSprite.SetPivot(BLOWN_BAR);
	m_blownBarSprite.SetScale(BLOWNBARINITIALSCALE);
	m_blownBarSprite.Update();

	m_seBlownBarSprite.Init("Assets/BlownBar.dds", 790.0f, 220.0f);
	m_seBlownBarSprite.SetPosition(SE_BLOWNBARPOS);
	m_seBlownBarSprite.SetPivot(BLOWN_BAR);
	m_seBlownBarSprite.SetScale(BLOWNBARINITIALSCALE);
	m_seBlownBarSprite.Update();

	m_bgmBlackIcon.Init("Assets/blackIcon.dds", 290.0f, 240.0f);
	m_bgmBlackIcon.SetPosition(SOUNDICONPOS);
	m_bgmBlackIcon.Update();

	m_seBlackIcon.Init("Assets/blackIcon.dds", 290.0f, 240.0f);
	m_seBlackIcon.SetPosition(SE_ICONPOS);
	m_seBlackIcon.Update();

	m_blackSprite.Init("Assets/Black.dds", 430.0f, 240.0f);
	m_blackSprite.SetPosition(BLACKPOS);
	m_blackSprite.Update();

	m_pause = FindGO<Pause>("pause");

	return true;
}

void SoundManager::Update()
{
	ButtonCount();
}

void SoundManager::ButtonCount()
{
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		m_Count++;
	}

	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		m_Count--;
	}

	if (m_Count == -1)
	{
		m_Count = 2;
	}

	if (m_Count == 0)
	{
		Barbgm();
	}

	if (m_Count == 1)
	{
		Barse();
	}

	if (m_Count == 2)
	{
		if (g_pad[0]->IsTrigger(enButtonStart))
		{
			m_pause->Activate();
			m_pause->SetCountNumber(0);
			Deactivate();
		}
	}

	if (m_Count == 3)
	{
		m_Count = 0;
	}
}

void SoundManager::Barbgm()
{
	if (m_bgmVolume.x < 1.0f)
	{
		if (g_pad[0]->IsPress(enButtonRight))
		{
			m_bgmVolume.x += 0.01f;
			m_soundIconSprite.SetPosition(Vector3(-380.0f + m_bgmVolume.x * 750.0f, 75.0f, 0.0f));
			m_soundIconSprite.Update();
			m_blownBarSprite.SetScale(Vector3(m_bgmVolume.x, 1.0f, 0.0f));
			m_blownBarSprite.Update();
			m_bgmBlackIcon.SetPosition(Vector3(-380.0f + m_bgmVolume.x * 750.0f, 75.0f, 0.0f));
			m_bgmBlackIcon.Update();
		}
	}

	if (m_bgmVolume.x > 0.001f)
	{
		if (g_pad[0]->IsPress(enButtonLeft))
		{
			m_bgmVolume.x -= 0.01f;
			m_soundIconSprite.SetPosition(Vector3(-380.0f + m_bgmVolume.x * 750.0f, 75.0f, 0.0f));
			m_soundIconSprite.Update();
			m_blownBarSprite.SetScale(Vector3(m_bgmVolume.x, 1.0f, 0.0f));
			m_blownBarSprite.Update();
			m_bgmBlackIcon.SetPosition(Vector3(-380.0f + m_bgmVolume.x * 750.0f, 75.0f, 0.0f));
			m_bgmBlackIcon.Update();
		}
	}
}

void SoundManager::Barse()
{
	if (m_seVolume.x < 1.0f)
	{
		if (g_pad[0]->IsPress(enButtonRight))
		{
			m_seVolume.x += 0.01f;
			m_seIconSprite.SetPosition(Vector3(-380.0f + m_seVolume.x * 750.0f, -120.0f, 0.0f));
			m_seIconSprite.Update();
			m_seBlownBarSprite.SetScale(Vector3(m_seVolume.x, 1.0f, 0.0f));
			m_seBlownBarSprite.Update();
			m_seBlackIcon.SetPosition(Vector3(-380.0f + m_seVolume.x * 750.0f, -120.0f, 0.0f));
			m_seBlackIcon.Update();
		}
	}

	if (m_seVolume.x > 0.001f)
	{
		if (g_pad[0]->IsPress(enButtonLeft))
		{
			m_seVolume.x -= 0.01f;
			m_seIconSprite.SetPosition(Vector3(-380.0f + m_seVolume.x * 750.0f, -120.0f, 0.0f));
			m_seIconSprite.Update();
			m_seBlownBarSprite.SetScale(Vector3(m_seVolume.x, 1.0f, 0.0f));
			m_seBlownBarSprite.Update();
			m_seBlackIcon.SetPosition(Vector3(-380.0f + m_seVolume.x * 750.0f, -120.0f, 0.0f));
			m_seBlackIcon.Update();
		}
	}
}

void SoundManager::Render(RenderContext& rc)
{
	m_backGroundSprite.Draw(rc);
	m_settingSprite.Draw(rc);
	m_buttonReturnSprite.Draw(rc);
	m_volumeSprite.Draw(rc);
	m_notVolumeSprite.Draw(rc);
	m_seVolumeSprite.Draw(rc);
	m_seNotVolumeSprite.Draw(rc);
	m_bgmSprite.Draw(rc);
	m_seSprite.Draw(rc);
	
	if (m_bgmVolume.x > 0.05f)
	{
		m_blownBarSprite.Draw(rc);
	}

	if (m_seVolume.x > 0.05f)
	{
		m_seBlownBarSprite.Draw(rc);
	}
	
	if (m_Count == 0)
	{
		m_soundIconSprite.Draw(rc);
	}
	else
	{
		m_bgmBlackIcon.Draw(rc);
	}

	if (m_Count == 1)
	{
		m_seIconSprite.Draw(rc);
	}
	else
	{
		m_seBlackIcon.Draw(rc);
	}

	if (m_Count == 2)
	{
		m_blackSprite.SetMulColor
		(
			Vector4
			(
				1.0f,
				1.0f,
				1.0f,
				0.0f
			)
		);
	}
	else
	{
		m_blackSprite.SetMulColor
		(
			Vector4
			(
				1.0f,
				1.0f,
				1.0f,
				0.5f
			)
		);
	}
	m_blackSprite.Draw(rc);
}
