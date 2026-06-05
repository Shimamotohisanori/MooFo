#include "stdafx.h"
#include "Score.h"
#include "SoundManager/SoundManager.h"

namespace
{
	/** 三桁の数字を描画する画像の大きさ */
	const Vector3 DIGIT_SCALE = { 0.0f,0.0f, 0.0f };

	/** 数字の大きさ */
	const int  GAMECLEAR_WIDTH = 70.0f;
	const int HEIGHT= 85.0f;

	/** Lerp関数の補完率 */
	constexpr float LERP_SPEED = 0.2f;
}


Score::Score()
{

}


Score::~Score()
{
	DeleteGO(m_scoreDecreaseSE);
	DeleteGO(m_scoreRiseSE);
}


bool Score::Start()
{
	m_score = 0;
	
	m_displayScore = 0;

	m_isResult = false;
	/** 最初は絶対更新するためここを - 1する */
	m_prevScore = -1;

	/** 全ての画像スプライト(50個)を初期化 */
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			/** 画像の読み込みと大きさ調整 */
			m_digitSprite[i][j].Init(m_digitPaths[j], GAMECLEAR_WIDTH,HEIGHT);

			/** 非表示にしないと前のスコアが残ってしまいバグってしまう。 */
			m_digitSprite[i][j].SetScale(DIGIT_SCALE);
		}
	}
	return true;
}


void Score::Update()
{
	
	/** スコアのカウンターアニメーション処理 */
	ScoreCounterAnimation();

	/** スコアの描画処理 */
	TextScore();

	/** スプライトの更新処理 */
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_digitSprite[i][j].Update();
		}
	}
}


void Score::TextScore()
{
	/** スコアが変わったときだけ更新する */
	if (m_displayScore == m_prevScore)
	{
		return;
	}

	/** スコアを保存 */
	m_prevScore = m_displayScore;
	int score = m_displayScore;

	/** 桁分解 */
	int digit[5];
	for (int i = 0; i < 5; i++)
	{
		digit[i] = score % 10;
		score /= 10;
	}

	/** 全部非表示 */
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_digitSprite[i][j].SetScale({ 0,0,0, });

		}
	}

	/** 表示(右から並べる) */
	float baseX;
	float baseY;

	if (m_isResult)
	{
		baseY = -100.0f;
		/** ゲームクリアならスコアを表示する */
		if (m_resultType == ResultType::GameClear)
		{
			baseX = -700.0f;
			baseY = -100.0f;
		}

		/** ゲームオーバーならスコアを表示する */
		else if (m_resultType == ResultType::GameOver)
		{
			baseX = -700.0f;
			baseY = -100.0f;
		}
	}
	else
	{
		/** ゲーム中なら */
		baseX = -900.0f;
		baseY = -450.0f;
	}

	for (int i = 0; i < 5; i++)
	{
		int d = digit[i];

		int index = 4 - i;

		/** 右→左に変換 */
		m_digitSprite[index][d].SetScale({1,1,1});

		/** 位置調整(横並び) */
		m_digitSprite[index][d].SetPosition({
			baseX + index * 80,
			baseY,//下側
			0.0f });
	}
	
}


void Score::AddScore(int value)
{
	SoundManager* soundManager = FindGO<SoundManager>("soundmanager");

	m_score += value;

	/** スコアが増えるSEを再生する */
	if (soundManager)
	{
		m_scoreRiseSE = soundManager->PlayingSE(SoundSE::enScoreRiseSE, false);
	}

}


void Score::DecreaseScore(int value)
{
	SoundManager* soundManager = FindGO<SoundManager>("soundmanager");

	m_score -= value;

	/** スコアが減るSEを再生する */
	if (soundManager)
	{
		m_scoreDecreaseSE = soundManager->PlayingSE(SoundSE::enScoreDecreaseSE, false);
	}

	/** スコアがマイナスにならないようにする */
	if (m_score < 0)
	{
		m_score = 0;
	}
}


void Score::SetResult(bool isResult)
{
	m_isResult = isResult;
}


void Score::SetScore(int score)
{
	/** スコアを直接反映する */
	m_score = score;

	/** 表示用スコアも同期 */
	//m_displayScore = score;

	/** 強制更新 */
	m_prevScore = -1;
}


void Score::ScoreCounterAnimation()
{
	/** Lerp関数で滑らかに値を近づける */
	/** 表示するスコアを徐々にスコアに近づける */
	/** 型変換をすることで、整数の計算を行う */
	m_displayScore = m_displayScore + (int)((m_score - m_displayScore) * LERP_SPEED);

	/** 絶対値が5未満ならスコアを到達させる */
	if (abs(m_score - m_displayScore) < 5)
	{
		m_displayScore = m_score;
	}
}


int Score::GetScore() const
{
	return m_score;
}


void Score::Render(RenderContext& rc)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_digitSprite[i][j].Draw(rc);
		}
	}
}