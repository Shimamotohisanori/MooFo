#pragma once
class Score :public IGameObject
{
public:
	Score();
	~Score();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//牛を救出した時にスコアを取得する関数
	void AddScore(int Value);
	//牛を救出できなかった時にスコアを減少させる関数
	void DecreaseScore(int Value);
	//スコアの描画処理
	void TextScore();
	//ゲームクリア・ゲームオーバーのときにスコアを表示するかどうかのフラグを立てる関数
	void SetResult(bool isResult);
	//スコアをゲットする関数
	int GetScore() const;

	/** リザルトの種類 */
	enum class ResultType
	{

		GameClear,
		GameOver,
	};
	
	void SetResultType(ResultType type)
	{
		m_resultType = type;
	}
	ResultType m_resultType = ResultType::GameClear;
private:
	int m_score = 0;
	int m_prevScore = -1;
	//ゲームクリア・ゲームオーバーのときにスコアを表示するかどうかのフラグ
	bool m_isResult = false;


//５桁(万・千・百・十・一)の五桁を描画するスプライト
	SpriteRender m_digitSprite[5][10];
	//数字の画像をロード
	const char* m_digitPaths[10] =
	{
		"Assets/sprite/NumberUI/MooFoNumberUI0.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI1.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI2.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI3.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI4.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI5.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI6.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI7.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI8.dds",
		"Assets/sprite/NumberUI/MooFoNumberUI9.dds",
	};

	
};

