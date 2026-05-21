#include "stdafx.h"
#include "Combo.h"
#include "GameTimer/Timer.h"
#include "Score/Score.h"
#include "GameTimer/AddTimerUI.h"
#include "SoundManager/SoundManager.h"
namespace
{
	const char* COMBO_NUMBER_FILEPATH = "Assets/sprite/NumberUI/";
	const char* COMBO_NUMBER_FORMAT = ".dds";
	const char* COMBO_NUMBER_FILENAME_LIST[10] =
	{
		"MooFoNumberUI0",
		"MooFoNumberUI1",
		"MooFoNumberUI2",
		"MooFoNumberUI3",
		"MooFoNumberUI4",
		"MooFoNumberUI5",
		"MooFoNumberUI6",
		"MooFoNumberUI7",
		"MooFoNumberUI8",
		"MooFoNumberUI9"
	};

	/** コンボ画像の初期ポジション */
	const Vector3 COMBO_SPRITE_INITIAL_POSITION = { -350.0f, -430.0f, 0.0f };

	/** 数字のスプライトのサイズ */
	const Vector2 COMBO_NUMBER_SPRITE_SIZE = Vector2(40.0f, 60.0f);

	/** コンボ画像のファイルパス */
	const char* COMBO_SPRITE_FILEPATH = "Assets/sprite/ComboUI/Combo.dds";

	/** コンボ画像の移動速度 */
	const float COMBO_SPRITE_MOVE_SPEED = 50.0f;
	
}
Combo::Combo()
{
}

Combo::~Combo()
{
	//DeleteGO(m_addTimerSE);
}

bool Combo::Start()
{
	m_timer = FindGO<Timer>("timer");

	m_score = FindGO<Score>("score");

	//コンボ画像の初期化
	m_comboSprite.Init(COMBO_SPRITE_FILEPATH, 200.0f, 200.0f);
	m_comboSprite.SetPosition(COMBO_SPRITE_INITIAL_POSITION);
	m_comboSprite.Update();

	/** 数のUIを左右全てに読み込む */
	for (int i = 0; i < 10; i++)
	{
		m_filePath[i] = std::string(COMBO_NUMBER_FILEPATH) + COMBO_NUMBER_FILENAME_LIST[i] + COMBO_NUMBER_FORMAT;

		m_comboTensSprite[i].Init(m_filePath[i].c_str(), COMBO_NUMBER_SPRITE_SIZE.x, COMBO_NUMBER_SPRITE_SIZE.y);
		m_comboOnesSprite[i].Init(m_filePath[i].c_str(), COMBO_NUMBER_SPRITE_SIZE.x, COMBO_NUMBER_SPRITE_SIZE.y);
	}

	return true;
}

void Combo::Update()
{
	ViewComboSprite();
}

void Combo::Render(RenderContext & rc)
{
	/** コンボ表示フラグが立っているなら */
	if (m_isViewComboSprite)
	{
		/** コンボ画像の位置をコンボタイマーに応じて右に移動させる */
		m_comboSprite.SetPosition(
			Vector3{
			COMBO_SPRITE_INITIAL_POSITION.x + m_comboSpriteMoveTime * COMBO_SPRITE_MOVE_SPEED,
			COMBO_SPRITE_INITIAL_POSITION.y,
			COMBO_SPRITE_INITIAL_POSITION.z
			});

		/** コンボ画像の表示時間に応じてコンボ画像のアルファ値を変化させる */
		m_comboSprite.SetMulColor(
			Vector4(
				1.0f,
				1.0f,
				1.0f,
				m_comboSpriteViewTime));

		/** コンボ画像を表示する */
		m_comboSprite.Draw(rc);

		/** コンボの十の位と一の位を取得する */
		int tens = m_combo / 10;
		int ones = m_combo % 10;

		/** コンボの十の位が0より大きいなら */
		if (tens > 0)
		{
			/** コンボの十の位の画像の位置をコンボタイマーに応じて右に移動させる */
			m_comboTensSprite[m_combo / 10].SetPosition(
				Vector3{
				COMBO_SPRITE_INITIAL_POSITION.x - 175.0f + m_comboSpriteMoveTime * COMBO_SPRITE_MOVE_SPEED,
				COMBO_SPRITE_INITIAL_POSITION.y - 22.5f,
				COMBO_SPRITE_INITIAL_POSITION.z
				});

			/** コンボの十の位の画像のアルファ値をコンボタイマーに応じて変化させる */
			m_comboTensSprite[m_combo / 10].SetMulColor(
				Vector4(
					1.0f,
					1.0f,
					1.0f,
					m_comboSpriteViewTime));

			/** コンボの十の位を表示する */
			m_comboTensSprite[m_combo / 10].Draw(rc);
		}

		/** コンボの一の位の画像の位置をコンボタイマーに応じて右に移動させる */
		m_comboOnesSprite[m_combo % 10].SetPosition(
			Vector3{
			COMBO_SPRITE_INITIAL_POSITION.x - 125.0f + m_comboSpriteMoveTime * COMBO_SPRITE_MOVE_SPEED,
			COMBO_SPRITE_INITIAL_POSITION.y - 22.5f,
			COMBO_SPRITE_INITIAL_POSITION.z
			});

		/** コンボの一の位の画像のアルファ値をコンボタイマーに応じて変化させる */
		m_comboOnesSprite[m_combo % 10].SetMulColor(
			Vector4(
				1.0f,
				1.0f,
				1.0f,
				m_comboSpriteViewTime));

		/** コンボの一の位を表示する */
		m_comboOnesSprite[m_combo % 10].Draw(rc);
	}
}

void Combo::AddCombo()
{
	m_combo++;
	
	/** 10秒以内に牛を救出出来ればコンボ継続 */
	m_comboTimer = 10.0f;

	m_isViewComboSprite = true;

	/** コンボの時だけ制限時間を増やす */
	if (m_combo >= 2)
	{
		m_timer = FindGO<Timer>("timer");
		if (m_timer)
		{
			//コンボすると３秒追加
			m_timer->AddTimer(3.0f);
			m_addTimerUI = FindGO<AddTimerUI>("addTimerUI");
			if (m_addTimerUI)
			{
				m_addTimerUI->Play();
			}

			SoundManager* soundManager = FindGO<SoundManager>("soundmanager");
			if (soundManager)
			{
				m_addTimerSE = soundManager->PlayingSE(SoundSE::enAddTimeSE, false);
			}
		}
	}
}

void Combo::ResetCombo()
{
	m_combo = 0;
	m_comboTimer = 0.0f;
}

void Combo::AddScore(int score)
{
	int multiplier = 1;

	/** 5コンボするごとにスコアの獲得量を２倍 */
	if (m_combo % 5 == 0 && m_combo > 0)
	{
		multiplier = 2;
	}

	/** スコアを加算する */
	Score* scoreobject = FindGO<Score>("score");

	/** スコアオブジェクトが存在するならスコアを加算する */
	if (scoreobject)
	{
		scoreobject->AddScore(score * multiplier);
	}
}

bool Combo::IsCombo() const
{
	return m_combo >= 2;
}

void Combo::ViewComboSprite()
{
	/** コンボ画像フラグが立っていたら */
	if (m_isViewComboSprite)
	{
		m_comboSpriteViewTime -= g_gameTime->GetFrameDeltaTime();

		m_comboSpriteMoveTime += g_gameTime->GetFrameDeltaTime();
		
		/** 1秒経てば */
		if (m_comboSpriteViewTime < 0.0f)
		{
			/** コンボ画像の位置を初期位置に戻す */
			m_comboSprite.SetPosition(COMBO_SPRITE_INITIAL_POSITION);

			/** コンボ画像の表示時間をリセットする */
			m_comboSpriteViewTime = 1.0f;

			/** コンボ画像を非表示にする */
			m_isViewComboSprite = false;
			
			/** コンボ画像の移動時間をリセットする */
			m_comboSpriteMoveTime = 0.0f;
		}

		m_comboSprite.Update();
		m_comboTensSprite[m_combo / 10].Update();
		m_comboOnesSprite[m_combo % 10].Update();
	}
}
