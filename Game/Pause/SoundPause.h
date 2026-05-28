#pragma once
class Pause;
class SoundPause : public IGameObject
{
public:
	SoundPause() {}
	~SoundPause() {}
	bool Start();
	void Update();
	void ButtonCount();
	void Barbgm();
	void Barse();
	void UpdateBGMUI();
	void UpdateSEUI();
	void Render(RenderContext& rc);

	/** 選択番号設定 */
	void SetCount(int count)
	{
		m_Count = count;
	}

	/** 選択番号取得 */
	int GetCount()
	{
		return m_Count;
	}

	/** BGM音量設定 */
	void SetBGMVolume(float bgm)
	{
		m_bgmVolume = bgm;
	}

	/** BGM音量取得 */
	float GetBGMVolume()
	{
		return m_bgmVolume;
	}

	/** SE音量設定 */
	void SetSEVolume(float se)
	{
		m_seVolume = se;
	}

	/** SE音量取得 */
	float GetSEVolume()
	{
		return m_seVolume;
	}

	
private:
	int m_Count = 0;

	/** BGMの音量 */
	float m_bgmVolume = 0.3f;

	/** SEの音量 */
	float m_seVolume  = 0.8f;

	SpriteRender m_backGroundSprite;
	SpriteRender m_settingSprite;
	SpriteRender m_volumeSprite;
	SpriteRender m_notVolumeSprite;
	SpriteRender m_bgmSprite;
	SpriteRender m_seSprite;
	SpriteRender m_seVolumeSprite;
	SpriteRender m_seNotVolumeSprite;
	SpriteRender m_buttonReturnSprite;
	SpriteRender m_soundIconSprite;
	SpriteRender m_seIconSprite;
	SpriteRender m_blownBarSprite;
	SpriteRender m_seBlownBarSprite;
	SpriteRender m_bgmBlackIcon;
	SpriteRender m_seBlackIcon;
	SpriteRender m_blackSprite;

	SoundManager* m_choiceSound;
	SoundSource* p_chiceSE;
	SoundSource* p_decisionSE;

private:
	Pause* m_pause;
};