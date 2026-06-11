#include "stdafx.h"
#include "CowFoodManager.h"
#include "Source/Actor/Character/Cow/CowLuring.h"
#include "CowFood.h"

bool CowFoodManager::Start()
{
	return true;
}

void CowFoodManager::Update()
{

}

void CowFoodManager::SpawnFood(const Vector3& pos)
{
	/** もし牛の餌が二個以上ステージ上に置かれたら */
	if (m_foodList.size() >= 2)
	{
		/** ゲーム世界の中から先頭の要素をを消す */
		DeleteGO(m_foodList.front());

		/** 管理リストから一番最初のリストを消す */
		m_foodList.erase(m_foodList.begin());
	}

	/** 新しく餌を設置する */
	CowLuring* food = NewGO<CowLuring>(0);

	/** プレイヤー位置を設置 */
	food->SetPosition(pos);

	/** 一番後ろのリストに牛の餌を追加 */
	m_foodList.push_back(food);
}