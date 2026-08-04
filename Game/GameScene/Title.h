#pragma once
//class LoadingScene;
class Difficulty;
/** タイトルシーンのクラス */
class Title : public IGameObject
{
public:
	Title() ;
	~Title();
	bool Start();
	void Update();

	/** タイトルで行う処理 */
	void InTitle();
	/** タイトルの初期化が完了したかどうかを返す */
	bool IsReady()const
	{
		return m_isReady;
	}

	/** 描画処理 */
	void Render(RenderContext& rc);


private:
	/** フェード用関数 */
	void FadeTitle();


private:
	/** タイトルのスプライトレンダー */
	SpriteRender m_titleSpriteRender;

	/** タイトル画面でのスタートを促すスプライトレンダー */
	SpriteRender m_titlePressStartSpriteRender;

	/** タイトルのBGMとSEを管理するサウンドマネージャー */
	SoundManager* m_soundManager = nullptr;

	/** タイトルのBGM */
	SoundSource* m_titleBGM = nullptr;

	/** タイトルのSE */
	SoundSource* m_titleSE = nullptr;

	/** タイトルからローディングシーンに移行するためのローディングシーン */
	//LoadingScene* m_loadingScene = nullptr;
	Difficulty* m_difficulty = nullptr;


	/** タイトルの状態を管理する列挙型 */
	enum enTitleState
	{
		/** タイトルのフェードイン */
		FadeIn,
		/** タイトルのフェードアウト */
		FadeOut,
	};
	enTitleState m_titleState = FadeIn;

	/** α値の変数 */
	float m_titleAlpha = 0.0f;

	/** 点滅の間隔 */
	float m_titleBlinkInterval = 25.0f;

	/** 点滅が終わる時間 */
	float m_titleFinalBlinkTime = 1.0f;

	/** ボタンを押したときのフラグ */
	bool m_isStartButtonPressed = false;
	/** タイトルの初期化が完了したかどうかのフラグ */
	bool m_isReady = false;
	
};

