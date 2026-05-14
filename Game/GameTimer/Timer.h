#pragma once
class CountDown;
class Pause;
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
	const float GetTimer() const { return m_timer; }


public:
	/** タイマーの処理 */
	void TextTimer();


private:
	/** 制限時間 */
	float m_timer = 120.0f;

	/** 前のフレームの時間 */
	uint8_t m_prevTime = -1;

	/** 三桁(百・十・一)を描画するスプライト */
	SpriteRender m_digitSprite[3][10];
	
	/** 数字の画像をロード */
	const char* m_digitPaths[10] =
	{
		"Assets/sprite/NumberUI/MooFoNumberUI0.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI1.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI2.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI3.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI4.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI5.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI6.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI7.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI8.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI9.dds",
	};

	/** カウントダウン */
	CountDown* m_countdown;

	/** ポーズ */
	Pause* m_pause;
};

