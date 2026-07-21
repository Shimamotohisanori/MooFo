#pragma once
class Game;
class Pause;
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

	/** 残機表示をリセットする関数*/
	void ResetLives();

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

	/** ゲームのポインタ*/
	Game* m_game = nullptr;
	/** ポーズのポインタ*/
	Pause* m_pause = nullptr;
};

