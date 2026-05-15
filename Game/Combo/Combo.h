#pragma once
class Timer;
class Score;
class AddTimerUI;
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
	
	/** タイムを追加する際に出すUI */
	AddTimerUI* m_addTimerUI = nullptr;
	
	/** 時間が増える際の音 */
	SoundSource* m_addTimerSE = nullptr;

	/** コンボの変数 */
	uint8_t m_combo = 0;

	/** コンボするごとに制限時間をプラスする変数 */
	float m_comboTimer = 0.0f;

	/** コンボの画像を表示する時間 */
	float m_comboSpriteViewTime = 1.0f;

	/** コンボ画像を移動する用の変数 */
	float m_comboSpriteMoveTime = 0.0f;

	/** コンボ画像を表示するかどうかのフラグ */
	bool m_isViewComboSprite = false;

};

