#include "stdafx.h"
#include "DifficultySetting.h"

namespace
{
	/** 難易度ごとのパラメーターテーブル */
	const DifficultyParam DIFFICULTY_PARAM_TABLE[static_cast<int>(EnDifficulty::en_DifficultyNum)] =
	{
		/** ノルマ 　UFO少なめ　追いかける牛　*/

		/** チュートリアル用のため未使用 */
		{ 0, 0,    0 },
		/** 簡単 */
		{ 5, 2 , -10 },

		/** 普通 */
		{10, 3,   -5 },

		/** 難しい */
		{20, 4,  -10 },
	};

	EnDifficulty g_currentDifficulty = EnDifficulty::en_Normal;
}

namespace GameDifficultyManager
{
	void SetDifficulty(EnDifficulty difficulty)
	{
		g_currentDifficulty = difficulty;
	}

	EnDifficulty GetDifficulty()
	{
		return g_currentDifficulty;
	}

	const DifficultyParam GetParam()
	{
		return DIFFICULTY_PARAM_TABLE[static_cast<int>(g_currentDifficulty)];
	}
}