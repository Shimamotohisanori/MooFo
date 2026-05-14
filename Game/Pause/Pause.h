#pragma once
class Game;
class SoundPause;
class SoundManager;
class Pause : public IGameObject
{
public:
	Pause(){}
	~Pause(){}
	bool Start();
	void Update();
	void Choose();
	void Select();
	//void StopBGM();
	void Render(RenderContext& rc);

	/** ポーズ中に選んでいる場所の番号設定関数 */
	void SetCountNumber(uint8_t countNumber)
	{
		m_countNumber = countNumber;
	}

	/** ポーズ中に選んでいる場所の番号取得関数 */
	uint8_t GetCountNumber()
	{
		return m_countNumber;
	}

	/** ポーズ中かどうかのフラグ設定関数 */
	void SetIsPause(bool flag)
	{
		m_isPause = flag;
	}

	/** ポーズ中かどうかのフラグ取得関数 */
	bool GetIsPause()
	{
		return m_isPause;
	}


private:
	uint8_t m_countNumber = 0;

	SoundManager* m_choiceSound;
	SoundSource* p_chiceSE;
	SoundSource* p_DecisionSE;

	/** ポーズ中かどうかのフラグ*/
	bool m_isPause = false;

	Game* m_game;
	SoundPause* m_soundPause;
	
	SpriteRender m_pauseBackGround;
	SpriteRender m_quiteSprite;
	SpriteRender m_resumeSprite;
	SpriteRender m_soundSprite;
	SpriteRender m_settingSprite;
	SpriteRender m_bgmSprite;
	SpriteRender m_sePrite;
	SpriteRender m_volumeSprite;
	SpriteRender m_notVolumeSprite;
	SpriteRender m_arrowSprite;

	

	

};