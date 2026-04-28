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
	/** ローディング中の背景のスプライトレンダー*/
	SpriteRender m_blackLoadingSpriteRender;

	/** ローディング中に使うスプライトレンダー*/
	SpriteRender m_loadingSpriteRender[3];

	/** Loadingの文字のスプライトレンダー*/
	SpriteRender m_loadingTextSpriteRender;

	/** 現在表示している画像*/
	int m_currentImage = 0;

	/** タイマー*/
	float m_timer = 0.0f;

	/**画像を切り替える時間*/
	float m_changeTime = 5.0f;

	/**ローディング開始からの経過時間*/
	float m_totalTime = 0.0f;

	/**ローディングの全体時間*/
	float m_loadingTime = 15.0f;

	/** Load文字フェード用の変数*/
	/** Loadingの文字のアルファ値*/
	float m_loadingTextAlpha = 1.0f;

	/** Loadingの文字のフェード速度*/
	float m_loadingFadeSpeed = 0.5f;

	/** フェードインしているかどうかのフラグ*/
	bool m_isFadeIn = false;

	SoundSource* m_loadingSound;	//ローディング中の音源。

	//これは、LoadingSceneを呼び出すときに、次のシーンをロードする関数を引数で渡してもらうための変数。
	/** 次のシーンをロードする関数*/
	std::function<void()> m_nextSceneLoading;

}; 

