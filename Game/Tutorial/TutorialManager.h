#pragma once
class FadeManager;
class Cow;
class UFO;
class Game;
class TutorialManager : public IGameObject
{
public:

	TutorialManager();
	~TutorialManager();
	

	bool Start();
	void Update();
	void Render(RenderContext& rc) {};

public:
	/** チュートリアル用のenum*/
	enum class EnTutorialStep
	{
		RopeThrow,/** ロープを投げる*/
		CowRescue,/** 牛を救助する*/
		PlaceFood,/** 餌を置く*/
		GuideToBarn,/** 誘導させて牛舎に避難させる*/
		Complete/**完了 */
	};

	/** 現在のステップを取得する関数*/
	EnTutorialStep GetCurrentStep()const
	{
		return m_cuurentStep;
	}

	
	

	/** 成功回数を加算する関数*/
	void AddSuccessCount(EnTutorialStep step);

	/** チュートリアルが完了しているかどうかを取得する関数*/
	bool IsComplete()const
	{
		return m_cuurentStep == EnTutorialStep::Complete;
	}

	/** ステップ遷移中(フェード中)かどうか*/
	bool IsTransitioning()const
	{
		return m_isTransitioning;
	}

private:



	/** チュートリアル完了演出を更新する関数*/
	void UpdateCompleteStep();
	/** ステップの切り替え・フェード制御を行う関数*/
	void UpdateStepTransition();
	/** 現在のステップに必要な成功回数を取得する関数*/
	int GetRequiredCount(EnTutorialStep step)const;
	/** 次のステップに切り替える関数*/
	void AdvanceStep();
	/** プレイヤー・カメラ・ロープをステップ開始時の状態に戻す関数*/
	void ResetPlayerForNextStep();

	/** プレイヤーの初期位置記憶を切り出した関数 */
	void CaptureInitialTransformIfNeeded();

	/** チュートリアル完了時にタイトルへ遷移させる関数(暗転を維持したまま呼ぶ) */
	void TransitionToTitle();

	/** 牛を救出するときのチュートリアル中に連れ去られて救出に失敗した時に新しい牛を生成する関数*/
	void CheckAndRespawnCowTutorial();
private:

	/** チュートリアル用オブジェクトのセットアップステップ */
	enum class EnSetupStep
	{
		NotStarted,
		SpawnCow,
		SpawnUFO,
		WaitUFOReady,
		ForceLight,
		Done
	};
	/** チュートリアル用の牛・UFOを準備する関数*/
	void SetupTutorialActors();


	EnSetupStep m_setupStep = EnSetupStep::NotStarted;

	/** チュートリアル専用に生成した牛 */
	Cow* m_tutorialCow = nullptr;

	/** 牛の餌のチュートリアルで使う牛 */
	Cow* m_foodTutorialCow = nullptr;

	/** チュートリアル専用に生成したUFO */
	UFO* m_tutorialUFO = nullptr;

	/** 現在のチュートリアルステップ*/
	EnTutorialStep m_cuurentStep = EnTutorialStep::RopeThrow;
	/** 現在のステップの成功回数*/
	int m_successCount = 0;
	/** チュートリアルが終了したかどうかのフラグ*/
	bool m_hasEndedTutorial = false;

	/** フェード処理のポインタ*/
	FadeManager* m_fadeManager = nullptr;

	/** ステップ遷移(フェード)中かどうかのフラグ */
	bool m_isTransitioning = false;

	/** 暗転中にステップを切り替え済みかどうかのフラグ */
	bool m_hasSwitchedStep = false;

	/** チュートリアル開始時のプレイヤー位置・回転を記憶しておくための変数 */
	bool m_hasCapturedInitialTransform = false;
	Vector3 m_playerInitialPos = Vector3::Zero;
	Quaternion m_playerInitialRot = Quaternion::Identity;


	/**各ステップに必要な成功回数*/
	/** ロープを投げる */
	static constexpr int REQUIRED_ROPE_THROW = 1;
	/** 牛を救助する */
	static constexpr int REQUIRED_COW_RESCUE = 1;
	/** 餌を置く */
	static constexpr int REQUIRED_PLACE_FOOD = 3;
	/** 誘導させて牛舎に避難させる */
	static constexpr int REQUIRED_GUIDE_BARN = 1;
};

