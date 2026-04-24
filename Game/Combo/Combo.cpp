#include "stdafx.h"
#include "Combo.h"
#include "GameTimer/Timer.h"
#include "Score/Score.h"

Combo::Combo()
{
}

Combo::~Combo()
{
}

bool Combo::Start()
{
	m_timer = FindGO<Timer>("timer");

	m_score = FindGO<Score>("score");

	//コンボ画像の初期化
	m_comboSprite.Init("Assets/sprite/ComboUI/Combo.dds", 200.0f, 200.0f);
	m_comboSprite.SetPosition({ -420.0f, -430.0f, 0.0f });
	m_comboSprite.Update();

	return true;
}

void Combo::Update()
{}

void Combo::Render(RenderContext & rc)
{
	m_comboSprite.Draw(rc);
}

void Combo::AddCombo()
{
	m_combo++;
	//5秒以内に牛を救出出来ればコンボ継続
	m_comboTimer = 6000.0f;
	//コンボの時だけ制限時間を増やす
	if (m_combo >= 2)
	{
		m_timer = FindGO<Timer>("timer");
		if (m_timer)
		{
			//コンボすると３秒追加
			m_timer->AddTimer(3.0f);
		}
	}
}

void Combo::ResetCombo()
{
	m_combo = 0;
	m_scoreMagnification = 1;
	m_comboTimer = 0.0f;
}

void Combo::AddScore(int score)
{
	int multiplier = 1;
	//5コンボするごとにスコアの獲得量を２倍
	if (m_combo % 5 == 0 && m_combo > 0)
	{
		multiplier = 2;
	}
	if (m_score)
	{
		m_score->AddScore(score * multiplier);
	}
}

bool Combo::IsCombo() const
{
	return m_combo >= 2;
}
