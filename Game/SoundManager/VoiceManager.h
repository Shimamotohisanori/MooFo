#pragma once
#include "sound/SoundSource.h"

class SoundManager;

enum SoundVoice
{
	enVoice_Catch1,/** 牛を捕まえたときのボイス */
	enVoice_Catch2,/** 牛を捕まえたときのボイス */
	enVoice_PutHay,/** 牛の餌を置いたときのボイス */
	enVoice_Rescue1,/** 牛を救出したときのボイス */
	enVoice_Rescue2,/** 牛を救出したときのボイス */
	enVoice_ThrowRope1,/** ロープを投げたときのボイス */
	enVoice_ThrowRope2,/** ロープを投げたときのボイス */
	enVoice_Num/* ボイスの種類の数 */
};

/** ボイスマネージャークラス */
class VoiceManager : public IGameObject
{
public:
	/** コンストラクタ */
	VoiceManager() {};
	~VoiceManager();
	void Update();

	void Init(SoundManager* soundManager);

	SoundSource* PlayingVoice(SoundVoice number, bool isLoop = false);

	/** ボイスの音量を取得 */
	float GetVoiceVolume()
	{
		return m_voiceVolume;
	}

	/** ボイスの音量を設定 */
	void SetVoiceVolume(float voice)
	{
		m_voiceVolume = voice;
	}

private:
	/** ボイスの音量 */
	float m_voiceVolume = 1.0f;

	/** ボイスの登録番号のオフセット */
	uint8_t m_voiceOffset = 0;
};

