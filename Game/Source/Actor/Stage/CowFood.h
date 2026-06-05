#pragma once

class CowFood : public IGameObject
{
public:
	CowFood(){}
	~CowFood(){}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	uint8_t GetFoodCount()
	{
		return m_foodCount;
	}

	void SetFoodCount(uint8_t foodCount)
	{
		m_foodCount = foodCount;
	}

private:

	/** 牛の餌を所持している数 */
	uint8_t m_foodCount = 0;

	/** UIを表示させるフラグ */
	bool m_isButtonUI = false; 

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
};