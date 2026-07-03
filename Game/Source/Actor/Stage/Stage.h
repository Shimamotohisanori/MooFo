#pragma once
#include "Source/Actor/Actor.h"

/**
 * ステージクラス
 */
class CowFood;
class Stage : public Actor
{
public:
	Stage(){}
	~Stage(){}

public:
	/** 重たくならないように1ステップずつ初期化させる関数*/
	bool LoadStepByStep();

private:
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;


private:

	enum class InitStep
	{
		PerimeterFence,/** 外周フェンス*/
		InnerFence,    /** 内部フェンス */
		Mountain,      /** 外周にある山 */
		Ground,        /** 地面 */
		Collision,     /** 当たり判定 */	
		Num

	};
	InitStep m_initStep = InitStep::PerimeterFence;

	/** モデルの表示 */
	ModelRender m_perimeterFenceModelRender;
	ModelRender m_innerFenceModelRender;
	ModelRender m_mountainModelRender;
	ModelRender m_groundModelRender;

	/** 当たり判定を追加する */
	PhysicsStaticObject m_perimeterObject;
	PhysicsStaticObject m_innerObject;

	/** 牛の餌オブジェクト */
	CowFood* m_cowFood;

};

