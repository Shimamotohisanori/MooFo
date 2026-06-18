#pragma once
class CountDown;
class Pause;
class Game;
class FadeManager;
/** タイマークラス */
class Timer : public IGameObject
{
	public:
	Timer();
	~Timer();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** コンボで時間を増やす処理 */
	void AddTimer(float time);

	/** タイマーの値を取得する関数 */
	const float GetTimer() const
	{
		return m_timer;
	}


public:
	/** タイマーの処理 */
	void TextTimer();


private:
	/** 制限時間 */
	float m_timer = 40.0f;
	/** 点滅タイマー*/
	float m_flashTimer = 1.0f;
	/** フェード速度*/
	float m_FadeSpeed = 0.8f;

	float m_FadeTimerAlpha = 1.0f;

	

	/** カウントアニメーション用タイマー*/
	float m_animTimer = 0.0f;
	/** α値*/
	float m_animAlpha = 1.0f;
	/** スケール*/
	float m_animScale = 0.8f;

	/**点滅中の数字インデックス*/ 
	int   m_flashDigits[3] = { 0, 0, 0 }; 
	
	/** 前のフレームの時間 */
	int m_prevTime = -1;

	/** 三桁(百・十・一)を描画するスプライト */
	SpriteRender m_digitSprite[3][10];

	SpriteRender m_redDigitSprite[3][10];
	
	/** カウントダウン */
	CountDown* m_countdown = nullptr;
	/** サウンドマネージャーのポインタ*/
	SoundSource* m_TimerSE = nullptr;
	/** ポーズ */
	Pause* m_pause = nullptr;
	/** ゲームのポインタ*/
	Game* m_game = nullptr;
	/** 点滅しているかどうかのフラグ*/
	bool  m_isFlash = false;

	/** カウントアニメーションしているかどうかのフラグ*/
	bool m_isAnim = false;
	/** タイマーの残り秒数が５秒になっているかどうかのフラグ*/
	bool m_isFiveTimer = false;

	/** カウントタイマーが終わったかどうかのフラグ*/
	bool m_isEndCountTimer = false;
	/** ５秒前に再生するSEが流れたかどうかのフラグ*/
	bool m_isPlaySE = false;
	/** フェードインしているかどうかのフラグ*/
	bool m_isFadeIn = false;
};

