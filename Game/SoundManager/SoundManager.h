#pragma once
#include "sound/SoundSource.h"
class SoundPause;

enum SoundBGM
{
	enTitleBGM,
	enInGameBGM,
	enGameClearBGM,
	enGameOverBGM,
	enGameLoadingBGM,
	enBGMNum
};

enum SoundSE
{
	enChoiceSE,
	enCowCatchSE,
	enCowCrySE,
	enDecisionSE,
	enThrowRopeSE,
	enUFOFlyingSE,
	enUFOTakeAwaySE,
	enWalkSE,
	enScoreDecreaseSE,
	enScoreRiseSE,
	enAddTimeSE,
	enCountDownSE,
	enTimeOutSE,
	enRescueCowSE,
	enSENum
};
class SoundManager : public IGameObject
{
public:
	SoundManager();
	~SoundManager();
	void Update();

	void SetBGMVolume(int number);

	void SetSEVolume(int number);

	float m_bgmVolume = 0.4f;
	float m_seVolume  = 0.6f;

	int m_bgmCount = 0;

	SoundPause* m_soundPause;

	SoundSource* PlayingBGM(SoundBGM number, bool isLoop = true);
	SoundSource* PlayingSE(SoundSE number, bool isLoop = true);

};

