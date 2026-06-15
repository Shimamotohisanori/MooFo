#pragma once
class CowFood;
class Player;
class Cow;
class CowLuring : public IGameObject
{
public:
	CowLuring(){}
	~CowLuring(){}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** 干し草の座標を設定する関数 */
	void SetPosition(const Vector3& pos);

	/** 干し草の座標を取得する関数 */
	const Vector3& GetPosition() const
	{
		return m_position;
	}
private:
	/** 干し草を置く処理 */
	void PutHay();

	/** 干し草の座標 */
	Vector3 m_position;

	/** 干し草のモデルレンダラー */
	ModelRender m_hayModelRender;

	/** 牛の餌クラス */
	CowFood* m_cowfood;

	/** プレイヤークラス */
	Player* m_player;

	/** 牛クラス */
	Cow* m_cow;


};

