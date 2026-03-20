#include "stdafx.h"
#include "CountDown.h"
#include"Source/Actor/Character/Player/Player.h"




bool CountDown::Start()
{
	return true;
}

void CountDown::Update()
{

	if (m_isCountDown)
	{
		InCountDown();
	}
	
}

void CountDown::InCountDown()
{
	m_CountDownTime -= g_gameTime->GetFrameDeltaTime();

	if (m_CountDownTime > 1.0f)
	{

		m_isCountDown = true;

		int number = static_cast<int>(m_CountDownTime);
		wchar_t countText[16];
		//swprintf_sで文字列にして画面に表示している。
		swprintf_s(countText, 16, L"%d", number);
		m_CountDownFontRender.SetText(countText);
		m_CountDownFontRender.SetScale(3.0f);
		m_CountDownFontRender.SetPosition(-70.0f, 130.0f, 0.0f);
		
	}
	//カウントダウンが1秒から0秒の間でSTARTを画面に表示する
	else if (m_CountDownTime > 0.0f)
	{

		m_isCountDown = true;

		m_CountDownFontRender.SetText(L"START!");
		m_CountDownFontRender.SetScale(3.5f);
		m_CountDownFontRender.SetPosition(-70.0f, 130.0f, 0.0f);
	}

	else 
	{
		//カウントダウン終了
		m_isCountDown = false;
		//表示をクリア
		m_CountDownFontRender.SetText(L"");
	}
}


void CountDown::Render(RenderContext& rc)
{
	if (m_isCountDown)
	{
		m_CountDownFontRender.Draw(rc);
	}
 	
}


