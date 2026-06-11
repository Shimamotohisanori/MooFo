#pragma once
#include <vector>
class CowLuring;
class CowFoodManager : public IGameObject
{
public:
	CowFoodManager(){}
	~CowFoodManager(){}

	bool Start();
	void Update();

	/** 餌を生成する関数 */
	void SpawnFood(const Vector3& pos);

	const std::vector<CowLuring*>& GetFoodList() const
	{
		return m_foodList;
	}
private:
	/** 牛の餌のリストを作る */
	std::vector<CowLuring*> m_foodList;
};