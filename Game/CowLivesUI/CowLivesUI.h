#pragma once
class Game;
class Pause;
class FadeManager;
class CountDown;
class CowLivesUI : public IGameObject
{
public:

	CowLivesUI();
	~CowLivesUI();
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** UFOが牛を連れ去ることに成功した時に呼ぶ関数*/
	void DecreaseLife();

	/** 全ての牛のライフが0になったかどうか*/
	bool GetIsAllCowLost()const
	{
		return m_lostCount >= COW_LIFE_NUM;
	}
	/** 「FAILED RESCUE」が表示されているときは操作を受け付けないようにする関数*/
	bool IsNotOperetion()const
	{
		return m_isNotOperation;
	}
	/** 「RESCUE　FAILED」の文字が出た後のフェードが始まっていた時の関数*/
	bool IsFadeInComplete()
	{
		return m_gameOverPhase == GameOverPhase::WaitingFade;
	}

	/** RESCUE FAILED演出中（文字表示～フェード完了待ちまで）かどうか */
	bool IsGameOverSequenceActive() const
	{
		return m_gameOverPhase != GameOverPhase::None;
	}
	/** 残機表示をリセットする関数*/
	void ResetLives();

private:
	/** ゲームオーバー演出の進行を１フレーム分進める関数*/
	void UpdateGameOverSequence();

	/** RESCUE FAILED演出を開始する関数*/
	void StartGameOverSequence();

private:
	/** 牛の残機数*/
	static constexpr int COW_LIFE_NUM = 3;

	/** 牛アイコン１枚分の情報*/
	struct CowIcon
	{
		SpriteRender render;
		bool isDead = false;
		/** 残機が一つになっているかどうかのフラグ*/
		bool isCrisis = false;
	};

	/**牛のアイコン配列*/
	CowIcon m_cowIcons[COW_LIFE_NUM];

	/** 連れ去られた牛の数*/
	int m_lostCount = 0;

	/** 点滅の切り替え間隔*/
	static constexpr int BLINK_INTERVAL = 15;

	/** 点滅用のフレームカウント*/
	int m_blinkFreamCount = 0;

	/** 点滅の表示/非表示状態(trueが表示中)*/
	bool m_isBlinkVisible = true;;

	/** FAILED RESCUEが出ていたら操作を受け付けないようにするフラグ*/
	bool m_isNotOperation = false;
	/**「救助失敗」時に流すSEが再生中かどうかのフラグ */
	bool m_isPlayFailedRescueSE = false;
	/** ゲームオーバー演出の状態*/
	enum class GameOverPhase
	{
		None,/** 通常状態*/
		ShowingFailedText,/** RESCUE FAILED表示中*/
		WaitingFade,/** フェード(暗転)完了待ち*/
	};
	GameOverPhase m_gameOverPhase = GameOverPhase::None;

	/** RESCUE FAILEDを表示する時間*/
	static constexpr float FAILED_TEXT_DISPLAY_TIME = 3.0f;

	
	/** テキスト表示からの経過時間*/
	float m_failedTextTimer = 0.0f;
	/** 「RESCUE」のスプライトレンダー*/
	SpriteRender m_rescueTextRender;
	/** 「FAILED」のスプライトレンダー*/
	SpriteRender m_failedTextRender;
	/** 「…」のスプライトレンダー*/
	SpriteRender m_tenTextRender;

	/** テキスト拡大演出の現在のスケール*/
	Vector3 m_texScale = Vector3(0.0f, 0.0, 1.0f);

	/** ゲームのポインタ*/
	Game* m_game = nullptr;
	/** ポーズのポインタ*/
	Pause* m_pause = nullptr;
	/** 「救助失敗」のテキストを出すときのSEのポインタ*/
	SoundSource* m_failedRescueSE = nullptr;
	/** カウントダウンのポインタ*/
	CountDown* m_countDown = nullptr;


	/** フェードマネージャーのポインタ*/
	FadeManager* m_fadeManager = nullptr;

};

