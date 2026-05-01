#pragma once
class Score;
class CowNumberOfRescues;
class LoadingScene;
class Title;
class GameOver : public IGameObject
{
public:
	GameOver();
	~GameOver();

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//GameOver時に行う処理
	void InGameOver();
	//スコアをセットする関数
	void SetFinalScore(int score);
	//救出数をセットする関数
	void SetFinalRescue(int rescue);


private:
	/** ゲームオーバーのフェード処理 */
	void FadeGameOver();

private:
	SpriteRender m_GameOverspriteRender;

	/** 黒い背景用のスプライトレンダラー */
	SpriteRender m_blackSpriteRender;

	/** ゲームオーバー画面でのスタートを促すスプライトレンダー */
	SpriteRender m_gameOverPressTitleSpriteRender;

	/** スコアの表示をコピーするための変数 */
	SoundManager* m_deathSound;
	Score*m_score;
	CowNumberOfRescues* m_cowNumberOfRescues;
	SoundSource* m_deathBGM;
	SoundSource* m_decisionSE;
	LoadingScene* m_loadingScene;

	enum enGameOverState
	{
		/** ゲームオーバーのフェードイン */
		FadeIn,
		/** ゲームオーバーのフェードアウト */
		FadeOut,
	};
	enGameOverState m_gameOverState = FadeIn;

	int m_finalScore;

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

