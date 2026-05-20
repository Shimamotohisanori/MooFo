#pragma once
#include<functional>
class Cow;
class CountDown;
class GameCamera;
class UFO;
/** ローディングシーンのクラス*/
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

	/** ロードするシーンのタイプ*/
	enum LoadType
	{
		/** ゲームシーンに移行するタイプ*/
		ToGameScene,

		/** タイトルシーンに移行するタイプ*/
		ToTitleScene,
	};

	/** ロードするシーンのタイプ*/
	LoadType m_loadType = ToGameScene;

	/** ロードするシーンのタイプを設定する関数 */
	void SetLoadType(LoadType loadType)
	{
		m_loadType = loadType;
	}


private:
	/** ゲームオブジェクトをステップバイステップでロードする関数*/
	void LoadGameObjectsStepByStep();

	/** タイトルのみをロードする関数 */
	void LoadTitleOnly();

	/** ランダムな牛のスポーン位置を生成する関数 */
	Vector3 RandomCowPos();


private:
	/** ローディング中の背景のスプライトレンダー*/
	SpriteRender m_blackLoadingSpriteRender;

	/** ローディング中に使うスプライトレンダー*/
	SpriteRender m_loadingSpriteRender[3];

	/** Loadingの文字のスプライトレンダー*/
	SpriteRender m_loadingTextSpriteRender;

	/** ロードするゲームオブジェクト(牛)のリスト*/
	std::vector<Cow*> m_tempCows;

	/** ロードするゲームオブジェクト(UFO)のリスト*/
	std::vector<UFO*> m_tempUFOs;

	/** 現在表示している画像*/
	uint8_t m_currentImage = 0;

	/** ロードするゲームオブジェクトのステップ*/
	uint8_t m_loadStep = 0;

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

	/** ローディング中の音源 */
	SoundSource* m_loadingSound;	

	//これは、LoadingSceneを呼び出すときに、次のシーンをロードする関数を引数で渡してもらうための変数。
	/** 次のシーンをロードする関数*/
	std::function<void()> m_nextSceneLoading;

}; 

