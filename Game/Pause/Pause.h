#pragma once
class Game;
class SoundPause;
class SoundManager;
class LoadingScene;
class Timer;
class Pause : public IGameObject
{
public:
	Pause() {}
	~Pause() {}
	bool Start();
	void Update();
	void Select();
	void Choice();
	void Render(RenderContext& rc);

	/** ポーズ中に選んでいる場所の番号設定関数 */
	void SetCountNumber(int countNumber)
	{
		m_countNumber = countNumber;
	}

	/** ポーズ中に選んでいる場所の番号取得関数 */
	int GetCountNumber()const
	{
		return m_countNumber;
	}

	/** ポーズ中かどうかのフラグ設定関数 */
	void SetIsPause(bool flag)
	{
		m_isPause = flag;
	}

	/** ポーズ中かどうかのフラグ取得関数 */
	bool GetIsPause()const
	{
		return m_isPause;
	}


private:
	/** 選択中の画面を表示させる番号 */
	int m_countNumber = 0;

	SoundManager* m_choiceSound = nullptr;
	SoundSource* p_chiceSE = nullptr;
	SoundSource* p_DecisionSE = nullptr;

	/** ポーズ中かどうかのフラグ*/
	bool m_isPause = false;
	/** ゲームのポインタ*/
	Game* m_game = nullptr;	

	/** ローディングシーンのポインタ*/
	LoadingScene* m_loadingScene = nullptr;
	/** サウンドポーズのポインタ*/
	SoundPause* m_soundPause = nullptr;
	/** Pasue中に使用する画像一覧*/
	/** Pause背景*/
	SpriteRender m_pauseBackGround;
	/** 「ゲームをやめる」を表示するアイコン*/
	SpriteRender m_quiteSprite;
	/**「ゲームに戻る」を表示するアイコン*/
	SpriteRender m_resumeSprite;
	/** 「音量」を表示するアイコン*/
	SpriteRender m_soundSprite;	
	/** 選択していないときに出す黒い背景*/
	SpriteRender m_soundBlackSprite;
	SpriteRender m_resumeBlackSprite;
	SpriteRender m_quiteBlackSprite;

};