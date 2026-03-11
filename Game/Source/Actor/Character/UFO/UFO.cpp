#include "stdafx.h"
#include "UFO.h"
#include <time.h>
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
	srand(time(nullptr));
	
	m_UFOmodelRender.Init(FILEPATH);
	return true;
}

void UFO::Update()
{
	if (m_UFOState == EnUFOState_Move)
	{
		/*移動*/
		Move();
		/*回転*/
		Rotation();
	}
	/*モデルの位置を反映*/
	m_UFOmodelRender.SetPosition(m_transform.GetPosition());
	/*モデルの回転を反映*/
	m_UFOmodelRender.SetRotation(m_transform.GetRotation());
	/*モデルの更新*/
	m_UFOmodelRender.Update();
}
		

void UFO::Move()
{
	
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
	m_UFOmodelRender.SetPosition(m_transform.GetPosition());

	//タイマーを減らす。
	m_moveTimer--;

}
	
void UFO::Rotation()
{

	//少しでも動いたら移動方向に向きを回転させる。
	if (fabsf(m_moveDir.x) >= 0.0001f || fabsf(m_moveDir.z) >= 0.0001f)
	{
		//移動方向に回転させる
		m_transform.GetRotation().SetRotationYFromDirectionXZ(m_moveDir);
		m_transform.SetRotation(m_transform.GetRotation());
	}


}

void UFO::Render(RenderContext& rc)
{
	m_UFOmodelRender.Draw(rc);
}

