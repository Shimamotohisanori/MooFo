#pragma once
class Score;
class CowNumberOfRescues;
class LoadingScene;
/** ゲームクリアのクラス */
class GameClear :public IGameObject
{
	public:
	GameClear();
	~GameClear();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	
	/** ゲームクリアで行う処理 */
	void InGameClear();

	/** スコアをセットする関数 */
	void SetFinalClearScore(int score);

	/** 救出数をセットする関数 */
	void SetFinalClearRescue(int rescue);


private:
	/** ゲームクリアのフェード処理 */
	void FadeGameClear();


private:
	/** ゲームクリアのスプライトレンダー */
	SpriteRender m_GameClearSpriteRender;

	/** 黒い背景用のスプライトレンダー */
	SpriteRender m_blackSpriteRender;

	/** ゲームクリア画面でのスタートを促すスプライトレンダー */
	SpriteRender m_gameClearPressTitleSpriteRender;

	/** ゲームクリアのBGMとSEを管理するサウンドマネージャー */
	SoundManager* m_clearSound = nullptr;

	/** ゲームクリアのBGM */
	SoundSource* m_clearBGM = nullptr;

	/** ゲームクリアのSE */
	SoundSource* m_decisionSE = nullptr;

	/** スコアクラスのポインタ */
	Score* m_score = nullptr;

	/** 救出数クラスのポインタ */
	CowNumberOfRescues* m_cowNumberOfRescues = nullptr;

	/** ゲームクリアからタイトルシーンに移行するためのローディングシーン */
	LoadingScene* m_loadingScene = nullptr;

	enum enGameClearState
	{
		/** ゲームクリアのフェードイン */
		FadeIn,
		/** ゲームクリアのフェードアウト */
		FadeOut,
	};
	enGameClearState m_gameClearState = FadeIn;

	/** 最終スコアを保存する変数 */
	int m_finalScore = 0;

	/** 最終救出数を保存する変数 */
	int m_finalRescue = 0;

	/** α値の変数 */
	float m_gameClearAlpha = 1.0f;

	/** 点滅の間隔 */
	float m_gameClearBlinkInterval = 25.0f;

	/** 点滅が終わる時間 */
	float m_gameClearFinalBlinkTime = 1.0f;

	/** ゲームクリアでのタイトルに戻るボタンを押したときのフラグ */
	bool m_isGameClearToTitleButtonPressed = false;

	/** スコアがセットされたかどうかのフラグ */
	bool m_isScoreSet = false;

	/** 救出数がセットされたかどうかのフラグ */
	bool m_isRescueSet = false;
	
};

