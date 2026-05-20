#pragma once
class Score;
class CowNumberOfRescues;
class LoadingScene;
class Title;
/** ゲームオーバーのクラス */
class GameOver : public IGameObject
{
public:
	GameOver();
	~GameOver();

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** GameOver時に行う処理 */
	void InGameOver();
	
	/** スコアをセットする関数 */
	void SetFinalScore(int score);
	
	/** 救出数をセットする関数 */
	void SetFinalRescue(int rescue);


private:
	/** ゲームオーバーのフェード処理 */
	void FadeGameOver();


private:
	/** ゲームオーバーのスプライトレンダー */
	SpriteRender m_GameOverspriteRender;

	/** 黒い背景用のスプライトレンダラー */
	SpriteRender m_blackSpriteRender;

	/** ゲームオーバー画面でのスタートを促すスプライトレンダー */
	SpriteRender m_gameOverPressTitleSpriteRender;

	/** ゲームオーバーのBGMとSEを管理するサウンドマネージャー */
	SoundManager* m_deathSound = nullptr;

	/** スコアクラスのポインタ */
	Score* m_score = nullptr;

	/** 救出数クラスのポインタ */
	CowNumberOfRescues* m_cowNumberOfRescues = nullptr;

	/** ゲームオーバーのBGM */
	SoundSource* m_deathBGM = nullptr;

	/** ゲームオーバーのSE */
	SoundSource* m_decisionSE = nullptr;

	/** ゲームオーバーからタイトルシーンに移行するためのローディングシーン */
	LoadingScene* m_loadingScene = nullptr;

	/** タイトルシーンに移行するときのタイトルクラスのポインタ */
	enum enGameOverState
	{
		/** ゲームオーバーのフェードイン */
		FadeIn,
		/** ゲームオーバーのフェードアウト */
		FadeOut,
	};
	enGameOverState m_gameOverState = FadeIn;

	/** スコアの最終値 */
	int m_finalScore;

	/** 救出数の最終値 */
	int m_finalRescue = 0;

	/** α値の変数 */
	float m_gameOverAlpha = 1.0f;

	/** 点滅の間隔 */
	float m_gameOverBlinkInterval = 25.0f;

	/** 点滅が終わる時間 */
	float m_gameOverFinalBlinkTime = 1.0f;

	/** ボタンを押したときのフラグ */
	bool m_isGameOverToTitleButtonPressed = false;

	/** 削除要求フラグ */
	bool m_isDeleteRequest = false;

	/** スコアをセットしたかどうかのフラグ */
	bool m_isScoreSet = false;
	
	/** 救出数をセットしたかどうかのフラグ */
	bool m_isRescueSet = false;
};

