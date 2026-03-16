#include "stdafx.h"
#include "Timer.h"


Timer::Timer()
{

}


Timer::~Timer()
{

}


bool Timer::Start()
{
	return true;
}

void Timer::Update()
{
	TextTimer();
}


void Timer::TextTimer()
{
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

void Timer::Render(RenderContext& rc)
{
	m_TimerFontRender.Draw(rc);
}


