#include "stdafx.h"
#include "CowFoodManager.h"
#include "Source/Actor/Character/Cow/CowLuring.h"
#include "Source/Actor/Character/Cow/Cow.h"

CowFoodManager::~CowFoodManager()
{
	/** 牛の餌を全て消す */
	ClearAllFood();
}

bool CowFoodManager::Start()
{
	return true;
}

void CowFoodManager::Update()
{
	/** 縮小完了した餌を削除する */
	auto it = m_foodList.begin();
	while (it != m_foodList.end())
	{
		if ((*it)->IsShrinkDone())
		{
			DeleteGO(*it);
			it = m_foodList.erase(it);
		}
		else
		{
			++it;
		}
	}

	/** 餌をターゲットにしているかどうかを取得する関数 */
	for (auto food : m_foodList)
	{
		SetNearestCow(food);
	}
}

void CowFoodManager::RemoveFood(CowLuring* food)
{
	/** nullptrなら何もしない */
	if (food == nullptr)
	{
		return;
	}

	/** リストから対象の餌を検索する */
	auto it = std::find(m_foodList.begin(), m_foodList.end(), food);

	if (it != m_foodList.end())
	{
		/** 先にリストから除去してから餌のオブジェクトを破棄する */
		m_foodList.erase(it);
		DeleteGO(food);
	}
}

void CowFoodManager::SpawnFood(const Vector3& pos)
{
	/** もし牛の餌が3個以上ステージ上に置かれたら */
	if (m_foodList.size() >= 3)
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
	/** シーン内の全ての牛を取得する */
	auto cows = FindGOs<Cow>("cow");

	for (auto cow : cows)
	{
		cow->SetIsTargetFood(false);
	}

	Cow* nearestCow = nullptr;

	float nearestDistSq = FLT_MAX;

	/** 食べていない牛の中から餌に最も近い牛を探す */
	for (auto cow : cows)
	{
		
		/** 食べている最中の牛はターゲット選択から除外する */
		Vector3 diff = cow->GetPosition() - food->GetPosition();

		diff.y = 0.0f;

		float distSq = diff.LengthSq();

		if (distSq < nearestDistSq)
		{
			nearestDistSq = distSq;
			nearestCow = cow;
		}
	}

	/** 食べていない牛のターゲットフラグを一旦リセットする */
	for (auto cow : cows)
	{
		if (cow->GetIsEating()) continue;
		cow->SetIsTargetFood(false);
	}

	/** 最近接の牛にターゲットフラグを立てる */
	if (nearestCow)
	{
		nearestCow->SetIsTargetFood(true);
	}
}

void CowFoodManager::ClearAllFood()
{
	for (auto food : m_foodList)
	{
		DeleteGO(food);
	}
	m_foodList.clear();
}
