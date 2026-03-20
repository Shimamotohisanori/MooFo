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

	/** カウントのゲッターセッターを作る */
	void SetCountNumber(int countNumber)
	{
		m_countNumber = countNumber;
	}

	int GetCountNumber()
	{
		return m_countNumber;
	}

private:
	int m_countNumber = 0;

	SoundManager* m_choiceSound;
	SoundSource* p_chiceSE;
	SoundSource* p_DecisionSE;

	Game* m_game;
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

	SoundPause* m_soundPause;

	

};