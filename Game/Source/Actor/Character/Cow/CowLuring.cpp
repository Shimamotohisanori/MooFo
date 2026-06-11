#include "stdafx.h"
#include "CowLuring.h"
#include "Source/Actor/Stage/CowFood.h"
#include "Source/Actor/Character/Player/Player.h"

namespace
{
	/** 干し草のモデルファイルパス */
	const char* COW_FOOD_FILEPATH = "Assets/modelData/CowFood/hay.tkm";
}

bool CowLuring::Start()
{
	/** 干し草のモデルを初期化する */
	m_hayModelRender.Init(COW_FOOD_FILEPATH);

	/** 干し草の初期座標を設定する */
	m_hayModelRender.SetPosition(m_position);
	return true;
}

void CowLuring::Update()
{

}

void CowLuring::PutHay()
{

}

void CowLuring::SetPosition(const Vector3& pos)
{
	/** 座標をメンバ変数に保存する */
	m_position = pos;

	/** モデルの座標を更新する */
	m_hayModelRender.SetPosition(pos);
}

void CowLuring::Render(RenderContext& rc)
{

	m_hayModelRender.Draw(rc);
}
