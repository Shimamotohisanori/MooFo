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
	enUFOCaptureSE,
	enUFOTakeAwaySE,
	enWalkSE,
	enScoreDecreaseSE,
	enScoreRiseSE,
	enAddTimeSE,
	enCountDownSE,
	enTimeOutSE,
	enRescueCowSE,
	enRopePullSE,
	enRunSE,
	enPauseSE,
	enSENum
};

/** サウンドマネージャークラス */
class SoundManager : public IGameObject
{
public:
	SoundManager();
	~SoundManager();
	void Update();

	/** BGMを再生する関数 */
	float m_bgmVolume = 0.3f;

	/** SEを再生する関数 */
	float m_seVolume  = 0.8f;

	/** BGMの数 */
	uint8_t m_bgmCount = 0;

	/** ポーズ中にサウンド操作をするクラスのポインタ */
	SoundPause* m_soundPause;

	/** BGMを再生する関数 */
	SoundSource* PlayingBGM(SoundBGM number, bool isLoop = true);

	/** SEを再生する関数 */
	SoundSource* PlayingSE(SoundSE number, bool isLoop = true);

};

