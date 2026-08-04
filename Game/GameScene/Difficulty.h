#pragma once
#include "DifficultySetting.h"
class LoadingScene;
class Difficulty : public IGameObject
{
public:
	Difficulty();
	~Difficulty();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

	void MoveCursor();
	void Decide();
	void UpdateButtonColor();

private:
	uint8_t m_cursorRow = 0;
	uint8_t m_cursorCol = 0;

	uint8_t selectNum = 0;

	bool m_isDecided = false;

	SpriteRender m_difficultyButtonSpriteRender[DIFFICULTY_GRID_ROWS][DIFFICULTY_GRID_COLS];
	
	SpriteRender m_difficultySprite;

	LoadingScene* m_loadingScene;

};

