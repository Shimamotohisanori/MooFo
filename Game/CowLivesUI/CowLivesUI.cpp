#include "stdafx.h"
#include "CowLivesUI.h"
#include"GameScene/Game.h"
#include"GameScene/LoadingScene.h"
#include"Pause/Pause.h"
#include"FadeManager/FadeManager.h"
#include"SoundManager/SoundManager.h"
#include"Tutorial/TutorialManager.h"
#include"CountDown/CountDown.h"
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
	const Vector3 ICON_START_POS = Vector3(-660.0f,460.0f,0.0f);

	/** アイコン同士の間隔 */
	constexpr float ICON_SPACING = 110.0f;

	/** RESCUEのテキスト*/
	const char* RESCUE_TEXT = "Assets/sprite/TextBeforGameOver/Rescue.dds";

	/** Failedのテキスト*/
	const char* FAILED_TEXT = "Assets/sprite/TextBeforGameOver/Failed.dds";
	/** …のテキスト*/
	const char* TEN_TEXT = "Assets/sprite/TextBeforGameOver/Ten.dds";

	/** テキストサイズ*/
	constexpr int TEXT_WIDTH = 600;
	constexpr int TEXT_HEIGHT = 300;
	/** 「FAILED」のみ少し小さいので大きさを変える*/
	constexpr int TEXT_FAILED_WIDTH = 600;
	constexpr int TEXT_FAILED_HEIGHT = 300;

	/** 「RESCUE」テキストの表示位置*/
	const Vector3 RESCUE_TEXT_POS = Vector3(-480.0f,50.0f,0.0f);
	/** 「FAILED」テキストの表示位置*/
	const Vector3 FAILED_TEXT_POS = Vector3(100.0f,50.0f,0.0f);
	/** 「…」テキストの表示位置*/
	const Vector3 TEN_TEXT_POS = Vector3(550.0f, 50.0f, 0.0f);

	/** テキスト拡大演出の目標スケールの実体定義*/
	const Vector3 TEXT_TARGET_SCALE = Vector3(1.0f, 1.0f, 1.0f);
}


CowLivesUI::CowLivesUI()
{

}

CowLivesUI::~CowLivesUI()
{
	DeleteGO(m_failedRescueSE);
}


bool CowLivesUI::Start()
{

	m_tenTextRender.Update();

	/** アイコンの初期化を行う*/
	ResetLives();
	return true;
}


void CowLivesUI::Update()
{
	m_pause = FindGO<Pause>("pause");

	m_countDown = FindGO<CountDown>("countdown");

	m_fadeManager = FindGO<FadeManager>("fadeManager");

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

	/** ゲームオーバー演出を１フレーム分進める*/
	UpdateGameOverSequence();
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

	m_gameOverPhase = GameOverPhase::None;
	m_isNotOperation = false;
}

void CowLivesUI::StartGameOverSequence()
{
	/** テキストのスケールを小さい状態からスタートさせる*/
	m_texScale = Vector3(0.0f, 0.0f, 1.0f);

	/** 「RESCUE」の文字を初期化する*/
	m_rescueTextRender.Init(RESCUE_TEXT, TEXT_WIDTH, TEXT_HEIGHT);
	m_rescueTextRender.SetPosition(RESCUE_TEXT_POS);
	m_rescueTextRender.SetScale(m_texScale);
	m_rescueTextRender.Update();

	/** 「FAILED」の文字を初期化する*/
	m_failedTextRender.Init(FAILED_TEXT, TEXT_WIDTH, TEXT_HEIGHT);
	m_failedTextRender.SetPosition(FAILED_TEXT_POS);
	m_failedTextRender.SetScale(m_texScale);
	m_failedTextRender.Update();

	/** 「…」の文字を初期化する*/
	m_tenTextRender.Init(TEN_TEXT, TEXT_WIDTH, TEXT_HEIGHT);
	m_tenTextRender.SetPosition(TEN_TEXT_POS);
	m_tenTextRender.SetScale(m_texScale);
	m_tenTextRender.Update();

	m_failedTextTimer = 0.0f;
	/** 文字を表示中にする*/
	m_gameOverPhase = GameOverPhase::ShowingFailedText;
	/**文字が表示されていたら操作を受け付けないようにする*/
	if (m_gameOverPhase == GameOverPhase::ShowingFailedText)
	{
		/** まだSEが再生されていなければ流す*/
		if (!m_isPlayFailedRescueSE)
		{
			/**サウンドマネージャーを取得する*/
			SoundManager* soundManager = FindGO<SoundManager>("soundmanager");

			/** 「救助失敗」時に流すSEを再生する*/
			m_failedRescueSE = soundManager->PlayingSE(SoundSE::enFailedRescue, false);
			m_isPlayFailedRescueSE = true;
		}
	
		m_isNotOperation = true;
	}
	/** テキストの拡大演出のスケールをリセット*/
	m_texScale = Vector3(0.0f, 0.0f, 1.0f);
}


void CowLivesUI::UpdateGameOverSequence()
{
	switch (m_gameOverPhase)
	{
	case GameOverPhase::ShowingFailedText:
		m_failedTextTimer += g_gameTime->GetFrameDeltaTime();

		/** テキストの大きさを徐々に大きくする*/
		m_texScale.x += (TEXT_TARGET_SCALE.x - m_texScale.x) * 0.1f;
		m_texScale.y += (TEXT_TARGET_SCALE.y - m_texScale.y) * 0.1f;

		/** それぞれの文字の大きさをセットする*/
		m_rescueTextRender.SetScale(m_texScale);
		m_rescueTextRender.Update();

		m_failedTextRender.SetScale(m_texScale);
		m_failedTextRender.Update();

		m_tenTextRender.SetScale(m_texScale);
		m_tenTextRender.Update();

		if (m_failedTextTimer >= FAILED_TEXT_DISPLAY_TIME)
		{
			/** 表示時間が経過したらフェード(暗転)を開始する*/
			if (m_fadeManager)
			{
				m_fadeManager->SetFadeIn();
			}
			m_gameOverPhase = GameOverPhase::WaitingFade;
		}
		break;

	case GameOverPhase::WaitingFade:
		/** 暗転が完了したらゲームオーバーに移行させる*/
		if (m_fadeManager && m_fadeManager->IsFadeInComplete())
		{
			m_game = FindGO<Game>("game");
			if (m_game)
			{
				/** ゲームオーバーの処理を呼ぶ*/
				m_game->Death();
			}
			m_gameOverPhase = GameOverPhase::None;
		}
		break;

	case GameOverPhase::None:
	default:
		break;
	}
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

	/** 3匹すべて×になったら、RESCUE FAILEDの演出を開始する*/
	if (GetIsAllCowLost())
	{
		StartGameOverSequence();
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
		return;
	}

	/** フェード(暗転)待ち中は牛の残機UIも文字も表示しない*/
	if (m_gameOverPhase == GameOverPhase::WaitingFade)
	{
		return;
	}
	/** カウントダウン中は描画しない*/
	if (m_countDown && m_countDown->GetCountDown())
	{
		return;
	}

	/** チュートリアルのステップ遷移中(フェードイン～フェードアウト完了まで)はUIを描画しない */
	Game* game = FindGO<Game>("game");
	if (game&&game->GetIsTutorialMode())
	{
		TutorialManager* tutorial = FindGO<TutorialManager>("tutorialmanager");
		if (tutorial && tutorial->IsTransitioning())
		{
			return;
		}
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
		/** ゲームオーバーの状態が通常状態じゃなければ*/
		if (m_gameOverPhase != GameOverPhase::None)
		{
			m_rescueTextRender.Draw(rc);
			m_failedTextRender.Draw(rc);
			m_tenTextRender.Draw(rc);
		}
	}