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
	int GetCount()const 
	{
		return m_Count;
	}

	/** BGM音量設定 */
	void SetBGMVolume(float bgm)
	{
		m_bgmVolume = bgm;
	}

	/** BGM音量取得 */
	float GetBGMVolume()const
	{
		return m_bgmVolume;
	}

	/** SE音量設定 */
	void SetSEVolume(float se)
	{
		m_seVolume = se;
	}

	/** SE音量取得 */
	float GetSEVolume()const
	{
		return m_seVolume;
	}

	
private:
	/** どこを選択しているかを表すカウント変数*/
	int m_Count = 0;

	/** BGMの音量 */
	float m_bgmVolume = 0.3f;

	/** SEの音量 */
	float m_seVolume  = 0.8f;

	/** */
	/** Pause画面に出てくる背景*/
	SpriteRender m_backGroundSprite;
	/** BGMやSEを調整する画面の外枠*/
	SpriteRender m_settingSprite;
	/** 音量アイコン*/
	SpriteRender m_volumeSprite;
	/** 音量が出ていないときのアイコン*/
	SpriteRender m_notVolumeSprite;
	/** 「BGM」とテキストで表示するアイコン*/
	SpriteRender m_bgmSprite;
	/** 「SE」とテキストで表示するアイコン*/
	SpriteRender m_seSprite;
	/** SEの音量アイコン*/
	SpriteRender m_seVolumeSprite;
	/** SEで音量が出ていないことを表すアイコン*/
	SpriteRender m_seNotVolumeSprite;
	/** 「もどる」を文字で表すアイコン*/
	SpriteRender m_buttonReturnSprite;
	/** BGMの調整をするための茶色い丸いスライドバー*/
	SpriteRender m_soundIconSprite;
	/** SEの調整をするための黄色い丸いスライドバー*/
	SpriteRender m_seIconSprite;
	/** BGM・SEを調整するための細長いバー*/
	SpriteRender m_blownBarSprite;
	SpriteRender m_seBlownBarSprite;
	/** 丸いスライドバーが選択されていないときに出すスライドバー*/
	SpriteRender m_bgmBlackIcon;
	SpriteRender m_seBlackIcon;
	/** 選ばれていないときに出す黒い画像*/
	SpriteRender m_blackSprite;

	/** 灰色のバー */
	SpriteRender m_grayBarSprite;

	/** サウンドマネージャーのポインタ*/
	SoundManager* m_choiceSound = nullptr;
	/** 選択する時のSEのサウンドソースのポインタ*/
	SoundSource* p_chiceSE = nullptr;
	/** 決定する時のSEのサウンドソースのポインタ*/
	SoundSource* p_decisionSE = nullptr;

private:
	/** ゲームのポインタ*/
	Pause* m_pause = nullptr;
};