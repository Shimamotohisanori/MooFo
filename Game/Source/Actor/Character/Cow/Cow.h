#pragma once
#include"Source/Actor/Character/Character.h"
class Rope;
class Player;
class CountDown;
class UFO;
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

		void SetPosition(const Vector3& pos)
		{
			m_transform.SetPosition(pos);
		}

		Vector3 GetPosition()
		{
			return m_transform.GetPosition();
		}

		Quaternion GetRotation()
		{
			return m_transform.GetRotation();
		}

		void SetIsCaptured(bool isCaptured)
		{
			m_isCaptured = isCaptured;
		}

		void ChangeRotationState()
		{
			m_rotationState = EnRotateState_Spin;
		}

		bool GetIsTakeAwayed()
		{
			return m_isTakeAwayed;
		}

		void SetIsTakeAwayed(bool isTakeAwayed)
		{
			m_isTakeAwayed = isTakeAwayed;
		}

		void SetTakingUFO(UFO* takingUFO)
		{
			m_takingUFO = takingUFO;
		}

		UFO* GetTakingUFO()
		{
			return m_takingUFO;
		}

private:
	//プレイヤーに引っ張られる関数
	void PulledByPlayer();

	//プレイヤーに捕獲される関数
	void CapturedByPlayer();

	/** プレイヤーから逃げる関数 */
	void AvoidPlayer();

private:
	//ロープ
	Rope* m_rope;

	//プレイヤー
	Player* m_player;

	//カウントダウン
	CountDown* m_countdown;

	//UFO
	UFO* m_takingUFO = nullptr;

	enum EnRotateState
	{
		EnRotateState_MoveDir,
		EnRotateState_Spin,
		EnRotateState_Num
	};
	
	/** 牛の回転ステート */
	EnRotateState m_rotationState = EnRotateState_MoveDir;
	
	/** 牛のモデルレンダラー */
	ModelRender m_cowmodelRender;

	uint8_t m_cowState;//牛のステート

	Vector3 m_moveDir = Vector3::Zero;//移動方向
	float m_moveSpeed = 50.0f;//移動速度
	int m_moveTimer = 0;//移動タイマー
	bool m_isMove = false;//移動しているかどうか

	bool m_isCaptured = false;//自身がロープに捕まったかどうかのフラグ

	bool m_isTakeAwayed = false;//UFOに連れて行かれたかどうかのフラグ

	
	enum EnAnimation
	{
		EnAnimation_Idle,
	    EnAnimation_Walk,
		EnAnimation_Num
	};
	AnimationClip animationClips[EnAnimation_Num];
};

