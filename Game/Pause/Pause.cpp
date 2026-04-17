#include "stdafx.h"
#include "Pause/Pause.h"
#include "GameScene/Game.h"
#include "SoundManager/SoundManager.h"
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
	m_pauseBackGround.Init("Assets/sprite/PauseUI/pauseBackGround2.dds",1980.0f,1080.0f);
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

	/** ポーズ画面の音量設定クラスを生成 */
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

void Pause::Choose()
{
	// ** 選択している場所の番号が-1になったら2にする */
	if (m_countNumber == -1)
	{
		m_countNumber = 2;
	}

	/** 選択している場所の番号が3になったら0にする */
	if (m_countNumber == 0)
	{
		m_arrowSprite.SetPosition(ARROWPOS);
		m_arrowSprite.SetScale(ARROWSCALE);
	}

	/** 選択している場所の番号が1になったら矢印の位置を変える */
	else if (m_countNumber == 1)
	{
		m_arrowSprite.SetPosition(SECONDARROWPOS);
		m_arrowSprite.SetScale(ARROWSCALE);
	}

	/** 選択している場所の番号が2になったら矢印の位置を変える */
	else if (m_countNumber == 2)
	{
		m_arrowSprite.SetPosition(THIRDARROWPOS);
		m_arrowSprite.SetScale(ARROWSOUNDSCALE);
	}
	
	/** 選択している場所の番号が3になったら0にする */
	else if (m_countNumber == 3)
	{
		m_countNumber = 0;
	}
	m_arrowSprite.Update();
}

void Pause::Select()
{
	/** 決定ボタンが押された場合の処理 */
	if (g_pad[0]->IsTrigger(enButtonStart))
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
			NewGO<Title>(0,"title");
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
	m_arrowSprite.Draw(rc);
}