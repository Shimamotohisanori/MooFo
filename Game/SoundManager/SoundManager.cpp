#include "stdafx.h"
#include "SoundManager.h"
#include "sound/SoundEngine.h"
#include "Pause/SoundPause.h"
namespace
{
	/** �t�@�C���p�X��namespace����� */
	const char* BGM_FILEPATH = "Assets/BGM/";
	const char* SE_FILEPATH = "Assets/SE/";
	const char* SOUND_FILE_FORMAT = ".wav";
	const char* BGM_FILENAME_LIST[enBGMNum] =
	{
		"TitleBGM",
		"InGameBGM",
		"GameClearBGM",
		"GameOverBGM"
	};

	const char* SE_FILENAME_LIST[enSENum] =
	{
		"ChoiceSE",
		"CowCatchSE",
		"CowCrySE",
		"DecisionSE",
		"ThrowRopeSE",
		"UFOFlyingSE",
		"UFOTakeAwaySE",
		"WalkSE"
	};
}

SoundManager::SoundManager()
{

	m_bgmCount = enBGMNum;
	for (int i = 0; i < enBGMNum; i++)
	{
		std::string bgmFile = std::string(BGM_FILEPATH) + BGM_FILENAME_LIST[i] + (SOUND_FILE_FORMAT);
		const char* bgmFilePath = bgmFile.c_str();
		g_soundEngine->ResistWaveFileBank(i, bgmFilePath);
	}

	for (int i = 0; i < enSENum; i++)
	{
		std::string seFile = std::string(SE_FILEPATH) + SE_FILENAME_LIST[i] + (SOUND_FILE_FORMAT);
		const char* seFilePath = seFile.c_str();
		g_soundEngine->ResistWaveFileBank(i + m_bgmCount, seFilePath);
		
	}
}

SoundManager::~SoundManager()
{
}

void SoundManager::Update()
{
	m_soundPause = FindGO<SoundPause>("soundpause");

	if (m_soundPause == nullptr)
	{
		return;
	}
	m_bgmVolume = m_soundPause->GetBGMVolume();
	m_seVolume = m_soundPause->GetSEVolume();

}

SoundSource* SoundManager::PlayingBGM(SoundBGM number, bool isLoop)
{
	SoundSource* bgm = NewGO<SoundSource>(0);
	bgm->Init(number);
	bgm->Play(isLoop);

	return bgm;
}

SoundSource* SoundManager::PlayingSE(SoundSE number, bool isLoop)
{
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(number + m_bgmCount);
	se->Play(isLoop);

	return se;
}

