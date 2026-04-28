#include "stdafx.h"
#include "AddTimerUI.h"

namespace
{
	/** タイムを追加する際に出す[+]の画像のファイルパス*/
	const char* PLUS_FILEPATH = "Assets/sprite/NumberUI/PlusFont.dds";
	/** タイムを追加する際に出す[3]の画像のファイルパス*/
	const char* THREE_FILEPATH = "Assets/sprite/NumberUI/MooFoNumberUI3.dds";
	/** タイムを追加する際に出すの画像の横幅*/
	const int WIDTH = 200.0f;
	/** タイムを追加する際に出す画像の縦幅*/
	const int HEIGHT = 200.0f;
	/** タイムを追加する際に出す画像のスケール */
	const Vector3 SCALE = Vector3(0.4f, 0.4f, 0.4f);
	/** [3]の画像の位置 */
	const Vector3 THREE_POSITION = Vector3(100.0f, 400.0f, 0.0f);
	/** [+]の画像の位置 */
	const Vector3 PLUS_POSITION = Vector3(30.0f, 400.0f, 0.0f);
}
AddTimerUI::AddTimerUI()
{

}

AddTimerUI::~AddTimerUI()
{

}


bool AddTimerUI::Start()
{
	/** タイムを追加する際に出す[+]のスプライトレンダーの初期化*/
	m_addTimerPlusSpriteRender.Init(PLUS_FILEPATH, WIDTH, HEIGHT);
	m_addTimerPlusSpriteRender.SetPosition(PLUS_POSITION);
	m_addTimerPlusSpriteRender.SetScale(SCALE);
	m_addTimerPlusSpriteRender.Update();
	/** タイムを追加する際に出す[3]のスプライトレンダーの初期化*/
	m_addTimerThreeSpriteRender.Init(THREE_FILEPATH, WIDTH, HEIGHT);
	m_addTimerThreeSpriteRender.SetPosition(THREE_POSITION);
	m_addTimerThreeSpriteRender.Update();
	m_addTimerThreeSpriteRender.SetScale(SCALE);

	/** 基準位置を保存しておく */
	m_basePosition = m_addTimerPlusSpriteRender.GetPosition();
	m_baseThreePosition = m_addTimerThreeSpriteRender.GetPosition();
	return true;
}

void AddTimerUI::Play()
{
	m_isAddTimer = true;
	m_timer = 0.0f;
	m_alpha = 0.0f;
}

void AddTimerUI::Update()
{
	/** フェードアウト処理 */
	FadeOut();
}

void AddTimerUI::FadeOut()
{
	const float FADE_IN_TIME = 0.3f;/** フェードインにかかる時間 */
	const float HOLD_TIME = 0.4f;/** 表示してからフェードアウトが始まるまでの時間 */
	const float FADE_OUT_TIME = 0.5f;/** フェードアウトにかかる時間 */
	const float MOVE_Y = -50.0f;/** タイムを追加したときの[+]と[3]のが出る位置を下にする */
	if (m_isAddTimer)
	{
		m_timer += g_gameTime->GetFrameDeltaTime();
/** タイマーがフェードイン時間未満の場合 */
		if (m_timer < FADE_IN_TIME)
		{
			/** フェードイン処理 */
			m_alpha = m_timer /FADE_IN_TIME;
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
			m_addTimerPlusSpriteRender.SetPosition(
				m_basePosition + Vector3(0.0f, offSetY, 0.0f));
			m_addTimerThreeSpriteRender.SetPosition(
				m_baseThreePosition + Vector3(0.0f, offSetY, 0.0f));
		}
		/** フェードアウト終了処理 */
		else
		{
			m_alpha = 0.0f;
			m_isAddTimer = false;
			m_timer = 0.0f;

			/** 位置を基準位置に戻す */
			m_addTimerPlusSpriteRender.SetPosition(m_basePosition);
			m_addTimerThreeSpriteRender.SetPosition(m_baseThreePosition);
		}
	}
	m_addTimerPlusSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	m_addTimerPlusSpriteRender.Update();
	m_addTimerThreeSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	m_addTimerThreeSpriteRender.Update();
}
void AddTimerUI::Render(RenderContext& rc)
{
	if (m_isAddTimer)
	{
		m_addTimerPlusSpriteRender.Draw(rc);
		m_addTimerThreeSpriteRender.Draw(rc);
	}
}