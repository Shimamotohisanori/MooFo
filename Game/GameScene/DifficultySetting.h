#pragma once
namespace
{
	constexpr int DIFFICULTY_GRID_ROWS = 2;
	constexpr int DIFFICULTY_GRID_COLS = 2;
}
enum class EnDifficulty
{
	en_Tutorial,
	en_Easy,
	en_Normal,
	en_Hard,
	en_DifficultyNum
};

struct DifficultyParam
{
	/** クリア条件となる牛の救出ノルマ数 */
	int normaCount;

	/** 同時に出現させるUFOの数 */
	int ufoCount;

	/** 追いかけてくる牛の出現確率 */
	int chaseCowRateOffset;

	/** 制限時間 */
	int timeLimit;

	/** 牛の最大数 */
	int maxCowCount;

	/** ボーナス牛の出現率 */
	int bonusCowRate;

	/** UFOに向かって走る牛の出現確率 */
	int UFOCowRate;
};

namespace GameDifficultyManager
{
	/** 難易度を設定する */
	void SetDifficulty(EnDifficulty difficulty);

		/** 現在設定されている難易度を取得する */
		EnDifficulty GetDifficulty();

	/** 現在の難易度に対応するパラメーターの取得する */
		const DifficultyParam GetParam();

}

class DifficultySetting : public  IGameObject
{

};

