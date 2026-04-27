#pragma once
class CountDown;
class Pause;
class Timer : public IGameObject
{
	public:
	Timer();
	~Timer();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//コンボで時間を増やす処理
	void AddTimer(float time);
	const float GetTimer() const { return m_timer; }
public:
	//タイマーの処理
	void TextTimer();

private:
	//制限時間スプライト
	float m_timer = 10.0f;//制限時間

	int m_prevTime = -1;
	//三桁(百・十・一)を描画するスプライト
	SpriteRender m_digitSprite[3][10];
	//数字の画像をロード
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

	CountDown* m_countdown;//カウントダウン
	Pause* m_pause;//ポーズ
};

