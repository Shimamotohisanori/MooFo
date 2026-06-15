#include "stdafx.h"
#include "CowFoodManager.h"
#include "Source/Actor/Character/Cow/CowLuring.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "CowFood.h"

bool CowFoodManager::Start()
{
	return true;
}

void CowFoodManager::Update()
{
	for (auto food : m_foodList)
	{
		SetNearestCow(food);
	}
}

void CowFoodManager::RemoveFood(CowLuring* food)
{
	if (food == nullptr)
	{
		return;
	}

	auto it = std::find(m_foodList.begin(), m_foodList.end(), food);

	if (it != m_foodList.end())
	{
		m_foodList.erase(it);
		DeleteGO(food);
	}
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

void CowFoodManager::SetNearestCow(CowLuring* food)
{
	auto cows = FindGOs<Cow>("cow");

	for (auto cow : cows)
	{
		cow->SetIsTargetFood(false);
	}

	Cow* nearestCow = nullptr;

	float nearestDistSq = FLT_MAX;

	for (auto cow : cows)
	{
		Vector3 diff = cow->GetPosition() - food->GetPosition();

		diff.y = 0.0f;

		float distSq = diff.LengthSq();

		if (distSq < nearestDistSq)
		{
			nearestDistSq = distSq;
			nearestCow = cow;
		}
	}

	if (nearestCow)
	{
		nearestCow->SetIsTargetFood(true);
	}
}