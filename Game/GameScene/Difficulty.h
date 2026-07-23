#pragma once
class Difficulty : IGameObject
{
public:
	Difficulty();
	~Difficulty();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:


private:

	uint8_t selectNum = 0;

	SpriteRender m_difficultySprite;

};

