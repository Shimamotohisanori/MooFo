#pragma once
class Score;
class CowNumberOfRescues;
class GameClear :public IGameObject
{
	public:
	GameClear();
	~GameClear();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//ゲームクリアで行う処理
	void InGameClear();

	//スコアをセットする関数
	void SetFinalClearScore(int score);

	//救出数をセットする関数
	void SetFinalClearRescue(int rescue);
private:
	SpriteRender m_GameClearSpriteRender;//ゲームクリアのスプライトレンダー
	bool m_isDeleteRequst = false;//削除要求フラグ


	int m_finalScore;//最終スコアを保存する変数
	int m_finalRescue;//最終救出数を保存する変数
	bool m_isScoreSet = false;//スコアがセットされたかどうかのフラグ
	bool m_isRescueSet = false;//救出数がセットされたかどうかのフラグ

	SoundManager* m_clearSound;
	SoundSource* p_clearBGM;
	Score* m_score;
	CowNumberOfRescues* m_cowNumberOfRescues;

};

