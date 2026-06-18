#include "stdafx.h"
#include "Timer.h"
#include "CountDown/CountDown.h"
#include "Pause/Pause.h"
#include"GameScene/Game.h"

namespace
{
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

	const char* m_redDigitPaths[10] =
	{
		"Assets/sprite/NumberUI/Red_MooFoNumberUI0.dds",
		"Assets/sprite/NumberUI/Red_MooFoNumberUI1.dds",
		"Assets/sprite/NumberUI/Red_MooFoNumberUI2.dds",
		"Assets/sprite/NumberUI/Red_MooFoNumberUI3.dds",
		"Assets/sprite/NumberUI/Red_MooFoNumberUI4.dds",
		"Assets/sprite/NumberUI/Red_MooFoNumberUI5.dds",
		"Assets/sprite/NumberUI/Red_MooFoNumberUI6.dds",
		"Assets/sprite/NumberUI/Red_MooFoNumberUI7.dds",
		"Assets/sprite/NumberUI/Red_MooFoNumberUI8.dds",
		"Assets/sprite/NumberUI/Red_MooFoNumberUI9.dds",
	};

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
	m_game = FindGO<Game>("game");
	/** 全ての画像スプライト(30個)を初期化 */
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			m_digitSprite[i][j].Init(m_digitPaths[j], 128, 128);
			m_redDigitSprite[i][j].Init(m_redDigitPaths[j], 128, 128);
			
			/** 非表示 */
			m_digitSprite[i][j].SetScale(DIGIT_SCALE);
			m_redDigitSprite[i][j].SetScale(DIGIT_SCALE);
		}
		
	}
	return true;
}

void Timer::Update()
{
	m_countdown = FindGO<CountDown>("countdown");
	
	m_pause = FindGO<Pause>("pause");
	
	if(m_pause == nullptr || m_countdown == nullptr)
	{
		return;
	}

	/** Pause中は制限時間の更新を止める */
	if (m_pause->GetIsPause())
	{
		return;
	}

	/** タイマーの更新 */
	TextTimer();

	/** 全ての画像スプライト(30個)を更新 */
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_digitSprite[i][j].Update();
		}
	}

	/** 赤の画像スプライトも更新*/
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_redDigitSprite[i][j].Update();
		}
	}
}


void Timer::TextTimer()
{
	/** カウントダウン中は制限時間を減らさないようにするため早期リターンをする。 */
	if (m_countdown->GetCountDown())
	{
		return;
	}

	/** フレームごとに時間を減らす */
	m_timer -= g_gameTime->GetFrameDeltaTime();
	
	/** 小数点の切り捨て */
	int time = (int)m_timer;

	/** 0以下にならないようにする */
	if (time < 0)
	{
		time = 0;
	}

	/** 桁分解 */
		/** (例)123÷100 = 1 */
	int hundreds = time / 100;

	/** (例)(123÷10) %10 = 12 %10 =1あまり2 */
	int tens = (time / 10) % 10;

	/** (例)123÷10 = 12あまり3 */
	int ones = time % 10;

	/** 秒数が変わっていなければ何もしない */
	if (time != m_prevTime)
	{
		m_prevTime = time;
		/** 全部非表示 */
		for (int i = 0; i < 10; i++)
		{
			m_digitSprite[0][i].SetScale({ 0,0,0, });
			m_digitSprite[1][i].SetScale({ 0,0,0, });
			m_digitSprite[2][i].SetScale({ 0,0,0, });

			m_redDigitSprite[0][i].SetScale({ 0,0,0, });
			m_redDigitSprite[1][i].SetScale({ 0,0,0, });
			m_redDigitSprite[2][i].SetScale({ 0,0,0, });

		}
		/** 30秒以下で赤色のタイマーに変える*/
		if (time <= 30)
		{
			/** 赤色のタイマーのみ表示する*/
			m_redDigitSprite[0][hundreds].SetScale({ 0.8f,0.8f,0.8f });
			m_redDigitSprite[1][tens].SetScale({ 0.8f,0.8f,0.8f });
			m_redDigitSprite[2][ones].SetScale({ 0.8f,0.8f,0.8f });

			/** 位置設定 */
			m_redDigitSprite[0][hundreds].SetPosition({ -100.0f,480.0f,0.0f });
			m_redDigitSprite[1][tens].SetPosition({ 0.0f,480.0f,0.0f });
			m_redDigitSprite[2][ones].SetPosition({ 100.0f,480.0f,0.0f });
		

			/** 点滅対象のインデックスを保存する*/
			m_flashDigits[0] = hundreds;
			m_flashDigits[1] = tens;
			m_flashDigits[2] = ones;

			m_isFlash = true;
			m_flashTimer = 1.0f;


		}
		else 
		{
			/** 白いタイマーの表示*/
			/** 必要な数字だけ表示 該当する数字だけON */
			m_digitSprite[0][hundreds].SetScale({ 0.8f,0.8f,0.8f });
			m_digitSprite[1][tens].SetScale({ 0.8f,0.8f,0.8f });
			m_digitSprite[2][ones].SetScale({ 0.8f,0.8f,0.8f });

			/** 位置設定 */
			m_digitSprite[0][hundreds].SetPosition({ -100.0f,480.0f,0.0f });
			m_digitSprite[1][tens].SetPosition({ 0.0f,480.0f,0.0f });
			m_digitSprite[2][ones].SetPosition({ 100.0f,480.0f,0.0f });
		}

	

		
	}

	/** 10秒以下のカウントアニメーション処理*/
	if (time <= 5)
	{
		m_isFiveTimer = true;
		/** 1秒ごとに0～1を繰り返すローカルのタイムを作る*/
		float localTime = fmodf(m_timer, 1.0f);
		m_animAlpha = 1.0f - (localTime / 1.0f);
		m_animScale = 1.5f +(localTime / 1.0f )* m_animAlpha;

		Vector3 animScaleVec(m_animScale, m_animScale, 1.0f);
		//m_redDigitSprite[0][m_flashDigits[0]].SetScale(animScaleVec);
		//m_redDigitSprite[1][m_flashDigits[1]].SetScale(animScaleVec);
		m_redDigitSprite[2][m_flashDigits[2]].SetScale(animScaleVec);

		/** 緊急性を強調させるために画面中央にタイマーを表示*/
		//m_redDigitSprite[0][m_flashDigits[0]].SetPosition({ -200.0f,100.0f,0.0f });
		//m_redDigitSprite[1][m_flashDigits[1]].SetPosition({ 0.0f,100.0f,0.0f });
		m_redDigitSprite[2][m_flashDigits[2]].SetPosition({ 0.0f,250.0f,0.0f });

		//m_redDigitSprite[0][m_flashDigits[0]].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_animAlpha));
		//m_redDigitSprite[1][m_flashDigits[1]].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_animAlpha));
		m_redDigitSprite[2][m_flashDigits[2]].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_animAlpha));
	}
	
	if (time <= 0)
	{
		m_isEndCountTimer = true;
	}
	
}


void Timer::AddTimer(float time)
{
	m_timer += time;
}


void Timer::Render(RenderContext& rc)
{
	if (m_countdown == nullptr || m_pause == nullptr)
	{
		return;
	}

	/** カウントダウン中は制限時間の描画を止めるために早期リターンをする */
	if (m_countdown->GetCountDown())
	{
		return;
	}

	/** Pause中は制限時間の描画を止める */
	if (m_pause->GetIsPause())
	{
		return;
	}
	/** タイムアウト時は描画を止める*/
	if (m_game->GetIsTimeOut())
	{
		return;
	}
	/** タイムアウトの後のフェード処理でも描画を止める*/
	if (m_game->IsFadeTimeOut())
	{
		return;
	}
	/** 制限時間が０以下になったら描画を止める*/
	if (m_isEndCountTimer)
	{
		return;
	}
	/** 制限時間が残り5秒になったら一の位だけ描画させる*/
	if (m_isFiveTimer)
	{
		m_redDigitSprite[2][m_flashDigits[2]].Draw(rc);
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_digitSprite[i][j].Draw(rc);
			m_redDigitSprite[i][j].Draw(rc);
		}
	}
	
}


