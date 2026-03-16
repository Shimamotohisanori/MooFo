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

	int m_countNumber = 0;

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