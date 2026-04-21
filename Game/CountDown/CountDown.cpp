#include "stdafx.h"
#include "CountDown.h"

namespace
{
	//ファイルパス指定
	const char* FILEPATH1 = "Assets/sprite/NumberUI/MooFoNumberUI1.dds";
	const char* FILEPATH2 = "Assets/sprite/NumberUI/MooFoNumberUI2.dds";
	const char* FILEPATH3 = "Assets/sprite/NumberUI/MooFoNumberUI3.dds";
	const char* FILEPATHStart = "Assets/sprite/NumberUI/Start.dds";
	//画像の大きさ
	const int GAMECLEAR_WIDTH = 500.0f;
	const int GAMEOVER_HIGHT = 300.0f;

	const int StartWIDTH = 700.0f;
	const int StartHIGHT = 500.0f;
}

bool CountDown::Start()
{
	m_countDown1.Init(FILEPATH1, GAMECLEAR_WIDTH, GAMEOVER_HIGHT);
	m_countDown2.Init(FILEPATH2, GAMECLEAR_WIDTH, GAMEOVER_HIGHT);
	m_countDown3.Init(FILEPATH3, GAMECLEAR_WIDTH, GAMEOVER_HIGHT);
	m_countDownStart.Init(FILEPATHStart, StartWIDTH, StartHIGHT);
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

	//最初は全てOFF
	m_Show1 = false;
	m_Show2 = false;
	m_Show3 = false;
	m_ShowStart = false;
	
     if (m_CountDownTime > 3.0f)
	{
		m_isCountDown = true;
		m_Show3 = true;

	}
	else if (m_CountDownTime > 2.0f)
	{
		m_isCountDown = true;
		m_Show2 = true;
	}
	else if (m_CountDownTime > 1.0f)
	{
		 m_isCountDown = true;
		 m_Show1 = true;
	}
	else if (m_CountDownTime > 0.0f)
	 {
		 m_ShowStart = true;
	 }
	//カウントダウン終了
	else
	{
		m_isCountDown = false;
	}
}


void CountDown::Render(RenderContext& rc)
{

	if (m_isCountDown)
	{
		if (m_Show3)
		{
			m_countDown3.Draw(rc);
		}

		if (m_Show2)
		{
			m_countDown2.Draw(rc);
		}

		if (m_Show1)
		{
			m_countDown1.Draw(rc);
		}
		if (m_ShowStart)
		{
			m_countDownStart.Draw(rc);
		}
	}
 	
}


