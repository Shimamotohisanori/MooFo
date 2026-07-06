#include "stdafx.h"
#include "SoundManager/SoundManager.h"
#include "SoundPause.h"
#include "Pause/Pause.h"

namespace
{
	const Vector3 RETURNPOS = { 0.0f,-380.0f,0.0f };
	const Vector3 BLACKPOS = { 0.0f,-375.0f,0.0f };
	const Vector3 VOLUMEPOS = { 480.0f,180.0f,0.0f };
	const Vector3 NOT_VOLUMEPOS = { -480.0f,180.0f,0.0f };
	const Vector3 SE_VOLUMEPOS = { 480.0f,0.0f,0.0f };
	const Vector3 SE_NOT_VOLUMEPOS = { -480.0f,0.0f,0.0f };
	const Vector3 VOICE_VOLUMEPOS = { 480.0f,-170.0f,0.0f };
	const Vector3 VOICE_NOT_VOLUMEPOS = { -480.0f,-170.0f,0.0f };
	const Vector3 BGMPOS = { 0.0f,250.0f,0.0f };
	const Vector3 SEPOS = { 0.0f,80.0f,0.0f };
	const Vector3 VOICEPOS = { 0.0f,-90.0f,0.0f };
	const Vector3 SOUNDICONPOS = { -370.0f,180.0f,0.0f };
	const Vector3 SE_ICONPOS = { -370.0f,0.0f,0.0f };
	const Vector3 VOICE_ICONPOS = { -370.0f,-170.0f,0.0f };
	const Vector3 BLOWNBARPOS = { -398.0f,179.0f,0.0f };
	const Vector3 SE_BLOWNBARPOS = { -398.0f, -1.0f,0.0f };
	const Vector3 VOICE_BLOWNBARPOS = { -398.0f,-169.0f,0.0f };
	const Vector3 BLOWNBARINITIALSCALE = { 0.0f,0.0f,0.0f };
	const Vector2 BLOWN_BAR = { 0.0f,0.5 };
}

bool SoundPause::Start()
{
	m_backGroundSprite.Init("Assets/sprite/PauseUI/pauseBackGround2.dds", 1980.0f, 1080.0f);
	m_backGroundSprite.Update();

	m_settingSprite.Init("Assets/sprite/PauseUI/setting3.dds", 1200.0f, 700.0f);
	m_settingSprite.Update();

	m_grayBarSprite.Init("Assets/sprite/PauseUI/graybar2.dds", 1200.0f, 700.0f);
	m_grayBarSprite.Update();

	m_buttonReturnSprite.Init("Assets/sprite/PauseUI/buttonReturn.dds", 450.0f, 200.0f);
	m_buttonReturnSprite.SetPosition(RETURNPOS);
	m_buttonReturnSprite.Update();

	m_volumeSprite.Init("Assets/sprite/PauseUI/volume.dds", 230.0f, 230.0f);
	m_volumeSprite.SetPosition(VOLUMEPOS);
	m_volumeSprite.Update();

	m_notVolumeSprite.Init("Assets/sprite/PauseUI/notVolume.dds", 245.0f, 245.0f);
	m_notVolumeSprite.SetPosition(NOT_VOLUMEPOS);
	m_notVolumeSprite.Update();

	m_seVolumeSprite.Init("Assets/sprite/PauseUI/volume.dds", 230.0f, 230.0f);
	m_seVolumeSprite.SetPosition(SE_VOLUMEPOS);
	m_seVolumeSprite.Update();

	m_seNotVolumeSprite.Init("Assets/sprite/PauseUI/notVolume.dds", 245.0f, 245.0f);
	m_seNotVolumeSprite.SetPosition(SE_NOT_VOLUMEPOS);
	m_seNotVolumeSprite.Update();

	m_voiceVolumeSprite.Init("Assets/sprite/PauseUI/volume.dds", 230.0f, 230.0f);
	m_voiceVolumeSprite.SetPosition(VOICE_VOLUMEPOS);
	m_voiceVolumeSprite.Update();

	m_voiceNotVolumeSprite.Init("Assets/sprite/PauseUI/notVolume.dds", 245.0f, 245.0f);
	m_voiceNotVolumeSprite.SetPosition(VOICE_NOT_VOLUMEPOS);
	m_voiceNotVolumeSprite.Update();

	m_bgmSprite.Init("Assets/sprite/PauseUI/BGM.dds", 250.0f, 300.0f);
	m_bgmSprite.SetPosition(BGMPOS);
	m_bgmSprite.Update();

	m_seSprite.Init("Assets/sprite/PauseUI/SE.dds", 250.0f, 300.0f);
	m_seSprite.SetPosition(SEPOS);
	m_seSprite.Update();

	m_voiceSprite.Init("Assets/sprite/PauseUI/VoiceUI.dds", 200.0f, 250.0f);
	m_voiceSprite.SetPosition(VOICEPOS);
	m_voiceSprite.Update();

	m_soundIconSprite.Init("Assets/sprite/PauseUI/soundIcon.dds", 290.0f, 240.0f);
	m_soundIconSprite.SetPosition(SOUNDICONPOS);
	m_soundIconSprite.Update();

	m_seIconSprite.Init("Assets/sprite/PauseUI/seIcon.dds", 290.0f, 240.0f);
	m_seIconSprite.SetPosition(SE_ICONPOS);
	m_seIconSprite.Update();

	m_voiceIconSprite.Init("Assets/sprite/PauseUI/VoiceIcon.dds", 290.0f, 240.0f);
	m_voiceIconSprite.SetPosition(VOICE_ICONPOS);
	m_voiceIconSprite.Update();

	m_blownBarSprite.Init("Assets/sprite/PauseUI/BlownBar.dds", 790.0f, 260.0f);
	m_blownBarSprite.SetPosition(BLOWNBARPOS);
	m_blownBarSprite.SetPivot(BLOWN_BAR);
	m_blownBarSprite.SetScale(BLOWNBARINITIALSCALE);
	m_blownBarSprite.Update();

	m_seBlownBarSprite.Init("Assets/sprite/PauseUI/BlownBar.dds", 790.0f, 260.0f);
	m_seBlownBarSprite.SetPosition(SE_BLOWNBARPOS);
	m_seBlownBarSprite.SetPivot(BLOWN_BAR);
	m_seBlownBarSprite.SetScale(BLOWNBARINITIALSCALE);
	m_seBlownBarSprite.Update();

	m_voiceBlownBarSprite.Init("Assets/sprite/PauseUI/BlownBar.dds", 790.0f, 260.0f);
	m_voiceBlownBarSprite.SetPosition(VOICE_BLOWNBARPOS);
	m_voiceBlownBarSprite.SetPivot(BLOWN_BAR);
	m_voiceBlownBarSprite.SetScale(BLOWNBARINITIALSCALE);
	m_voiceBlownBarSprite.Update();

	m_bgmBlackIcon.Init("Assets/sprite/PauseUI/blackIcon.dds", 290.0f, 240.0f);
	m_bgmBlackIcon.SetPosition(SOUNDICONPOS);
	m_bgmBlackIcon.Update();

	m_seBlackIcon.Init("Assets/sprite/PauseUI/blackIcon.dds", 290.0f, 240.0f);
	m_seBlackIcon.SetPosition(SE_ICONPOS);
	m_seBlackIcon.Update();

	m_voiceBlackIcon.Init("Assets/sprite/PauseUI/blackIcon.dds", 290.0f, 240.0f);
	m_voiceBlackIcon.SetPosition(VOICE_ICONPOS);
	m_voiceBlackIcon.Update();

	m_blackSprite.Init("Assets/sprite/PauseUI/Black.dds", 430.0f, 240.0f);
	m_blackSprite.SetPosition(BLACKPOS);
	m_blackSprite.Update();

	m_pause = FindGO<Pause>("pause");

	m_choiceSound = FindGO<SoundManager>("soundmanager");

	/** SoundPauseクラスが生成されたときに
	 *  SoundManagerの音量を参考にしてBGM,SEに反映 */
	m_bgmVolume = m_choiceSound->m_bgmVolume;
	m_seVolume = m_choiceSound->m_seVolume;

	/** UIに反映 */
	UpdateBGMUI();
	UpdateSEUI();
	UpdateVoiceUI();

	return true;
}

void SoundPause::Update()
{
	ButtonCount();
}

void SoundPause::ButtonCount()
{
	/** ↓下キーで選択項目を進める */
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		/** 選択番号を増やす */
		m_Count++; 
		p_chiceSE = m_choiceSound->PlayingSE(SoundSE::enChoiceSE, false); // カーソル移動音
	}

	/** ↓上キーで選択項目を戻す */
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		/** 選択番号を減らす */
		m_Count--;
		p_chiceSE = m_choiceSound->PlayingSE(SoundSE::enChoiceSE, false);
	}

	/**↓範囲外（下限）に行ったら一番下へループ */
	if (m_Count == -1)
	{
		m_Count = 2;
	}

	/**↓BGM音量調整モード */
	if (m_Count == 0)
	{
		Barbgm();
	}

	/** ↓SE音量調整モード */
	if (m_Count == 1)
	{
		Barse();
	}

	/** ↓Voice音量調整モード */
	if (m_Count == 2)
	{
		Barvoice();
	}

	/** ↓「戻る」選択中 */
	if (m_Count == 3)
	{
		/** スタートボタンでポーズ画面へ戻る */
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			p_decisionSE = m_choiceSound->PlayingSE(SoundSE::enDecisionSE, false);
			/** ポーズ画面を有効化 */
			m_pause->Activate();
			/** カーソル初期化 */
			m_pause->SetCountNumber(0);
			/** この画面を無効化 */
			Deactivate();               
		}
	}

	/** ↓範囲外（上限）に行ったら先頭へループ */
	if (m_Count == 4)
	{
		m_Count = 0;
	}
}

void SoundPause::Barbgm()
{
	/** 右キーで音量アップ（最大1.0まで）*/
	if (m_bgmVolume < 1.0f)
	{
		if (g_pad[0]->IsPress(enButtonRight))
		{
			m_bgmVolume += 0.01f;
		}
	}

	/** 左キーで音量ダウン（最小0.0まで）*/
	if (m_bgmVolume > 0.001f)
	{
		if (g_pad[0]->IsPress(enButtonLeft))
		{
			m_bgmVolume -= 0.01f;
		}
	}

	/** BGM音量をSoundManagerに反映 */
	m_choiceSound->m_bgmVolume = m_bgmVolume;

	/** UI更新 */
	UpdateBGMUI();
}

void SoundPause::Barse()
{
	/** 右キーでSE音量アップ */
	if (m_seVolume < 1.0f)
	{
		if (g_pad[0]->IsPress(enButtonRight))
		{
			m_seVolume += 0.01f;
		}
	}

	/** 左キーでSE音量ダウン */
	if (m_seVolume > 0.001f)
	{
		if (g_pad[0]->IsPress(enButtonLeft))
		{
			m_seVolume -= 0.01f;
		}
	}

	/** SE音量をSoundManagerに反映 */
	m_choiceSound->m_seVolume = m_seVolume;

	UpdateSEUI();
}

void SoundPause::Barvoice()
{
	/** 右キーでVoice音量アップ */
	if (m_voiceVolume < 1.0f)
	{
		if (g_pad[0]->IsPress(enButtonRight))
		{
			m_voiceVolume += 0.01f;
		}
	}

	/** 左キーでVoice音量ダウン */
	if (m_voiceVolume > 0.001f)
	{
		if (g_pad[0]->IsPress(enButtonLeft))
		{
			m_voiceVolume -= 0.01f;
		}
	}

	UpdateVoiceUI();
}

void SoundPause::UpdateBGMUI()
{
	/** 音量に応じたアイコン座標計算 */
	float posX = -370.0f + m_bgmVolume * 730.0f;

	/** 音量アイコン更新 */
	m_soundIconSprite.SetPosition(Vector3(posX, 180.0f, 0.0f));
	m_soundIconSprite.Update();

	/** 音量バー更新 */
	m_blownBarSprite.SetScale(Vector3(m_bgmVolume, 1.0f, 0.0f));
	m_blownBarSprite.Update();

	/** 黒アイコン更新 */
	m_bgmBlackIcon.SetPosition(Vector3(posX, 180.0f, 0.0f));
	m_bgmBlackIcon.Update();
}

void SoundPause::UpdateSEUI()
{
	float posX = -370.0f + m_seVolume * 730.0f;

	m_seIconSprite.SetPosition(Vector3(posX, 0.0f, 0.0f));
	m_seIconSprite.Update();

	m_seBlownBarSprite.SetScale(Vector3(m_seVolume, 1.0f, 0.0f));
	m_seBlownBarSprite.Update();

	m_seBlackIcon.SetPosition(Vector3(posX, 0.0f, 0.0f));
	m_seBlackIcon.Update();
}

void SoundPause::UpdateVoiceUI()
{
	float posX = -370.0f + m_voiceVolume * 730.0f;

	m_voiceIconSprite.SetPosition(Vector3(posX, -170.0f, 0.0f));
	m_voiceIconSprite.Update();
	
	m_voiceBlownBarSprite.SetScale(Vector3(m_voiceVolume, 1.0f, 0.0f));
	m_voiceBlownBarSprite.Update();
	
	m_voiceBlackIcon.SetPosition(Vector3(posX, -170.0f, 0.0f));
	m_voiceBlackIcon.Update();
}

void SoundPause::Render(RenderContext& rc)
{
	m_backGroundSprite.Draw(rc);
	m_grayBarSprite.Draw(rc);
	m_buttonReturnSprite.Draw(rc);

	/** BGM音量バー描画 */
	if (m_bgmVolume > 0.05f)
	{
		m_blownBarSprite.Draw(rc);
	}

	/** SE音量バー描画 */
	if (m_seVolume > 0.05f)
	{
		m_seBlownBarSprite.Draw(rc);
	}

    /** Voice音量バー描画 */
	if (m_voiceVolume > 0.05f)
	{
		m_voiceBlownBarSprite.Draw(rc);
	}

	m_settingSprite.Draw(rc);
	m_volumeSprite.Draw(rc);
	m_notVolumeSprite.Draw(rc);
	m_seVolumeSprite.Draw(rc);
	m_seNotVolumeSprite.Draw(rc);
	m_voiceVolumeSprite.Draw(rc);
	m_voiceNotVolumeSprite.Draw(rc);
	m_bgmSprite.Draw(rc);
	m_seSprite.Draw(rc);
	m_voiceSprite.Draw(rc);

	/** BGM選択アイコン描画 */
	if (m_Count == 0)
	{
		m_soundIconSprite.Draw(rc);
	}
	else
	{
		m_bgmBlackIcon.Draw(rc);
	}

	/** SE選択中アイコン描画 */
	if (m_Count == 1)
	{
		m_seIconSprite.Draw(rc);
	}
	else
	{
		m_seBlackIcon.Draw(rc);
	}

	/** Voice選択中アイコン描画 */
	if (m_Count == 2)
	{
		m_voiceIconSprite.Draw(rc);
	}
	else
	{
		m_voiceBlackIcon.Draw(rc);
	}

	/** 終了ボタン選択時は背景非表示 */
	if (m_Count == 3)
	{
		m_blackSprite.SetMulColor
		(
			Vector4
			(
				1.0f,
				1.0f,
				1.0f,
				0.0f
			)
		);
	}
	/** 非選択時は背景を半透明表示 */
	else
	{
		m_blackSprite.SetMulColor
		(
			Vector4
			(
				1.0f,
				1.0f,
				1.0f,
				0.5f
			)
		);
	}
	m_blackSprite.Draw(rc);
}