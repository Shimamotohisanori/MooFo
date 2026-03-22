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
	const float GetTimer() const { return m_timer; }
public:
	//タイマーの処理
	void TextTimer();

private:
	FontRender m_TimerFontRender;//タイマーのフォントレンダー
	float m_timer = 9990.0f;//制限時間

	CountDown* m_countdown;//カウントダウン
	Pause* m_pause;//ポーズ
};

