#pragma once
#include "sound/SoundSource.h"
class SoundPause;

enum SoundBGM
{
	/** タイトルBGM*/
	enTitleBGM,
	/** インゲームBGM*/
	enInGameBGM,
	/** ゲームクリア時のBGM*/
	enGameClearBGM,
	/** ゲームオーバー時のBGM*/
	enGameOverBGM,
	/** ローディング中のBGM*/
	enGameLoadingBGM,
	enBGMNum
};

enum SoundSE
{
	enChoiceSE,/** 選択する時のSE*/
	enCowCatchSE,/** 牛をロープで捕まえるときのSE*/
	enCowCrySE,/** 牛をロープで引っ張ったときに鳴く鳴き声SE*/
	enDecisionSE,/** 決定のSE*/
	enThrowRopeSE,/** ロープを投げるときのSE*/
	enUFOCaptureSE,/** UFOが光を出すときのSE*/
	enScoreDecreaseSE,/** スコアが減少した時に流すSE*/
	enScoreRiseSE,/** スコアが上昇する時に流すSE*/
	enAddTimeSE,/** タイマーが増えた時に流すSE*/
	enCountDownSE,/** カウントダウンの時に流すSE*/
	enTimeOutSE,/** タイムアウトの時に流すSE*/
	enRescueCowSE,/** 牛を救出出来たときに流すSE*/
	enRopePullSE,/** ロープを引っ張るときに流すSE*/
	enRunSE,/** プレイヤーが走る時に流すSE*/
	enPauseSE,/* Pauseになったら流すSE**/
	enPuthaySE,/** 牛の餌を置くときのSE*/
	enTakehaySE,/** 牛の餌を取るときのSE*/
	enTimerEndFive,/** 制限時間が残り５秒の時に流すSE*/
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
	SoundPause* m_soundPause = nullptr;

	/** BGMを再生する関数 */
	SoundSource* PlayingBGM(SoundBGM number, bool isLoop = true);

	/** SEを再生する関数 */
	SoundSource* PlayingSE(SoundSE number, bool isLoop = true);

};

