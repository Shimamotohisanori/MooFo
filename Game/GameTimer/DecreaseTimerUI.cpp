#include "stdafx.h"
#include "DecreaseTimerUI.h"
namespace 
{
	/** タイムが減少するに出す[-]の画像のファイルパス*/
	const char* DECREASE_FILEPATH = "Assets/sprite/NumberUI/DecreaseUI.dds";

	/** タイムが減少する際に出す[2]の画像のファイルパス*/
	const char* TWO_FILEPATH = "Assets/sprite/NumberUI/MooFoNumberUI2.dds";

	/** タイムが減少する際に出すの画像の横幅*/
	const int WIDTH = 200.0f;

	/** タイムが減少する際に出す画像の縦幅*/
	const int HEIGHT = 200.0f;

	/** タイムが減少する際に出す画像のスケール */
	const Vector3 SCALE = Vector3(0.4f, 0.4f, 0.4f);

	/** [2]の画像の位置 */
	const Vector3 TWO_POSITION = Vector3(100.0f, 400.0f, 0.0f);

	/** [-]の画像の位置 */
	const Vector3 DECREASE_POSITION = Vector3(30.0f, 400.0f, 0.0f);
}

DecreaseTimerUI::DecreaseTimerUI()
{

}

DecreaseTimerUI::~DecreaseTimerUI()
{

}

bool DecreaseTimerUI::Start()
{
	/** タイマーが減少する際に出す[-]のスプライトレンダーの初期化*/
	m_decreaseSpriteRender.Init(DECREASE_FILEPATH, WIDTH, HEIGHT);
	m_decreaseSpriteRender.SetPosition(DECREASE_POSITION);
	m_decreaseSpriteRender.SetScale(SCALE);
	m_decreaseSpriteRender.Update();

	/** タイマーが減少する際に出す[2]のスプライトレンダーの初期化*/
	m_decreaseTwoSpriteRender.Init(TWO_FILEPATH, WIDTH, HEIGHT);
	m_decreaseTwoSpriteRender.SetPosition(TWO_POSITION);
	m_decreaseTwoSpriteRender.Update();
	m_decreaseTwoSpriteRender.SetScale(SCALE);

	/** 基準位置を保存*/
	m_basePosition = m_decreaseSpriteRender.GetPosition();
	m_baseTwoPosition = m_decreaseTwoSpriteRender.GetPosition();
	return true;
}

void DecreaseTimerUI::Play()
{
	m_isDecreaseTimer = true;
	m_timer = 0.0f;
	m_alpha = 0.0f;
}

void DecreaseTimerUI::Update()
{
	/** フェードアウト処理*/
	FadeOut();
}

void DecreaseTimerUI::FadeOut()
{
	const float FADE_IN_TIME = 0.3f;/** フェードインにかかる時間 */
	const float HOLD_TIME = 0.4f;/** 表示してからフェードアウトが始まるまでの時間 */
	const float FADE_OUT_TIME = 0.5f;/** フェードアウトにかかる時間 */
	const float MOVE_Y = -50.0f;/** タイムを追加したときの[-]と[2]のが出る位置を下にする */
	if (m_isDecreaseTimer)
	{
		m_timer += g_gameTime->GetFrameDeltaTime();
		/** タイマーがフェードイン時間未満の場合 */
		if (m_timer < FADE_IN_TIME)
		{
			/** フェードイン処理 */
			m_alpha = m_timer / FADE_IN_TIME;
		}
		/** タイマーがフェードイン時間以上で表示保持時間未満の場合*/
		else if (m_timer < FADE_IN_TIME + HOLD_TIME)
		{
			/** 表示保持処理 */
			m_alpha = 1.0f;
		}
		/** タイマーがフェードイン時間と表示保持時間の合計以上でフェードアウト時間未満の場合*/
		else if (m_timer < FADE_IN_TIME + HOLD_TIME + FADE_OUT_TIME)
		{
			/** フェードアウト処理 */
			float t = (m_timer - FADE_IN_TIME - HOLD_TIME) / FADE_OUT_TIME;
			m_alpha = 1.0f - t;
			/** 時間に比例して下へ動く*/
			float offSetY = MOVE_Y * t;
			m_decreaseSpriteRender.SetPosition(
				m_basePosition + Vector3(0.0f, offSetY, 0.0f));
			m_decreaseTwoSpriteRender.SetPosition(
				m_baseTwoPosition + Vector3(0.0f, offSetY, 0.0f));
		}
		/** フェードアウト終了処理 */
		else
		{
			m_alpha = 0.0f;
			m_isDecreaseTimer = false;
			m_timer = 0.0f;

			/** 位置を基準位置に戻す */
			m_decreaseSpriteRender.SetPosition(m_basePosition);
			m_decreaseTwoSpriteRender.SetPosition(m_baseTwoPosition);
		}
	}
	m_decreaseSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	m_decreaseSpriteRender.Update();
	m_decreaseTwoSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	m_decreaseTwoSpriteRender.Update();
}

void DecreaseTimerUI::Render(RenderContext& rc)
{
	if (m_isDecreaseTimer)
	{
		m_decreaseSpriteRender.Draw(rc);
		m_decreaseTwoSpriteRender.Draw(rc);
	}
}
