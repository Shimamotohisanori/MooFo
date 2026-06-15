#include "stdafx.h"
#include "UFOLightUI.h"
#include "CountDown/CountDown.h"
#include "Pause/Pause.h"
#include"UFOLightManager.h"

namespace
{
	/** 光が出ているときに表示する数字のスプライト */
	const char* LIGHT_APPEAR_NUMBER_FILEPATH[5] =
	{
		"Assets/sprite/UFOLightFontUI/Number1.dds",
		"Assets/sprite/UFOLightFontUI/Number2.dds",
		"Assets/sprite/UFOLightFontUI/Number3.dds",
		"Assets/sprite/UFOLightFontUI/Number4.dds",
		"Assets/sprite/UFOLightFontUI/Number5.dds"
	};

	/** 光が出ているときに表示する「光の発射まで」のスプライト */
	const char* LIGHT_APPEAR_SPRITE_FILEPATH = "Assets/sprite/UFOLightFontUI/NextLightFring.dds";

	/** 光が出ているときに発射する「秒」のスプライト */
	const char* LIGHT_APPEAR_SECONDS_SPRITE_FILEPATH = "Assets/sprite/UFOLightFontUI/Seconds.dds";

	/** UFOの大きさ */
	const Vector3 UFO_SCALE = Vector3(3.0f, 8.0f, 3.0f);
	const Vector3 FONT_SCALE = Vector3(2.0f, 2.0f, 2.0f);

	/** 数字の大きさ */
	const float NUMBERFONT_WIDTH = 100.0f;
	const float NUMBERFONT_HEIGHT = 100.0f;

	/** 「光の発射まで」の大きさ */
	const float FONT_WIDTH = 200.0f;
	const float FONT_HEIGHT = 200.0f;

	const float SECOMDS_WIDTH = 100.0f;
	const float SECOMDS_HEIGHT = 100.0f;
}


UFOLightUI::UFOLightUI()
{}

UFOLightUI::~UFOLightUI()
{}

bool UFOLightUI::Start()
{
	for (int i = 0; i < 5; i++)
	{
		/** 光の数字スプライト初期化 */
		m_LightApperNumberSpriteRender[i].Init(LIGHT_APPEAR_NUMBER_FILEPATH[i], NUMBERFONT_WIDTH, NUMBERFONT_HEIGHT);
		m_LightApperNumberSpriteRender[i].Update();
	}

	/** 光の発射までのスプライト初期化 */
	m_LightApperSpriteRender.Init(LIGHT_APPEAR_SPRITE_FILEPATH, FONT_WIDTH, FONT_HEIGHT);
	m_LightApperSpriteRender.SetPosition(Vector3(740.0f, 500.0f, 0.0f));
	m_LightApperSpriteRender.SetScale(FONT_SCALE);
	m_LightApperSpriteRender.Update();

	/** 秒のスプライト初期化 */
	m_secondsSpriteRender.Init(LIGHT_APPEAR_SECONDS_SPRITE_FILEPATH, SECOMDS_WIDTH, SECOMDS_HEIGHT);
	m_secondsSpriteRender.SetPosition(Vector3(940.0f, 510.0f, 0.0f));
	m_secondsSpriteRender.SetScale(FONT_SCALE);
	m_secondsSpriteRender.Update();
	
	
	return true;
}

void UFOLightUI::Update()
{
	
	if (m_ufolightManager == nullptr)
	{
		m_ufolightManager = FindGO<UFOLightManager>("ufolightmanager");
	}

	if (m_ufolightManager == nullptr)
	{
		return;
	}

	m_pause = FindGO<Pause>("pause");
	m_countdown = FindGO<CountDown>("countdown");
	/** 光が出ているかどうかのフラグを取得 */
	CountSpriteUI();

	/** 光が出ているときだけ秒数のスプライトを更新 */
	if (m_currentCount >= 0)
	{
		m_LightApperNumberSpriteRender[m_currentCount].Update();
	}
}

void UFOLightUI::Render(RenderContext& rc)
{
	if (m_pause == nullptr || m_countdown == nullptr)
	{
		return;
	}

	if (m_countdown->GetIsCountDown())
	{
		return;
	}

	
	/** Pause中は描画を止める */
	if (m_pause->GetIsPause())
	{
		return;
	}
	/** 光が出ているときは描画を止める */
	if (!m_ufolightManager->IsEmitting())
	{
		/** 光の発射までのスプライト描画 */
		m_LightApperSpriteRender.Draw(rc);
		/** 秒のスプライト描画 */
		m_secondsSpriteRender.Draw(rc);
	}
	

	/** 数字スプライトの描画*/
	if (m_currentCount >= 0 && !m_ufolightManager->IsEmitting())
	{
		m_LightApperNumberSpriteRender[m_currentCount].Draw(rc);
	}

}

void UFOLightUI::CountSpriteUI()
{
	if (m_ufolightManager == nullptr)
	{
		return;
	}

	/** 光が出ているときは表示しない */
	if (m_ufolightManager->IsEmitting())
	{
		/** 光が出ているときはフォントを表示しない状態にする */
		m_currentCount = -1;
		return;
	}

	float timer = m_ufolightManager->GetTimer();

	/** 残り秒数を計算(5→1) */
	int seconds = static_cast<int>(std::ceilf(timer));

	/** 残り秒数が変わったらスプライトを更新 */
	if (seconds <= 0 || seconds > 5)
	{
		m_currentCount = -1;
		return;
	}

	/** 画像配列用に変換 */
	m_currentCount = seconds - 1;

	/** 表示位置 */
	Vector3 pos;
	pos.x = 900.0f;
	pos.y = 513.0f;
	pos.z = 0.0f;
	m_LightApperNumberSpriteRender[m_currentCount].SetPosition(pos);

}
