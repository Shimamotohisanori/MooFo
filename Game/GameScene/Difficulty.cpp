#include "stdafx.h"
#include "Difficulty.h"
#include "LoadingScene.h"

namespace
{
	/** 難易度設定のファイルパス */
	const char* DIFFICULTY_FILEPATH = "Assets/sprite/GameTransition/Difficulty/difficulty.dds";

	/** 各難易度の表示二次元配列(行列) */
	const char* BUTTON_FILEPATH[DIFFICULTY_GRID_ROWS][DIFFICULTY_GRID_COLS] =
	{
		{"Assets/sprite/GameTransition/Difficulty/test.dds" , "Assets/sprite/GameTransition/Difficulty/test.dds"},
		{"Assets/sprite/GameTransition/Difficulty/test.dds" , "Assets/sprite/GameTransition/Difficulty/test.dds"}
		
	};

	/** 難易度設定の背景画像のファイルパス */
	const char* DIFFICULTY_BACKGROUND_FILEPATH = "Assets/sprite/GameTransition/Difficulty/difficulty_background.dds";

	/** やさしいの説明画像のファイルパス */
	const char* TUTORIAL_EXPLANATION_FILEPATH = "Assets/sprite/GameTransition/Difficulty/Tutorial_Explanation.dds";

	/** かんたんの説明画像のファイルパス */
	const char* EASY_EXPLANATION_FILEPATH = "Assets/sprite/GameTransition/Difficulty/Easy_Explanation.dds";

	/** ふつうの説明画像のファイルパス */
	const char* NORMAL_EXPLANATION_FILEPATH = "Assets/sprite/GameTransition/Difficulty/Normal_Explanation.dds";

	/** むずかしいの説明画像のファイルパス */
	const char* HARD_EXPLANATION_FILEPATH = "Assets/sprite/GameTransition/Difficulty/Hard_Explanation.dds";

	/** 説明画像の幅 */
	constexpr float EXPLANATION_WIDTH = 1200.0f;

	/** 説明画像の高さ */
	constexpr float EXPLANATION_HEIGHT = 150.0f;

	/** 説明画像の移動速度 */
	constexpr float EXPLANATION_MOVE_SPEED = 4.0f;

	/** 左端まで来たら戻す位置 */
	constexpr float EXPLANATION_RESET_X = 1200.0f;

	/** この座標まで行ったらリセット */
	constexpr float EXPLANATION_END_X = -1200.0f;

	/** 説明画像の初期位置 */
	const Vector3 EXPLANATION_INITIAL_POSITION = Vector3(0.0f, -380.0f, 0.0f);

	/** 難易度設定の黒画面を出す座標 */
	const Vector3 BUTTON_POSITION[DIFFICULTY_GRID_ROWS][DIFFICULTY_GRID_COLS] =
	{
		{Vector3(26.0f, -28.0f,0.0f),Vector3(665.0f, -28.0f,0.0f)},
		{Vector3(26.0f,-260.0f,0.0f),Vector3(665.0f,-260.0f,0.0f)},
	};

	const EnDifficulty BUTTON_DIFFICULTY[DIFFICULTY_GRID_ROWS][DIFFICULTY_GRID_COLS] =
	{
		{EnDifficulty::en_Tutorial, EnDifficulty::en_Easy},
		{EnDifficulty::en_Normal, EnDifficulty::en_Hard},
	};

	/** 難易度選択中の透明度 */
	const Vector4 COLOR_SELECTED     = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
	const Vector4 COLOR_NOT_SELECTED = Vector4(1.0f, 1.0f, 1.0f, 0.5f);

	/** 画像の大きさ */
	constexpr float WIDTH  = 1920.0f;
	constexpr float HEIGHT = 1080.0f;
	constexpr float BLACK_WIDTH  = 1970.0f;
	constexpr float BLACK_HEIGHT = 1100.0f;
}

Difficulty::Difficulty()
{

}

Difficulty::~Difficulty()
{

}

bool Difficulty::Start()
{
	m_difficultySprite.Init(DIFFICULTY_FILEPATH, WIDTH, HEIGHT);
	m_difficultySprite.Update();

	for (int i = 0; i < DIFFICULTY_GRID_ROWS; i++)
	{
		for (int j = 0; j < DIFFICULTY_GRID_COLS; j++)
		{
			m_difficultyButtonSpriteRender[i][j].Init(BUTTON_FILEPATH[i][j], BLACK_WIDTH, BLACK_HEIGHT);
			m_difficultyButtonSpriteRender[i][j].SetPosition(BUTTON_POSITION[i][j]);
			m_difficultyButtonSpriteRender[i][j].Update();
		}
	}

	m_difficultyBackgroundSprite.Init(DIFFICULTY_BACKGROUND_FILEPATH, WIDTH, HEIGHT);
	m_difficultyBackgroundSprite.Update();

	m_tutorialExplanationSprite.Init(TUTORIAL_EXPLANATION_FILEPATH, EXPLANATION_WIDTH, EXPLANATION_HEIGHT);
	m_tutorialExplanationSprite.SetPosition(EXPLANATION_INITIAL_POSITION);
	m_tutorialExplanationSprite.Update();

	m_easyExplanationSprite.Init(EASY_EXPLANATION_FILEPATH, EXPLANATION_WIDTH, EXPLANATION_HEIGHT);
	m_easyExplanationSprite.SetPosition(EXPLANATION_INITIAL_POSITION);
	m_easyExplanationSprite.Update();

	m_normalExplanationSprite.Init(NORMAL_EXPLANATION_FILEPATH, EXPLANATION_WIDTH, EXPLANATION_HEIGHT);
	m_normalExplanationSprite.SetPosition(EXPLANATION_INITIAL_POSITION);
	m_normalExplanationSprite.Update();

	m_hardExplanationSprite.Init(HARD_EXPLANATION_FILEPATH, EXPLANATION_WIDTH, EXPLANATION_HEIGHT);
	m_hardExplanationSprite.SetPosition(EXPLANATION_INITIAL_POSITION);
	m_hardExplanationSprite.Update();

	/** 初期状態の色を設定 */
	UpdateButtonColor();
	return true;
}

void Difficulty::Update()
{
	MoveCursor();
	Decide();
	m_tutorialExplanationSprite.Update();
	m_easyExplanationSprite.Update();
	m_normalExplanationSprite.Update();
	m_hardExplanationSprite.Update();


	/** 選択中の透明度を反映する */
	for (int i = 0; i < DIFFICULTY_GRID_ROWS; i++)
	{
		for (int j = 0; j < DIFFICULTY_GRID_COLS; j++)
		{
			m_difficultyButtonSpriteRender[i][j].Update();
		}
	}
}

void Difficulty::MoveCursor()
{
	uint8_t prevRow = m_cursorRow;
	uint8_t prevCol = m_cursorCol;

	/** ボタン名は実際のプロジェクトの定義に合わせて要修正 */
	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		m_cursorCol = (m_cursorCol + 1) % DIFFICULTY_GRID_COLS;
	}
	else if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		m_cursorCol = (m_cursorCol - 1 + DIFFICULTY_GRID_COLS) % DIFFICULTY_GRID_COLS;
	}
	else if (g_pad[0]->IsTrigger(enButtonDown))
	{
		m_cursorRow = (m_cursorRow + 1) % DIFFICULTY_GRID_ROWS;
	}
	else if (g_pad[0]->IsTrigger(enButtonUp))
	{
		m_cursorRow = (m_cursorRow - 1 + DIFFICULTY_GRID_ROWS) % DIFFICULTY_GRID_ROWS;
	}

	if (prevRow != m_cursorRow || prevCol != m_cursorCol)
	{
		/** 説明画像を初期位置に戻す */
		m_easyExplanationSprite.SetPosition(EXPLANATION_INITIAL_POSITION);
		m_normalExplanationSprite.SetPosition(EXPLANATION_INITIAL_POSITION);
		m_hardExplanationSprite.SetPosition(EXPLANATION_INITIAL_POSITION);
		m_tutorialExplanationSprite.SetPosition(EXPLANATION_INITIAL_POSITION);

		/** カーソルが動くと色を更新 */
		UpdateButtonColor();
	}
}

void Difficulty::Decide()
{
	/** 決定ボタン名も実際のプロジェクトの定義に合わせて要修正 */
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		GameDifficultyManager::SetDifficulty(BUTTON_DIFFICULTY[m_cursorRow][m_cursorCol]);

		/** カーソル位置に対応する難易度を取得して変数に入れる */
		EnDifficulty selected = BUTTON_DIFFICULTY[m_cursorRow][m_cursorCol];

		if (selected == EnDifficulty::en_Tutorial)
		{
			m_isDecided = true;

			/** ここにチュートリアルのクラスを記入 */
		}

		m_isDecided = true;

		m_loadingScene = NewGO<LoadingScene>(0, "loading");
		m_loadingScene->SetLoadType(LoadingScene::LoadType::ToGameScene);

		/** チュートリアルが選ばれていたらLoadingSceneに伝える*/
		if(selected ==EnDifficulty::en_Tutorial)
		{
			m_loadingScene->SetIsTutorial(true);
		}

		DeleteGO(this);
	}
}

void Difficulty::UpdateButtonColor()
{
	for (int row = 0; row < DIFFICULTY_GRID_ROWS; row++)
	{
		for (int col = 0; col < DIFFICULTY_GRID_COLS; col++)
		{
			bool isSelected = (row == m_cursorRow && col == m_cursorCol);
			m_difficultyButtonSpriteRender[row][col].SetMulColor(
				isSelected ? COLOR_SELECTED : COLOR_NOT_SELECTED
			);
		}
	}
}


void Difficulty::Render(RenderContext& rc)
{
	m_difficultyBackgroundSprite.Draw(rc);

	switch (BUTTON_DIFFICULTY[m_cursorRow][m_cursorCol])
	{
	case EnDifficulty::en_Tutorial:
		m_tutorialExplanationSprite.Draw(rc);
		break;

	case EnDifficulty::en_Easy:
		m_easyExplanationSprite.Draw(rc);
		break;

	case EnDifficulty::en_Normal:
		m_normalExplanationSprite.Draw(rc);
		break;

	case EnDifficulty::en_Hard:
		m_hardExplanationSprite.Draw(rc);
		break;
	}

	m_difficultySprite.Draw(rc);

	/** 選択中のマスだけ通常画像、それ以外は薄暗い画像を描画する */
	for (int row = 0; row < DIFFICULTY_GRID_ROWS; row++)
	{
		for (int col = 0; col < DIFFICULTY_GRID_COLS; col++)
		{
				m_difficultyButtonSpriteRender[row][col].Draw(rc);
		}
	}
}
