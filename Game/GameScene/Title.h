#pragma once
class LoadingScene;
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
	SoundManager* m_soundManager;

	/** タイトルのBGM */
	SoundSource* m_titleBGM;

	/** タイトルのSE */
	SoundSource* m_titleSE;

	/** タイトルからローディングシーンに移行するためのローディングシーン */
	LoadingScene* m_loadingScene;

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
};

