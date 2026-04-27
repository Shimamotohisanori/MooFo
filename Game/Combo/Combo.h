#pragma once
class Timer;
class Score;
/** コンボクラス */
class Combo : public IGameObject
{
public:
	Combo();
	~Combo();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** コンボを取得する関数 */
	int GetCombo()
	{
		return m_combo;
	}

	/** スコア倍率を取得する関数 */
	int GetScoreMagnification()
	{
		return m_scoreMagnification;
	}

	/** コンボタイマーを減らす関数 */
	void DecreaseComboTimer(float deltaTime)
	{
		m_comboTimer -= deltaTime;
		if (m_comboTimer < 0.0f)
		{
			m_comboTimer = 0.0f;
			ResetCombo();
		}
	}

	/** コンボタイマーを取得する関数 */
	float GetComboTimer()
	{
		return m_comboTimer;
	}

	/** コンボを加算する関数 */
	void AddCombo();

	/** コンボが途切れたときに呼ぶ関数 */
	void ResetCombo();

	/** スコア加算関数 */
	void AddScore(int score);

	/** コンボしたかどうか判定するフラグ */
	bool IsCombo()const;


private:
	/** コンボの描画関数 */
	void ViewComboSprite();


private:
	/** ファイルパス */
	std::string m_filePath[10];

	/** コンボ用の十の位専用数字画像 */
	SpriteRender m_comboTensSprite[10];

	/** コンボ用の一の位専用数字画像 */
	SpriteRender m_comboOnesSprite[10];

	/** コンボ画像 */
	SpriteRender m_comboSprite;

	/** スコア */
	Score* m_score = nullptr;

	/** タイマー */
	Timer* m_timer = nullptr;

	/** コンボの変数 */
	int m_combo = 0;

	/** スコアの倍率 */
	int m_scoreMagnification = 1;

	/** コンボするごとに制限時間をプラスする変数 */
	float m_comboTimer = 0.0f;

	/** コンボの画像を表示する時間 */
	float m_comboSpriteViewTime = 1.0f;

	/** コンボ画像を移動する用の変数 */
	float m_comboSpriteMoveTime = 0.0f;

	/** コンボ画像を表示するかどうかのフラグ */
	bool m_isViewComboSprite = false;

};

