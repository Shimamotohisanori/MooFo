#include "stdafx.h"
#include "Aiming.h"
#include "Pause/Pause.h"
#include "CountDown/CountDown.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "GameCamera/GameCamera.h"
#include "Rope/Rope.h"
#include"CowLivesUI.h"
namespace
{
	/** 照準の画像ファイルパス */
	const char* AIMING_FILEPATH = "Assets/sprite/AimingUI/aiming.dds";

	/** 照準の大きさ */
	const Vector3 AIMING_SCALE = { 2.0f, 2.0f, 1.0f };

	/** 照準の初期座標 */
	const Vector3 AIMING_INITIAL_POSITION = { 0.0f, 0.0f, 0.0f };
}
Aiming::Aiming()
{}

Aiming::~Aiming()
{}

bool Aiming::Start()
{
	m_aimingSpriteRender.Init(AIMING_FILEPATH,50.0f,50.0f);
	m_aimingSpriteRender.SetPosition(AIMING_INITIAL_POSITION);
	m_aimingSpriteRender.SetScale(AIMING_SCALE);
	m_aimingSpriteRender.Update();

	m_pause = FindGO<Pause>("pause");
	m_countdown = FindGO<CountDown>("countdown");
	m_gameCamera = FindGO<GameCamera>("gameCamera");
	m_rope = FindGO<Rope>("rope");

	return true;
}

void Aiming::Update()
{
	m_cowLivesUI = FindGO<CowLivesUI>("cowlivesui");

	/** 牛がプレイヤーに捕獲されたかどうかを確認 */
	m_isAnyCowCaptured = false;
	auto cows = FindGOs<Cow>("cow");
	for (const auto& cow : cows)
	{
		if (cow->GetIsCaptured())
		{
			m_isAnyCowCaptured = true;
			break;
		}
	}

	m_aimingSpriteRender.Update();
}

void Aiming::Render(RenderContext& rc)
{
	/** カウントダウン中は照準を描画しない */
	if (m_countdown && m_countdown->GetCountDown())
	{
		return;
	}

	/** ポーズ中は照準を描画しない */
	if (m_pause && m_pause->GetIsPause())
	{
		return;
	}

	/** 牛が捕獲されていない場合のみ照準を描画 */
	if (m_isAnyCowCaptured)
	{
		return;
	}

	/** ロープを投げている最中は照準を描画しない */
	if (m_rope && m_rope->GetIsThrowRope())
	{
		return;
	}
	/** フェード処理中は描画しない*/
	if (m_cowLivesUI && m_cowLivesUI->IsFadeInComplete())
	{
		return;
	}
	/** 照準が牛を狙っている時は赤く
	 * そうじゃないなら白くする */
	if (m_gameCamera && m_gameCamera->GetIsAimingCow())
	{
		m_aimingSpriteRender.SetMulColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	}

	else
	{
		m_aimingSpriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	m_aimingSpriteRender.Draw(rc);
}