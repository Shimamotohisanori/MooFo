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

	/** カーソルの移動を行う関数 */
	void MoveCursor();

	/** 決定ボタンが押されたときの処理を行う関数 */
	void Decide();

	/** 選択中のボタンの色を更新する関数 */
	void UpdateButtonColor();

private:
	uint8_t m_cursorRow = 0;
	uint8_t m_cursorCol = 0;

	uint8_t selectNum = 0;

	bool m_isDecided = false;

	/** 選択中のマスの画像 */
	SpriteRender m_difficultyButtonSpriteRender[DIFFICULTY_GRID_ROWS][DIFFICULTY_GRID_COLS];
	
	/** 難易度選択画面の画像 */
	SpriteRender m_difficultySprite;

	/** 難易度選択画面の背景画像 */
	SpriteRender m_difficultyBackgroundSprite;

	/** やさしい説明画像(チュートリアル) */
	SpriteRender m_tutorialExplanationSprite;

	/** かんたん説明画像 */
	SpriteRender m_easyExplanationSprite;

	/** ふつう説明画像 */
	SpriteRender m_normalExplanationSprite;

	/** むずかしい説明画像 */
	SpriteRender m_hardExplanationSprite;

	LoadingScene* m_loadingScene;

};

