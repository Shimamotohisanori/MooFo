#include "stdafx.h"
#include "DifficultySetting.h"

namespace
{
	/** 難易度ごとのパラメーターテーブル */
	const DifficultyParam DIFFICULTY_PARAM_TABLE[static_cast<int>(EnDifficulty::en_DifficultyNum)] =
	{
		/** ノルマ 　UFO少なめ　追いかける牛　 制限時間　牛最大出現数*/

		/** チュートリアル用のため未使用 */
		{ 0, 0,    0 ,  0, 0},
		/** 簡単 */
		{ 5, 2 , -10 ,120, 6},

		/** 普通 */
		{10, 3,   -5, 110, 8},

		/** 難しい */
		{20, 4,  -10 ,100,10},
	};

	EnDifficulty g_currentDifficulty = EnDifficulty::en_Easy;
}

namespace GameDifficultyManager
{
	/** 難易度の設定 */
	void SetDifficulty(EnDifficulty difficulty)
	{
		g_currentDifficulty = difficulty;
	}

	/** 現在設定している難易度の取得 */
	EnDifficulty GetDifficulty()
	{
		return g_currentDifficulty;
	}

	/** 現在の難易度のパラメーターを設定する */
	const DifficultyParam GetParam()
	{
		return DIFFICULTY_PARAM_TABLE[static_cast<int>(g_currentDifficulty)];

	}
}