#pragma once
#include<functional>
class LoadingScene : public IGameObject
{
public:
	LoadingScene();
	~LoadingScene();

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/** 次のシーンをセットする関数*/
	void SetNextScene(std::function<void()>next);
	/** ローディング中の処理を行う関数*/
	void InLoading();
	/** Loading文字のフェード処理を行う関数*/
	void FadeLoadingText();
private:
	SpriteRender m_blackLoadingSpriteRender;/** ローディング中の背景のスプライトレンダー*/
	SpriteRender m_loadingSpriteRender[3];/** ローディング中に使うスプライトレンダー*/
	SpriteRender m_loadingTextSpriteRender;/** Loadingの文字のスプライトレンダー*/

	int m_currentImage = 0;/** 現在表示している画像*/

	float m_timer = 0.0f;/** タイマー*/
	float m_changeTime = 5.0f;/**画像を切り替える時間*/
	float m_totalTime = 0.0f;/**ローディング開始からの経過時間*/
	float m_loadingTime = 15.0f;/**ローディングの全体時間*/

	/** Load文字フェード用の変数*/
	float m_loadingTextAlpha = 1.0f;/** Loadingの文字のアルファ値*/
	float m_loadingFadeSpeed = 0.5f;/** Loadingの文字のフェード速度*/
	bool m_isFadeIn = false;/** フェードインしているかどうかのフラグ*/
	//これは、LoadingSceneを呼び出すときに、次のシーンをロードする関数を引数で渡してもらうための変数。
	std::function<void()> m_nextSceneLoading;/** 次のシーンをロードする関数*/

}; 

