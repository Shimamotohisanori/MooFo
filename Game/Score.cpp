#include "stdafx.h"
#include "Score.h"
namespace
{
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
	m_prevScore = -1;
	//全ての画像スプライト(50個)を初期化
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			//画像の読み込みと大きさ調整
			m_digitSprite[i][j].Init(m_digitPaths[j], WIDTH,HEIGHT);
			//非表示
			m_digitSprite[i][j].SetScale({ 0,0,0, });
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
	for (int i = 0; i < 5; i++)
	{
		int d = digit[i];
		m_digitSprite[4 - i][d].SetScale({ 1,1,1 });

		//位置調整(横並び)
		m_digitSprite[4 - i][d].SetPosition({
			-900.0f + i * 100,//左寄り
			-450.0f,//下側
			0.0f });
	}
	
}
void Score::AddScore(int value)
{
	m_score += value;
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