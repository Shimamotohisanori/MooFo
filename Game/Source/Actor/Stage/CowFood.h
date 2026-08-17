#pragma once
class Rope;
class CountDown;
class CowFoodManager;
class LoadingScene;
class Game;
class Pause;
class CowLivesUI;
class CowFood : public IGameObject
{
public:
	CowFood() {}
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
	uint8_t m_foodCount = 3;

	/** 餌を設置した後のクールタイム */
	float m_coolTime = 0.0f;

	/** 餌を回収する際のクールタイム */
	float m_collectCoolTime = 0.0f;

	/** クールタイム中かどうか */
	bool m_isCoolTime = false;

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

	/** 餌を描画させる画像 */
	SpriteRender m_bucketFood;
	SpriteRender m_bucketFood_Second;
	SpriteRender m_bucketFood_Third;

	/** 餌のシルエットを描画させる画像 */
	SpriteRender m_bucketFood_Black;
	SpriteRender m_bucketFood_SecondBlack;
	SpriteRender m_bucketFood_ThirdBlack;

	/** 牛の餌の禁止状態のUI */
	SpriteRender m_tabooCowFood;
	SpriteRender m_tabooCowFood_Second;
	SpriteRender m_tabooCowFood_Third;

	/** 牛の餌の当たり判定 */
	PhysicsStaticObject m_FoodObject;

	/** Aボタンのエフェクト */
	EffectEmitter* m_AbuttonEffect;
	
	/** 牛の餌のエフェクト */
	EffectEmitter* m_cowFoodEffect;

	/** 牛舎付近のエフェクト */
	EffectEmitter* m_barnNearEffect = nullptr;

	/** 牛の餌を置くエフェクト */
	EffectEmitter* m_foodAreaEffect = nullptr;

	/** 縄のクラス */
	Rope* m_rope = nullptr;
	/** ゲームクラス*/
	Game* m_game = nullptr;

	/** ポーズ画面のクラス */
	Pause* m_pause = nullptr;

	/** カウントダウン */
	CountDown* m_CountDown = nullptr;

	/** 牛の餌の管理 */
	CowFoodManager* m_CowFoodManager = nullptr;
	/** プレイヤーの残機のUI*/
	CowLivesUI* m_cowLivesUI = nullptr;


	/** 牛の餌を置く音 */
	SoundSource* m_puthaySE = nullptr;

	/** 牛の餌を取る音 */
	SoundSource* m_takehaySE = nullptr;

	/** 餌禁止のエリアにいるかどうか */
	bool m_isInTabooArea = false;
};