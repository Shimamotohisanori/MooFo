#include "stdafx.h"
#include "CowLivesUI.h"
#include"GameScene/Game.h"
#include"GameScene/LoadingScene.h"
#include"Pause/Pause.h"
namespace
{
	/** 横幅*/
	constexpr int WIDTH = 110;
	/** 縦幅*/
	constexpr int HEIGHT = 110;

	/** 連れ去られていない時のプレイヤーの残機UI*/
	const char* COW_NORMAL_FILEPATH = "Assets/sprite/CowLiveUI/CowLife_Normal.dds";
	/** 連れ去られた時ののプレイヤーの残機UI*/
	const char* COW_DEAD_FILEPATH = "Assets/sprite/CowLiveUI/CowLife_Dead.dds";

	/** 牛の残機が一つの時に出すプレイヤー残機UI*/
	const char * COW_CRISIS_FILEPATH = "Assets/sprite/CowLiveUI/CowLife_Crisis.dds";
	/** 一枚目のアイコン表示位置*/
	const Vector3 ICON_START_POS = Vector3(-680.0f,410.0f,0.0f);

	/** アイコン同士の間隔 */
	constexpr float ICON_SPACING = 110.0f;
}

CowLivesUI::CowLivesUI()
{

}

CowLivesUI::~CowLivesUI()
{

}


bool CowLivesUI::Start()
{
	/** アイコンの初期化を行う*/
	ResetLives();
	return true;
}


void CowLivesUI::Update()
{
	m_pause = FindGO<Pause>("pause");
	for (int i = 0; i < COW_LIFE_NUM; i++)
	{
		m_cowIcons[i].render.Update();
	}

	/** 残り残機が一つになったら点滅タイマーを進める*/
	m_blinkFreamCount++;
	if (m_blinkFreamCount >= BLINK_INTERVAL)
	{
		m_blinkFreamCount = 0;
		m_isBlinkVisible = !m_isBlinkVisible;
	}
}


void CowLivesUI::ResetLives()
{
	for (int i = 0; i < COW_LIFE_NUM; i++)
	{
		m_cowIcons[i].render.Init(COW_NORMAL_FILEPATH, WIDTH, HEIGHT);

		Vector3 pos = ICON_START_POS;
		/** アイコンの間隔をあける*/
		pos.x -= ICON_SPACING * i;

		m_cowIcons[i].render.SetPosition(pos);
		m_cowIcons[i].render.Update();
		m_cowIcons[i].isDead = false;

		/**残機が一つになったら描画させる牛もリセットしておく*/
		m_cowIcons[i].isCrisis = false;

	}
	m_lostCount = 0;

	/** 点滅状態もリセットさせる*/
	m_blinkFreamCount = 0;
	m_isBlinkVisible = true;
}


void CowLivesUI::DecreaseLife()
{
	/** すでに残機が全滅していたら何もしない*/
	if (GetIsAllCowLost())
	{
		return;
	}

	/** まだ×になっていない最初のアイコンを×にする*/
	for (int i = 0; i < COW_LIFE_NUM; i++)
	{
		if (!m_cowIcons[i].isDead)
		{
			m_cowIcons[i].isDead = true;

			/** 画像を死んでいる牛に切り替える*/
			m_cowIcons[i].render.Init(COW_DEAD_FILEPATH, WIDTH, HEIGHT);

			Vector3 pos = ICON_START_POS;
			/** アイコンの間隔をあける*/
			pos.x -= ICON_SPACING * i;
			/** ポジションをセットする*/
			m_cowIcons[i].render.SetPosition(pos);
			/** 画像を更新する*/
			m_cowIcons[i].render.Update();

			/** 牛が連れ去られたら牛のカウントを増やす*/
			m_lostCount++;
			break;
		}
	}


	/** 残り残機が１つだったら、残り一つの残機を薄い赤色の牛のアイコンにする*/
	if (COW_LIFE_NUM - m_lostCount == 1)
	{
		for (int i = 0; i < COW_LIFE_NUM; i++)
		{
				if (!m_cowIcons[i].isDead)
				{
					m_cowIcons[i].render.Init(COW_CRISIS_FILEPATH, WIDTH, HEIGHT);

					Vector3 pos = ICON_START_POS;
					pos.x -= ICON_SPACING * i;
					m_cowIcons[i].render.SetPosition(pos);
					m_cowIcons[i].render.Update();

					/** 点滅させるためフラグを立てる*/
					m_cowIcons[i].isCrisis = true;

					/** 点滅を毎回同じタイミングで始めさせるためリセットしておく*/
					m_blinkFreamCount = 0;
					m_isBlinkVisible = true;
					break;
				}
			}
		}

	/** 3匹すべて×になったらゲームオーバーにする*/
	if (GetIsAllCowLost())
	{
		m_game = FindGO<Game>("game");
		if (m_game)
		{
			m_game->Death();
		}
	}
}


void CowLivesUI::Render(RenderContext& rc)
{
	/**フェード完了までUIの表示を遅らす*/
	LoadingScene* lodingScene = FindGO<LoadingScene>("loading");
	if (lodingScene != nullptr && !lodingScene->GetLoadingEnd())
	{
		return;
	}
	/** Pauseが存在しているかつPause中なら描画しない*/
	if (m_pause && m_pause->GetIsPause())
	{
		return;
	}
	/** ゲームが存在しているかつタイムアウトしていたら描画しない*/
	if (m_game && m_game->GetIsTimeOut())
	{

	}



	for (int i = 0; i < COW_LIFE_NUM; i++)
	{

		/** 残り一つの残機は点滅させるので非表示フレームでは描画しない*/
		if (m_cowIcons[i].isCrisis && !m_isBlinkVisible)
		{
			continue;
		}
		m_cowIcons[i].render.Draw(rc);
	}
}

