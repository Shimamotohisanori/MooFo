#include "stdafx.h"
#include "Pause/Pause.h"
#include "GameScene/Game.h"
#include "SoundManager/SoundManager.h"
#include "SoundPause.h"
#include "GameScene/Title.h"
#include "CountDown/CountDown.h"
#include "GameScene/LoadingScene.h"
namespace
{
	const char* PAUSE_BACKGROUND_PATH = "Assets/sprite/PauseUI/pauseBackGround2.dds";
	const char* PAUSE_QUITE_PATH      = "Assets/sprite/PauseUI/quiteGame.dds";
	const char* PAUSE_RESUME_PATH     = "Assets/sprite/PauseUI/resumeGame.dds";
	const char* PAUSE_SOUND_PATH      = "Assets/sprite/PauseUI/sound.dds";
	const char* PAUSE_BLACK_SOUND_PATH  = "Assets/sprite/PauseUI/soundBlack.dds";
	const char* PAUSE_BLACK_RESUME_PATH = "Assets/sprite/PauseUI/resumeGameBlack.dds";
	const char* PAUSE_BLACK_QUITE_PATH  = "Assets/sprite/PauseUI/quiteGameBlack.dds";
	
	const Vector3 QUITEPOS  = { 0.0f, -75.0f,0.0f };
	const Vector3 RESUMEPOS = { 0.0f, 150.0f,0.0f };
	const Vector3 SOUNDPOS  = { 0.0f,-300.0f,0.0f };
	const Vector3 QUITE_BLACKPOS  = {0.0f, 150.0f,0.0f};
	const Vector3 RESUME_BLACKPOS = {0.0f, -80.0f,0.0f};
	const Vector3 SOUND_BLACKPOS  = {0.0f,-310.0f,0.0f};
}

bool Pause::Start()
{
	m_pauseBackGround.Init(PAUSE_BACKGROUND_PATH,1980.0f,1080.0f);
	m_pauseBackGround.Update();

	m_quiteSprite.Init(PAUSE_QUITE_PATH,650.0f,550.0f);
	m_quiteSprite.SetPosition(QUITEPOS);
	m_quiteSprite.Update();

	m_resumeSprite.Init(PAUSE_RESUME_PATH,650.0f,550.0f);
	m_resumeSprite.SetPosition(RESUMEPOS);
	m_resumeSprite.Update();

	m_soundSprite.Init(PAUSE_SOUND_PATH,650.0f,550.0f);
	m_soundSprite.SetPosition(SOUNDPOS);
	m_soundSprite.Update();

	m_soundBlackSprite.Init(PAUSE_BLACK_SOUND_PATH, 780.0f, 620.0f);
	m_soundBlackSprite.SetPosition(SOUND_BLACKPOS);
	m_soundBlackSprite.Update();

	m_quiteBlackSprite.Init(PAUSE_BLACK_QUITE_PATH, 710.0f, 590.0f);
	m_quiteBlackSprite.SetPosition(QUITE_BLACKPOS);
	m_quiteBlackSprite.Update();

	m_resumeBlackSprite.Init(PAUSE_BLACK_RESUME_PATH, 640.0f, 540.0f);
	m_resumeBlackSprite.SetPosition(RESUME_BLACKPOS);
	m_resumeBlackSprite.Update();

	/** ポーズ画面の音量設定クラスを生成 */
	m_soundPause = NewGO<SoundPause>(0, "soundpause");

	m_soundPause->Deactivate();


	m_game = FindGO<Game>("game");
	m_choiceSound = FindGO<SoundManager>("soundmanager");
	

	m_isPause = true;

	return true;
}

void Pause::Update()
{
	CountDown* countDown = FindGO<CountDown>("countdown");

	/** カウントダウンが終わるまではポーズ画面の操作を受け付けない */
	if (!countDown || countDown->GetCountDown())
	{
		return;
	}

	/** タイムアウトしている場合はポーズ画面の操作を受け付けない */
	if (m_game && m_game->GetIsTimeOut())
	{
		return;
	}

	Choice();
	Select();

	/** 上下の入力で選択している場所の番号を変える */
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		m_countNumber++;
		p_chiceSE = m_choiceSound->PlayingSE(SoundSE::enChoiceSE, false);
	}
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		m_countNumber--;
		p_chiceSE = m_choiceSound->PlayingSE(SoundSE::enChoiceSE, false);
	}

}

void Pause::Choice()
{
	/** 範囲外に言ったら一番下に戻る。 */
	if (m_countNumber == -1)
	{
		m_countNumber = 2;
	}
	/** 範囲外に行ったら先頭に戻る。 */
	if (m_countNumber >= 3)
	{
		m_countNumber = 0;
	}
}

void Pause::Select()
{
	/** 決定ボタンが押された場合の処理 */
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		/** 決定SEを鳴らす */
		p_DecisionSE = m_choiceSound->PlayingSE(SoundSE::enDecisionSE, false);
		
		/** 選択している場所の番号が0のときはゲームを終了する */
		if (m_countNumber == 0)
		{
			m_isPause = false;
			Deactivate();
			m_game->m_isSound = false;
		}

		/** 選択している場所の番号が1のときはタイトルに戻る */
		else if (m_countNumber == 1)
		{
			/** ゲームの牛を全て削除する */
			if (m_game)
			{
				m_game->KillAllCows();
			}

			m_loadingScene = NewGO<LoadingScene>(0, "loading");
			m_loadingScene->SetLoadType(LoadingScene::LoadType::ToTitleScene);
			m_loadingScene->SetNextScene([]()
				{
					/** タイトルの画像を呼び出す*/
					NewGO<Title>(0, "title");
				});

			DeleteGO(m_game);
			DeleteGO(this);
		}

		/** 選択している場所の番号が2のときは音量設定画面に行く */
		else if (m_countNumber == 2)
		{
			m_soundPause->Activate();
			m_soundPause->SetCount(0);
			Deactivate();
		}
	}
}

void Pause::Render(RenderContext& rc)
{
	m_pauseBackGround.Draw(rc);
	m_quiteSprite.Draw(rc);
	m_resumeSprite.Draw(rc);
	m_soundSprite.Draw(rc);
	m_quiteBlackSprite.Draw(rc);
	m_resumeBlackSprite.Draw(rc);
	m_soundBlackSprite.Draw(rc);

	/** ポーズ画面で選択された位置が分かりやすいように色を透過させるようにする。 */
	if (m_countNumber == 0)
	{
		m_quiteBlackSprite.SetMulColor
		(
			Vector4(1.0f, 1.0f, 1.0f, 0.0f)
		);

		m_resumeBlackSprite.SetMulColor
		(
			Vector4(1.0f, 1.0f, 1.0f, 0.5f)
		);
		m_soundBlackSprite.SetMulColor
		(
			Vector4(1.0f, 1.0f, 1.0f, 0.5)
		);
	}

	if (m_countNumber == 1)
	{
		m_quiteBlackSprite.SetMulColor
		(
			Vector4(1.0f, 1.0f, 1.0f, 0.5f)
		);

		m_resumeBlackSprite.SetMulColor
		(
			Vector4(1.0f, 1.0f, 1.0f, 0.0f)
		);
		m_soundBlackSprite.SetMulColor
		(
			Vector4(1.0f, 1.0f, 1.0f, 0.5)
		);
	}

	if (m_countNumber == 2)
	{
		m_quiteBlackSprite.SetMulColor
		(
			Vector4(1.0f, 1.0f, 1.0f, 0.5f)
		);

		m_resumeBlackSprite.SetMulColor
		(
			Vector4(1.0f, 1.0f, 1.0f, 0.5f)
		);
		m_soundBlackSprite.SetMulColor
		(
			Vector4(1.0f, 1.0f, 1.0f, 0.0)
		);
	}
}