#pragma once
class LoadingScene;
class Title : public IGameObject
{
public:
	Title() ;
	~Title();
	bool Start();
	void Update();
	//タイトルで行う処理
	void InTitle();
	void Render(RenderContext& rc);


private:
	/** フェード用関数 */
	void FadeTitle();

private:
	SpriteRender m_titleSpriteRender;//タイトルのスプライトレンダー

	/** タイトル画面でのスタートを促すスプライトレンダー */
	SpriteRender m_titlePressStartSpriteRender;

	SoundManager* m_soundManager;
	SoundSource* m_titleBGM;
	SoundSource* m_titleSE;
	LoadingScene* m_loadingScene;

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

