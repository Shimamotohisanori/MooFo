#include "stdafx.h"
#include "SoundManager/SoundManager.h"
#include "Pause/Pause.h"
#include "GameScene/Game.h"
#include "SoundPause.h"
#include "GameScene/Title.h"
namespace
{
	//const std::string PAUSE_SPRITE_FILE_PASS = "Assets/sptite/";
	const Vector3 QUITEPOS = { 0.0f,-100.0f,0.0f };
	const Vector3 RESUMEPOS = { 0.0f,150.0f,0.0f };
	const Vector3 SOUNDPOS = { 0.0f,-300.0f,0.0f };
	const Vector3 ARROWPOS = { -350.0f,150.0f,0.0f };
	const Vector3 SECONDARROWPOS = { -350.0f,-105.0f,0.0f };
	const Vector3 THIRDARROWPOS = { -170.0f,-300.0f,0.0f };
	const Vector3 ARROWSCALE = { 1.0f,1.0f,1.0f };
	const Vector3 ARROWSOUNDSCALE = { 0.5f,0.5f,1.0f };
}bool Pause::Start()
{
	m_pauseBackGround.Init("Assets/sprite/PauseUI/pauseBackGround.dds",1980.0f,1080.0f);
	m_pauseBackGround.Update();

	m_quiteSprite.Init("Assets/sprite/PauseUI/quiteGame.dds",750.0f,650.0f);
	m_quiteSprite.SetPosition(QUITEPOS);
	m_quiteSprite.Update();

	m_resumeSprite.Init("Assets/sprite/PauseUI/resumeGame.dds",650.0f,550.0f);
	m_resumeSprite.SetPosition(RESUMEPOS);
	m_resumeSprite.Update();

	m_soundSprite.Init("Assets/sprite/PauseUI/sound.dds",350.0f,250.0f);
	m_soundSprite.SetPosition(SOUNDPOS);
	m_soundSprite.Update();

	m_arrowSprite.Init("Assets/sprite/PauseUI/Arrow.dds", 150.0f, 150.0f);

	//m_settingSprite.Init("Assets/setting.dds",300.0f,200.0f);
	//m_bgmSprite.Init("Assets/BGM.dds",100.0f,150.0f);
	//m_sePrite.Init("Assets/SE.dds",100.0f,150.0f);
	//m_volumeSprite.Init("Assets/volume.dds",100.0f,100.0f);
	//m_notVolumeSprite.Init("Assets/notVolume.dds",100.0f,100.0f);

	/** サウンドのポーズ画面を出す */
	m_soundPause = NewGO<SoundPause>(0, "soundpause");

	m_soundPause->Deactivate();


	m_game = FindGO<Game>("game");

	m_choiceSound = FindGO<SoundManager>("soundmanager");
	return true;
}

void Pause::Update()
{
	Choose();
	Select();
	/** 上下ボタンでカウントの数値を変えてBGMやSEを選択できるようにする */
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

void Pause::Choose()
{
	/** 選択肢以外の数値に変わったら元に戻す */
	if (m_countNumber == -1)
	{
		m_countNumber = 2;
	}

	/** 数値に当てはまるif文がある場合矢印のポジションを設定して選択している物を表している */
	if (m_countNumber == 0)
	{
		m_arrowSprite.SetPosition(ARROWPOS);
		m_arrowSprite.SetScale(ARROWSCALE);
	}
	else if (m_countNumber == 1)
	{
		m_arrowSprite.SetPosition(SECONDARROWPOS);
		m_arrowSprite.SetScale(ARROWSCALE);
	}
	else if (m_countNumber == 2)
	{
		m_arrowSprite.SetPosition(THIRDARROWPOS);
		m_arrowSprite.SetScale(ARROWSOUNDSCALE);
	}
	else if (m_countNumber == 3)
	{
		m_countNumber = 0;
	}
	m_arrowSprite.Update();
}

void Pause::Select()
{
	/** それぞれの選択した結果をif文で処理し、実行させる */
	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		p_DecisionSE = m_choiceSound->PlayingSE(SoundSE::enDecisionSE, false);
		if (m_countNumber == 0)
		{
			Deactivate();
			m_game->m_isSound = false;
		}
		else if (m_countNumber == 1)
		{
			NewGO<Title>(0,"title");
			DeleteGO(m_game);
			DeleteGO(this);
		}
		else if (m_countNumber == 2)
		{
			m_soundPause->Activate();
			m_soundPause->SetCount(0);
			Deactivate();
		}
	}
}

//void Pause::StopBGM()
//{
//	/** もしm_gameがアクティブじゃなかったら */
//	if (m_game->IsActive() == false)
//	{
//		/** ゲーム内のBGMをストップさせる */
//		SoundManager::
//	}
//}

void Pause::Render(RenderContext& rc)
{
	m_pauseBackGround.Draw(rc);
	m_quiteSprite.Draw(rc);
	m_resumeSprite.Draw(rc);
	m_soundSprite.Draw(rc);
	m_arrowSprite.Draw(rc);
}