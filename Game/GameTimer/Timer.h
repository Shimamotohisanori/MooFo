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
	public:
		//コンボで時間を増やす処理
	void AddTimer(float time);
	const float GetTimer() const { return m_timer; }
public:
	//タイマーの処理
	void TextTimer();

private:
	FontRender m_TimerFontRender;//タイマーのフォントレンダー
	float m_timer = 30.0f;//制限時間

	CountDown* m_countdown;//カウントダウン
	Pause* m_pause;//ポーズ
};

