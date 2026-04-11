#include "stdafx.h"
#include "Score.h"

namespace
{
	/** 三桁の数字を描画する画像の大きさ */
	const Vector3 DIGIT_SCALE = { 0.0f,0.0f, 0.0f };

	//数字の大きさ
	const int  WIDTH = 85.0f;
	const int HEIGHT= 85.0f;
}

Score::Score()
{

}

Score::~Score()
{

}


bool Score::Start()
{
	m_score = 0;
	//if (m_score == m_prevScore){return;}
	// このコードが値が同じなら早期リターンで画像が表示されなくなるから
	//最初は絶対更新するためここを-1する
	m_prevScore = -1;
	//全ての画像スプライト(50個)を初期化
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			//画像の読み込みと大きさ調整
			m_digitSprite[i][j].Init(m_digitPaths[j], WIDTH,HEIGHT);

			//非表示にしないと前のスコアが残ってしまいバグってしまう。
			m_digitSprite[i][j].SetScale(DIGIT_SCALE);
		}
	}
	return true;
}

void Score::Update()
{

	TextScore();

	//これがないと表示されない
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
	//スコアが変わったときだけ更新する
	if (m_score == m_prevScore)
	{
		return;
	}
	//スコアを保存
	m_prevScore = m_score;
	int score = m_score;

	//桁分解
	int digit[5];
	for (int i = 0; i < 5; i++)
	{
		digit[i] = score % 10;
		score /= 10;
	}

	//全部非表示
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_digitSprite[i][j].SetScale({ 0,0,0, });

		}
	}

//表示(右から並べる)
	float baseX;
	float baseY;

	if (m_isResult)
	{
		baseY = -100.0f;
		//ゲームクリアならスコアを-580の位置に表示する
		if (m_resultType == ResultType::GameClear)
		{
			baseX = -580.0f;
			baseY = -150.0f;
		}
		//ゲームオーバーならスコアを-680の位置に表示する
		else if (m_resultType == ResultType::GameOver)
		{
			baseX = -680.0f;
			baseY = -70.0f;
		}
	}
	else
	{
		//ゲーム中なら
		baseX = -900.0f;
		baseY = -450.0f;
	}

	for (int i = 0; i < 5; i++)
	{
		int d = digit[i];

		int index = 4 - i;
		//右→左に変換
		m_digitSprite[index][d].SetScale({1,1,1});

		//位置調整(横並び)
		m_digitSprite[index][d].SetPosition({
			baseX + index * 80,//
			baseY,//下側
			0.0f });
	}
	
}
void Score::AddScore(int value)
{
	m_score += value;
}
void Score::DecreaseScore(int value)
{
	m_score -= value;
	//スコアがマイナスにならないようにする
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
	m_score = score;
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