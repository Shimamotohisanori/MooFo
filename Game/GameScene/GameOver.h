#pragma once
class Score;
class CowNumberOfRescues;
class LoadingScene;
class Title;
class GameOver : public IGameObject
{
public:
	GameOver();
	~GameOver();

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//GameOver時に行う処理
	void InGameOver();
	//スコアをセットする関数
	void SetFinalScore(int score);
	//救出数をセットする関数
	void SetFinalRescue(int rescue);


private:
	SpriteRender m_GameOverspriteRender;

	/** 黒い背景用のスプライトレンダラー */
	SpriteRender m_blackSpriteRender;

	//スコアの表示をコピーするための変数
	int m_finalScore;
	bool m_isDeleteRequest = false;
	bool m_isScoreSet = false;

	int m_finalRescue = 0;
	bool m_isRescueSet = false;
	SoundManager* m_deathSound;
	Score*m_score;
	CowNumberOfRescues* m_cowNumberOfRescues;
	SoundSource* p_deathBGM;
	LoadingScene* m_loadingScene;
	
};

