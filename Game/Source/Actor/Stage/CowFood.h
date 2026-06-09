#pragma once
class Rope;
class CountDown;
class LoadingScene;
class CowFood : public IGameObject
{
public:
	CowFood(){}
	~CowFood();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** 現在の牛の餌を所持している数を取得する関数 */
	uint8_t GetFoodCount() const
	{
		return m_foodCount;
	}

	/** 現在の牛の餌を所持している数を設定する関数 */
	void SetFoodCount(uint8_t foodCount)
	{
		m_foodCount = foodCount;
	}

	/** 牛の餌を置くかどうかのフラグを取得する関数 */
	bool GetIsPutFood() const
	{
		return m_isPutFood;
	}

	/** 牛の餌を置くかどうかのフラグを設定する関数 */
	void SetIsPutFood(bool isPutFood)
	{
		m_isPutFood = isPutFood;
	}

private:

	void CowFoodPut();

	/** 牛の餌を所持している数 */
	uint8_t m_foodCount = 2;

	/** UIを表示させるフラグ */
	bool m_isButtonUI = false; 

	/** 牛の餌が置かれたか確認するフラグ */
	bool m_isPutFood = false;

	/** プレイヤーが餌を置いたか確認するフラグ */
	bool m_isPutPlayer = false;

	/** 牛の餌のポジション */
	Vector3 m_position;

	/** 牛の餌のモデル */
	ModelRender m_cowFoodModelRender;

	/** Aボタンを描画させる画像 */
	SpriteRender m_Abutton;

	/** 餌を描画させる画像 */
	SpriteRender m_bucketFood;
	SpriteRender m_bucketFood_Second;

	/** 餌のシルエットを描画させる画像 */
	SpriteRender m_bucketFood_Black;
	SpriteRender m_bucketFood_SecondBlack;

	/** 牛の餌の当たり判定 */
	PhysicsStaticObject m_FoodObject;

	/** 牛の餌のエフェクト */
	EffectEmitter* m_cowFoodEffect;

	/** 縄のクラス */
	Rope* m_rope = nullptr;

	/** カウントダウン */
	CountDown* m_CountDown = nullptr;
};