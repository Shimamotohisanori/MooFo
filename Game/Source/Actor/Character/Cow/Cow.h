#pragma once
#include"Source/Actor/Character/Character.h"
class Cow : public Character
{
public:
	Cow();
	~Cow();

	public:
		bool Start() override;
		void Update() override;
		void Render(RenderContext& rc) override;
public:
		void Move()override;
		void Rotation();
		void PlayAnimation();
		void ManageState();
public:
	enum EnRotationState
	{
		EnRotationState_MoveDir,
		EnRotatitonState_Spin,
		EnRotationState_Num
	};
	EnRotationState m_rotationState = EnRotationState_MoveDir;//回転ステート
private:
	ModelRender m_CowmodelRender;
	uint8_t m_cowState;//プレイヤーステート

	Vector3 m_moveDir = Vector3::Zero;//移動方向
	float m_moveSpeed = 50.0f;//移動速度
	int m_moveTimer = 0;//移動タイマー
	enum EnAnimation
	{
		EnAnimation_Idle,
	    EnAnimation_Walk,
		EnAnimation_Num
		//EnAnimation_Run,
		//EnAnimation_Jump,
	};
	AnimationClip animationClips[EnAnimation_Num];
};

