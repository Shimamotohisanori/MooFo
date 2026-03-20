#pragma once
class GameOver : public IGameObject
{
public:
	GameOver();
	~GameOver();

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//GameOver時に行う処理
	void InGameOver();
private:
	SpriteRender m_GameOverspriteRender;

	bool m_isDeleteRequest = false;

	SoundManager* m_deathSound;
	SoundSource* p_deathBGM;
};

