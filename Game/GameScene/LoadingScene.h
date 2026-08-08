#pragma once
#include<functional>
#include "AnimSpriteRender.h"
class Cow;
class CountDown;
class GameCamera;
class UFO;
class Game;
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
	
	/** 分割ロード中の処理を行う関数*/
	void LoadInitialSpritesStepByStep();
	/** 餌のアニメーションをバックグラウンドでロードする関数*/
	void LoadFoodAnimInBackground();
	/** シーンが切り替わるタイミングでフェードアウト処理を行う*/
	void FadeOutLoadingScene();
	/** チュートリアルかどうかを設定する関数*/
	void SetIsTutorial(bool isTutorial)
	{
		m_isTutorial = isTutorial;
	}
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

	/** ローディングが終わったことを知らせる関数*/
	bool GetLoadingEnd()const
	{
		return m_isFadeComplete;
	}

private:
	/** ゲームオブジェクトをステップバイステップでロードする関数*/
	void LoadGameObjectsStepByStep();
	 
	/** タイトルのみをロードする関数 */
	void LoadTitleOnly();

	/** アニメーションが再生されて、終わったら自動で次の画像に遷移する関数*/
	void AutoAdvanceImage();

	/** ロープ・餌のGif画像のパスリストとフレーム数を準備する*/
	void PrepareAnimResources();;
	/** ランダムな牛のスポーン位置を生成する関数 */
	Vector3 RandomCowPos();


private:
	/** アニメーション分割ロード用のパスリスト(餌のGif)*/
	std::vector<std::string> m_foodAnimLoadPaths;

	/** アニメーション分割ロード用のパスリスト(ロープで引っ張るGif)*/
	std::vector<std::string> m_ropeAnimLoadPaths;

	/** 歩く牛アニメーションの画像パスリスト*/
	std::vector < std::string>m_CowWalkAnimLoadPaths;
	/** ローディング中の操作説明画像のスプライトレンダラー */
	SpriteRender m_LoadingInstructionSpriteRender;
	/** 牛の餌を説明するアニメーションスプライトレンダラー */
	AnimSpriteRender m_foodAnimSpriteRender;

	/** ロープで救出することを説明するアニメーションスプライトレンダラー */
	AnimSpriteRender m_ropeAnimSpriteRender;

	/** ローディング中の下に出す歩く牛アニメーション*/
	AnimSpriteRender m_cowWalkSpriteRender;


	enum class InitLoadStep
	{
		LoadingInstruction,/** ローディング中の操作説明画像 */
		LoadingGifBackGroundImage,/** 選ばれたGif画像の背景 */
		//LoadingText,/** Loadingの文字 */
		Num
	};

	InitLoadStep m_initLoadStep = InitLoadStep::LoadingInstruction;
	/** ローディング中の背景のスプライトレンダー*/
	SpriteRender m_blackLoadingSpriteRender;

	/** ローディング中に使うスプライトレンダー*/
	SpriteRender m_loadingSpriteRender[2];

	/** Loadingの文字のスプライトレンダー*/
	SpriteRender m_loadingTextSpriteRender;

	/** ロードするゲームオブジェクト(牛)のリスト*/
	std::vector<Cow*> m_tempCows;

	/** ロードするゲームオブジェクト(UFO)のリスト*/
	std::vector<UFO*> m_tempUFOs;

	/** ステップ分割ロード中のステージへのポインタ*/
	class Stage* m_stage = nullptr;  
	/**スカイキューブのポインタ*/
	SkyCube * m_skyCube = nullptr;
	/** ゲームのポインタ*/
	Game* m_game = nullptr;
	/** 現在表示している画像*/
	uint8_t m_currentImage = 0;
	/** アニメーションロードのインデックス*/
	int m_animLoadIndex = 0;

	int  m_ropeLoadIndex = 0;
	int  m_foodLoadIndex = 0;

	/** ロードするゲームオブジェクトのステップ*/
	uint8_t m_loadStep = 0;

	/** 操作説明画像を表示する時間*/
	float m_LoadingInstructionTime = 0.0f;
	/** フェードインしているかどうかのフラグ*/
	//bool m_isFadeIn = false;
	/** ローディングが開始したかどうかのフラグ*/
	bool m_isLoadingStarted = false;
	/** ローディングが終了したかどうかのフラグ*/
	bool m_isLoadingEnd = false;
	/** フェードアウトが終了したかどうかのフラグ*/
	bool m_isFadeComplete = false;

	/** Sceneの切り替え時にフェードインするための変数*/
	/** LodingSceneのα値*/
	float m_SceneFadeAlpha = 1.0f;
	
	/** フェード速度*/
	float m_SceneFadeSpeed = 0.5f;
	
	/** Sceneがフェードアウトしているかどうかのフラグ*/
	bool m_isSceneFadeOut = false;
	
	/** 最初の牛を救出する画像を見たかどうかのフラグ*/
	bool m_hasSeenImage0 = false;
	
	/** 牛の餌の説明画像を見たかどうかのフラグ*/
	bool m_hasSeenImage1 = false;
	
	/** プレイヤーが画像を見終えて決定したかどうかのフラグ*/
	bool m_isReadyToStart = false;
	
	/** 1枚目のGif画像が終了して自動遷移したかどうかのフラグ*/
	bool m_hasAutoAdvancedImage0 = false;

	/** 2枚目のGif画像が終了したかどうかのフラグ*/
	bool m_hasFinishedImage2 = false;
	
	/** 餌のGifがロードされたかどうかのフラグ*/
	bool m_isFoodLoaded = false;
	
	/** 次のシーンが作成されたかどうかのフラグ*/
	bool m_nextSceneCreated = false;
	
	/** 2枚目にロープのGif画像を表示するかどうか(falseの時は餌のGIF)*/
	bool m_useRopeGif = false;
	
	/** 選ばれたGifが最終フレームに到達しているかどうかのフラグ(表示中のみUpdateして、到達後は制止させる)*/
	bool m_hasSelectedGifReachedEnd = false;
	/** チュートリアルかどうかのフラグ*/
	bool m_isTutorial = false;
	
	/** ローディング中の音源 */
	SoundSource* m_loadingSound = nullptr;

	/** 操作説明画像を最低表示しておく時間(秒) */
	static constexpr float MIN_IMAGE0_DISPLAY_TIME = 3.0f; // 好きな秒数に調整
	//これは、LoadingSceneを呼び出すときに、次のシーンをロードする関数を引数で渡してもらうための変数。
	/** 次のシーンをロードする関数*/
	std::function<void()> m_nextSceneLoading = nullptr;

}; 

