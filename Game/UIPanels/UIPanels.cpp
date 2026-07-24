#include "stdafx.h"
#include "UIPanels.h"
#include "GameScene/Game.h"
#include "GameScene/LoadingScene.h"
#include "Pause/Pause.h"
namespace
{
	/** 黒いパネルのスプライト */
	const char* PANEL_TEXTURE_FILEPATH = "Assets/sprite/PanelUI/PanelUI.dds";

	/** 黒いパネルの色 */
	const Vector4 PANEL_COLOR = Vector4(1.0f, 1.0f, 1.0f, 0.5f);

	/** 画面左下のUIの後ろに配置する黒いパネルの位置 */
	const Vector3 LOWER_LEFT_PANEL_POSITION = Vector3(-700.0f, -350.0f, 0.0f);

	/** 画面左下のUIの後ろに配置する黒いパネルの大きさ */
	const Vector3 LOWER_LEFT_PANEL_SCALE = Vector3(6.0f, 4.0f, 1.0f);

	/** タイマーの後ろに配置する黒いパネルの位置 */
	const Vector3 TIMER_PANEL_POSITION = Vector3(0.0f, 420.0f, 0.0f);

	/** タイマーの後ろに配置する黒いパネルの大きさ */
	const Vector3 TIMER_PANEL_SCALE = Vector3(5.0f, 2.5f, 1.0f);

	/** 画面左上のUIの後ろに配置する黒いパネルの位置 */
	const Vector3 UPPER_LEFT_PANEL_POSITION = Vector3(-770.0f, 460.0f, 0.0f);

	/** 画面左上のUIの後ろに配置する黒いパネルの大きさ */
	const Vector3 UPPER_LEFT_PANEL_SCALE = Vector3(4.25f, 1.7f, 1.0f);

	/** 画面右下のUIの後ろに配置する黒いパネルの位置 */
	const Vector3 LOWER_RIGHT_PANEL_POSITION = Vector3(780.0f, -420.0f, 0.0f);

	/** 画面右下のUIの後ろに配置する黒いパネルの大きさ */
	const Vector3 LOWER_RIGHT_PANEL_SCALE = Vector3(4.0f, 2.5f, 1.0f);
}
UIPanels::UIPanels()
{}

UIPanels::~UIPanels()
{}

bool UIPanels::Start()
{
	m_lowerLeftPanelSpriteRender.Init(PANEL_TEXTURE_FILEPATH,100.0f,100.0f);
	m_lowerLeftPanelSpriteRender.SetPosition(LOWER_LEFT_PANEL_POSITION);
	m_lowerLeftPanelSpriteRender.SetScale(LOWER_LEFT_PANEL_SCALE);
	m_lowerLeftPanelSpriteRender.SetMulColor(PANEL_COLOR);
	m_lowerLeftPanelSpriteRender.Update();

	m_timerPanelSpriteRender.Init(PANEL_TEXTURE_FILEPATH, 100.0f, 100.0f);
	m_timerPanelSpriteRender.SetPosition(TIMER_PANEL_POSITION);
	m_timerPanelSpriteRender.SetScale(TIMER_PANEL_SCALE);
	m_timerPanelSpriteRender.SetMulColor(PANEL_COLOR);
	m_timerPanelSpriteRender.Update();

	m_upperLeftPanelSpriteRender.Init(PANEL_TEXTURE_FILEPATH, 100.0f, 100.0f);
	m_upperLeftPanelSpriteRender.SetPosition(UPPER_LEFT_PANEL_POSITION);
	m_upperLeftPanelSpriteRender.SetScale(UPPER_LEFT_PANEL_SCALE);
	m_upperLeftPanelSpriteRender.SetMulColor(PANEL_COLOR);
	m_upperLeftPanelSpriteRender.Update();

	m_lowerRightPanelSpriteRender.Init(PANEL_TEXTURE_FILEPATH, 100.0f, 100.0f);
	m_lowerRightPanelSpriteRender.SetPosition(LOWER_RIGHT_PANEL_POSITION);
	m_lowerRightPanelSpriteRender.SetScale(LOWER_RIGHT_PANEL_SCALE);
	m_lowerRightPanelSpriteRender.SetMulColor(PANEL_COLOR);
	m_lowerRightPanelSpriteRender.Update();
	return true;
}

void UIPanels::Update()
{
}

void UIPanels::Render(RenderContext & renderContext)
{
	/** ロード中なら描画処理を行わない */
	LoadingScene* loadingScene = FindGO<LoadingScene>("loading");
	if (loadingScene && !loadingScene->GetLoadingEnd())
	{
		return;
	}

	/** ゲームが存在しない
	 * タイムアウトしている
	 * フェードアウト中の場合は描画処理を行わない */
	Game* game = FindGO<Game>("game");
	if (!game || game->GetIsTimeOut() || game->IsFadeTimeOut())
	{
		return;
	}

	/** ポーズ中の場合は描画処理を行わない */
	Pause* pause = FindGO<Pause>("pause");
	if (pause && pause->GetIsPause())
	{
		return;
	}

	m_lowerLeftPanelSpriteRender.Draw(renderContext);

	m_timerPanelSpriteRender.Draw(renderContext);

	m_upperLeftPanelSpriteRender.Draw(renderContext);

	m_lowerRightPanelSpriteRender.Draw(renderContext);
}
