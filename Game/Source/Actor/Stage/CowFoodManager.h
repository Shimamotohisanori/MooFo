#pragma once
#include <vector>
class CowLuring;
class CowFoodManager : public IGameObject
{
public:
	CowFoodManager(){}
	~CowFoodManager();

	bool Start();
	void Update();

	void RemoveFood(CowLuring* food);

	/** 餌を生成する関数 */
	void SpawnFood(const Vector3& pos);

	/** 餌のリストを取得する関数 */
	const std::vector<CowLuring*>& GetFoodList() const
	{
		return m_foodList;
	}

	/** 牛の餌を全て消す関数 */
	void ClearAllFood();


private:
	/** 指定した餌に最も近い牛を探してターゲットフラグを立てる関数 */
	void SetNearestCow(CowLuring* food);

	/** 牛の餌のリストを作る */
	std::vector<CowLuring*> m_foodList;
};