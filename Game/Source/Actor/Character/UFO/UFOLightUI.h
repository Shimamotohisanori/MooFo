#pragma once
class CountDown;
class Pause;
class UFOLightManager;
class Game;
/** UFOの光のUIクラス */
class UFOLightUI : public IGameObject
{
public:
	UFOLightUI();
	~UFOLightUI();

public:
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

private:
	/** 秒数の描画処理 */
	void CountSpriteUI();

private:
	/** UFOの管理クラス */
	UFOLightManager* m_ufolightManager = nullptr;

	/** ポーズ */
	Pause* m_pause = nullptr;

	/** カウントダウン */
	CountDown* m_countdown = nullptr;
    /** ゲーム*/
	Game* m_game = nullptr;

	/** 光が出ているときに表示する数字のスプライトレンダー */
	SpriteRender m_LightApperNumberSpriteRender[5];

	/** 「光の発射まで」を表示するスプライトレンダー */
	SpriteRender m_LightApperSpriteRender;

	/** 「秒」を表示するスプライトレンダー */
	SpriteRender m_secondsSpriteRender;

	/** 現在の秒数 */
	int m_currentCount = 0;
};

