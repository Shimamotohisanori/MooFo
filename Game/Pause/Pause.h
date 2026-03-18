#pragma once
class Game;
class SoundManager;
//class Title;
class Pause : public IGameObject
{
public:
	Pause(){}
	~Pause(){}
	bool Start();
	void Update();
	void Choose();
	void Select();
	void Render(RenderContext& rc);

	/** ポーズ中に選んでいる場所の番号設定関数 */
	void SetCountNumber(int countNumber)
	{
		m_countNumber = countNumber;
	}

	/** ポーズ中に選んでいる場所の番号取得関数 */
	int GetCountNumber()
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
	int m_countNumber = 0;

	/** ポーズ中かどうかのフラグ*/
	bool m_isPause = false;

	Game* m_game;
	//Title* m_title;
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

	SoundManager* m_soundManager;

};