#include "stdafx.h"
#include "Timer.h"
#include "CountDown/CountDown.h"
#include "Pause/Pause.h"

namespace
{
	/** 三桁を描画する画像の大きさ */
	const Vector3 DIGIT_SCALE = {0.0f,0.0f, 0.0f};
}

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
	//全ての画像スプライト(30個)を初期化
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			m_digitSprite[i][j].Init(m_digitPaths[j], 128, 128);
			// 非表示
			m_digitSprite[i][j].SetScale(DIGIT_SCALE);
		}
		
	}
	return true;
}

void Timer::Update()
{

	if (m_pause->GetIsPause())
	{
		return;
	}

	TextTimer();

	//Update() を呼ばないとDrawしても正しく表示されない
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_digitSprite[i][j].Update();
		}
	}
}


void Timer::TextTimer()
{
	//カウントダウン中は制限時間を減らさないようにするため早期リターンをする。
	if (m_countdown->GetCountDown())
	{
		return;
	}
	//フレームごとに時間を減らす
	m_timer -= g_gameTime->GetFrameDeltaTime();
	//小数点の切り捨て
	int time = (int)m_timer;
	//0以下にならないようにする
	if (time < 0)
	{
		time = 0;
	}
	//秒数が変わっていなければ何もしない
	if (time != m_prevTime)
	{
		m_prevTime = time;
		//桁分解
		int hundreds = time / 100;//(例)123÷100 = 1
		int tens = (time / 10) % 10;//(例)(123÷10) %10 = 12 %10 =1あまり2 
		int ones = time % 10;//(例)123÷10 = 12あまり3
		//全部非表示
		for (int i = 0; i < 10; i++)
		{
			m_digitSprite[0][i].SetScale({ 0,0,0, });
			m_digitSprite[1][i].SetScale({ 0,0,0, });
			m_digitSprite[2][i].SetScale({ 0,0,0, });
		}

		// 必要な数字だけ表示 該当する数字だけON
		m_digitSprite[0][hundreds].SetScale({ 1,1,1 });
		m_digitSprite[1][tens].SetScale({ 1,1,1 });
		m_digitSprite[2][ones].SetScale({ 1,1,1 });
		//位置設定
		m_digitSprite[0][hundreds].SetPosition({ -100.0f,480.0f,0.0f });
		m_digitSprite[1][tens].SetPosition({ 0.0f,480.0f,0.0f });
		m_digitSprite[2][ones].SetPosition({ 100.0f,480.0f,0.0f });
	}
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

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_digitSprite[i][j].Draw(rc);
		}
	}
	
}


