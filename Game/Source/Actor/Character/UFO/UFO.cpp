#include "stdafx.h"
#include "UFO.h"
#include <time.h>
#include"CountDown/CountDown.h"
#include "Source/Actor/Character/Cow/Cow.h"
namespace
{
	const char* FILEPATH = "Assets/modelData/UFO/UFO2.tkm"; //enModelUpAxis = enModelUpAxisZ;

}
UFO::UFO()
{

}

UFO::~UFO()
{

}


bool UFO::Start()
{
	m_countdown = FindGO<CountDown>("countdown");
	srand(time(nullptr));
	m_ufomodelRender.SetScale(Vector3{3.5f,3.5f,3.5f});
	m_ufomodelRender.Init(FILEPATH);
	m_ufomodelRender.SetPosition(m_transform.GetPosition());
	m_ufomodelRender.Update();
	return true;
}

void UFO::Update()
{
	/** カウントダウン中はUFOを動かさない */
	if (m_countdown->GetCountDown())
	{
		return;
	}

	if (m_UFOState == EnUFOState_Move)
	{
		/** 移動 */
		Move();
		/** 回転 */
		Rotation();
	}

	/** 牛を見つける関数 */
	FindTheCow();

	/** モデルの位置を反映 */
	m_ufomodelRender.SetPosition(m_transform.GetPosition());
	/** モデルの回転を反映 */
	m_ufomodelRender.SetRotation(m_transform.GetRotation());
	/** モデルの更新 */
	m_ufomodelRender.Update();
}
		

void UFO::Move()
{
	/** 牛を連れていってる最中は移動させない */
	if(m_isCowTakeAwayed)
	{
		return;
	}

	if (m_moveTimer <= 0)
	{
		Vector3 dir
		(
			//(0,1,2,から-1を引いているので)-1,0,1の範囲でランダムな値を生成
			rand() % 3 - 1,//x
			0,             //yは常に0
			rand() % 3 - 1//z
		);
		// 0,0,0になったら一秒休む
		if (dir.LengthSq() == 0)
		{
			m_moveDir = Vector3::Zero;
			m_moveTimer = 60.0f;//1秒休む
		}
		else
		{
			dir.Normalize();
			m_moveDir = dir;
			m_moveTimer = 120.0f;//2秒ごとに方向を変える
		}
	}
	//移動
	Vector3 pos = m_transform.GetPosition();
	//少しづつ位置を動かす
	pos += m_moveDir * m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	//ポジションを更新
	m_transform.SetPosition(pos);
	//モデルに位置を反映
	m_ufomodelRender.SetPosition(m_transform.GetPosition());

	//タイマーを減らす。
	m_moveTimer--;

}
	
void UFO::Rotation()
{
	/** 牛を連れていってる最中は回転させない */
	if(m_isCowTakeAwayed)
	{
		return;
	}

	/** 少しでも動いたら移動方向に向きを回転させる */
	if (fabsf(m_moveDir.x) >= 0.0001f || fabsf(m_moveDir.z) >= 0.0001f)
	{
		//移動方向に回転させる
		m_transform.GetRotation().SetRotationYFromDirectionXZ(m_moveDir);
		m_transform.SetRotation(m_transform.GetRotation());
	}
}

void UFO::TakeAwayTheCow()
{
	/** 牛を連れていけるかどうかのフラグが立っていたら処理をする */
	if (m_isCowTakeAwayed)
	{
		/** 牛を回転させる */
		auto cow = FindGOs<Cow>("cow");
		for (auto c : cow)
		{
			c->
		}
	}
}

void UFO::FindTheCow()
{
	/** 牛を連れていけるかどうかのフラグが立っていたら処理しない */
	if (m_isCowTakeAwayed) return;

	auto cow = FindGOs<Cow>("cow");
	for (auto c : cow)
	{
		/** 牛とUFOの距離が100未満だったら牛を連れていく */
		if ((c->GetPosition() - m_transform.GetPosition()).LengthSq() < 10000.0f)
		{
			/** 連れていける */
			m_isCowTakeAwayed = true;
		}
	}
}

void UFO::Render(RenderContext& rc)
{
	m_ufomodelRender.Draw(rc);
}

