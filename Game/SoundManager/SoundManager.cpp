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
	// ポーズ画面の音量設定クラスを取得
	m_soundPause = FindGO<SoundPause>("soundpause");

	// まだ存在していなければ何もしない
	if (m_soundPause == nullptr)
	{
		return;
	}

	// ポーズ画面で設定された音量を取得
	m_bgmVolume = m_soundPause->GetBGMVolume();
	m_seVolume = m_soundPause->GetSEVolume();
}

SoundSource* SoundManager::PlayingBGM(SoundBGM number, bool isLoop)
{
	// 新しく音を鳴らすためのオブジェクトを作成
	SoundSource* bgm = NewGO<SoundSource>(0);

	// どのBGMを鳴らすか設定
	bgm->Init(number);

	bgm->SetVolume(m_bgmVolume);
	// ループ再生するかどうかを指定して再生
	bgm->Play(isLoop);

	// 作成した音を返す
	return bgm;
}

SoundSource* SoundManager::PlayingSE(SoundSE number, bool isLoop)
{
	// 新しく音を鳴らすオブジェクトを作成
	SoundSource* se = NewGO<SoundSource>(0);

	// SEはBGMの後ろに登録されているのでオフセットを足す
	se->Init(number + m_bgmCount);

	// ★ここで音量設定
	se->SetVolume(m_seVolume);

	// 再生
	se->Play(isLoop);

	return se;
}

