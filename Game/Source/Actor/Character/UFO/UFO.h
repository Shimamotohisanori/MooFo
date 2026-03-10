#pragma once
#include"Source/Actor/Character/Character.h"
class UFO : public Character
{
	public:
	UFO();
	~UFO();
	public:
		bool Start() override;
		void Update() override;
		void Render(RenderContext& rc) override;
		void Move()override;
		void Rotation();

		void SetPosition(const Vector3& pos)
		{
			m_transform.SetPosition(pos);
		}
		enum EnUFOState
		{
			EnUFOState_Move,
			EnUFOState_Idle,
			EnUFOState_Num
		};
		EnUFOState m_UFOState = EnUFOState_Move;//プレイヤーステート
private:
	ModelRender m_UFOmodelRender;
	Vector3 m_UfOpos = Vector3(0.0f, 70.0f, 0.0f);
	Vector3 m_moveDir = Vector3::Zero;//移動方向
	float m_moveSpeed = 50.0f;//移動速度
	int m_moveTimer = 0;//移動タイマー

};

