#include "stdafx.h"
#include "DifficultySetting.h"

namespace
{
	/** 難易度ごとのパラメーターテーブル */
	const DifficultyParam DIFFICULTY_PARAM_TABLE[static_cast<int>(EnDifficulty::en_DifficultyNum)] =
	{
		/** ノルマ 　UFO少なめ　追いかける牛の出現確率　 制限時間　牛最大出現数 ボーナス牛の出現確率  UFOに向かって歩く牛の出現確率 */

		/** ※追いかける牛がマイナス数値の理由は出現確率の定義ですでに20％未満なためそこから引き算方式で出現確率を調整しています。 */

		/** チュートリアル用のため未使用 */
		{ 0, 0,    0 ,  0, 0, 0, 0},
		/** 簡単 */
		{ 5, 2 , 0 ,120, 6, 0, 50},

		/** 普通 */
		{10, 3,   -10, 110, 8, 4,40},

		/** 難しい */
		{20, 4,  -5 ,100,10, 6,35},
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