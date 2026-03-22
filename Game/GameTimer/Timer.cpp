#include "stdafx.h"
#include "Timer.h"
#include"CountDown/CountDown.h"
#include"Pause/Pause.h"
Timer::Timer()
{

}


Timer::~Timer()
{

}


bool Timer::Start()
{
	m_countdown = FindGO<CountDown>("countdown");
	m_pause = FindGO<Pause>("pause");

	return true;
}

void Timer::Update()
{

	if (m_pause->GetIsPause())
	{
		return;
	}

	TextTimer();
}


void Timer::TextTimer()
{
	//カウントダウン中は制限時間を減らさないようにするため早期リターンをする。
	if (m_countdown->GetCountDown())
	{
		return;
	}
  
	m_timer -= g_gameTime->GetFrameDeltaTime();
	//タイマーのテキスト
	uint8_t second = (uint8_t)m_timer % 60;//1の位
	wchar_t timerText[256];
	swprintf_s(timerText, 256, L"%03d",second);
	//タイマーのテキストをFontRenderにセット
	m_TimerFontRender.SetText(timerText);

	//座標を設定
	m_TimerFontRender.SetPosition({-100.0f,530.0f,0.0f });

	//文字の大きさ
	m_TimerFontRender.SetScale(2.5f);

	//文字の色
	m_TimerFontRender.SetColor(g_vec4White);
}


void Timer::AddTimer(float time)
{
	m_timer += time;
}
void Timer::Render(RenderContext& rc)
{
	//カウントダウン中は制限時間の描画を止めるために早期リターンをする。
	if (m_countdown->GetCountDown())
	{
		return;
	}

	m_TimerFontRender.Draw(rc);
}


