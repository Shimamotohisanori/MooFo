#include "stdafx.h"
#include "CountDown.h"
#include "SoundManager/SoundManager.h"

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

	/** カウントダウンのフェードインにかかる時間 */
	constexpr float FADE_TIME = 1.0f;
}

CountDown::~CountDown()
{
	/** カウントダウンの音を削除 */
	//DeleteGO(m_countDownSE);
}

bool CountDown::Start()
{
	m_countDown1.Init(FILEPATH1, GAMECLEAR_WIDTH, GAMEOVER_HIGHT);
	m_countDown2.Init(FILEPATH2, GAMECLEAR_WIDTH, GAMEOVER_HIGHT);
	m_countDown3.Init(FILEPATH3, GAMECLEAR_WIDTH, GAMEOVER_HIGHT);
	m_countDownStart.Init(FILEPATHStart, StartWIDTH, StartHIGHT);

	SoundManager* soundManager = FindGO<SoundManager>("soundmanager");

	/** カウントダウンの音を再生 */
	m_countDownSE = soundManager->PlayingSE(SoundSE::enCountDownSE, false);

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
	m_countDownTime -= g_gameTime->GetFrameDeltaTime();

	//最初は全てOFF
	m_Show1 = false;
	m_Show2 = false;
	m_Show3 = false;
	m_ShowStart = false;
	
    if (m_countDownTime > 3.0f)
	{
		m_isCountDown = true;
		m_Show3 = true;

	}
	
	 else if (m_countDownTime > 2.0f)
	{
		m_isCountDown = true;
		m_Show2 = true;
	}
	
	else if (m_countDownTime > 1.0f)
	{
		 m_isCountDown = true;
		 m_Show1 = true;
	}
	
	else if (m_countDownTime > 0.0f)
	{
		 m_ShowStart = true;
	}

	//カウントダウン終了
	else
	{
		m_isCountDown = false;
	}

	/** カウントダウンのフェードイン処理 */
	/** カウントダウンの残り時間を1秒で割った余りを取得して、0から1の範囲にする */
	float localTime = fmodf(m_countDownTime, 1.0f);

	/** カウントダウンの残り時間が0から1の範囲にあるときだけフェードイン処理を行う */
	/** α値は1からフェードインの進行度に応じて0に近づいていく */
	m_countDownAlpha = 1.0f - ( localTime / FADE_TIME);	

	/** カウントダウンのスケール処理 */
	/** この式はスケールの初期スケール + フェードインの進行度に応じたスケールの変化量 * α値 */
	m_countDownScale = 0.5f + (localTime / FADE_TIME) * m_countDownAlpha;

	m_countDown1.SetScale(Vector3(m_countDownScale, m_countDownScale, 1.0f));
	m_countDown2.SetScale(Vector3(m_countDownScale, m_countDownScale, 1.0f));
	m_countDown3.SetScale(Vector3(m_countDownScale, m_countDownScale, 1.0f));
	m_countDownStart.SetScale(Vector3(m_countDownScale, m_countDownScale, 1.0f));

	m_countDown1.Update();
	m_countDown2.Update();
	m_countDown3.Update();
	m_countDownStart.Update();
}


void CountDown::Render(RenderContext& rc)
{

	if (m_isCountDown)
	{
		if (m_Show3)
		{
			m_countDown3.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_countDownAlpha));
			m_countDown3.Draw(rc);
		}

		if (m_Show2)
		{
			m_countDown2.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_countDownAlpha));
			m_countDown2.Draw(rc);
		}

		if (m_Show1)
		{
			m_countDown1.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_countDownAlpha));
			m_countDown1.Draw(rc);
		}

		if (m_ShowStart)
		{
			m_countDownStart.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_countDownAlpha));
			m_countDownStart.Draw(rc);
		}
	}
 	
}


