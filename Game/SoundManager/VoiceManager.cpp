#include "stdafx.h"
#include "VoiceManager.h"
#include "SoundManager.h"
#include "sound/SoundEngine.h"

namespace
{
	/** ボイス関連の定数をnamespace内にまとめる */
	/** ボイスのファイルパス */
	const char* VOICE_FILEPATH = "Assets/Voice/";

	/** ボイスのファイル形式 */
	const char* SOUND_FILE_FORMAT = ".wav";

	/** ボイスのファイル名一覧 */
	const char* VOICE_FILENAME_LIST[enVoice_Num] =
	{
		"catchVoice",
		"catchVoice2",
		"putHayVoice",
		"rescueVoice",
		"rescueVoice2",
		"throwVoice",
		"throwVoice2"
	};
}

VoiceManager::~VoiceManager()
{}

void VoiceManager::Update()
{}

void VoiceManager::Init(SoundManager* soundManager)
{
	/** SoundManagerに「今まで何個IDを使ったか」を聞く */
	m_voiceOffset = soundManager->GetTotalSoundCount();

	/** ボイスファイルパスを作って登録する。 */
	for (int i = 0; i < enVoice_Num; i++)
	{
		std::string voiceFile = std::string(VOICE_FILEPATH) + VOICE_FILENAME_LIST[i] + SOUND_FILE_FORMAT;
		g_soundEngine->ResistWaveFileBank(i + m_voiceOffset, voiceFile.c_str());
	}
}

SoundSource* VoiceManager::PlayingVoice(SoundVoice number, bool isLoop)
{
	/** 新しく音を鳴らすためのオブジェクトを作成 */
	SoundSource* voice = NewGO<SoundSource>(0);
	/** どのボイスを鳴らすか設定 */
	voice->Init(number + m_voiceOffset);
	/** 音量を設定 */
	voice->SetVolume(m_voiceVolume);
	/** ループ再生するかどうかを指定して再生 */
	voice->Play(isLoop);
	/** 作成した音を返す */
	return voice;
}