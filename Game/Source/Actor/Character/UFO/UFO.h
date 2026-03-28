#pragma once
#include"Source/Actor/Character/Character.h"
class CountDown;
class Cow;
class Score;
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

	/** 牛を連れて行けるかどうかのフラグを設定する関数 */
	void SetIsCowTakeAwayed(bool isCowTakeAwayed)
	{
		m_isCowTakeAwayed = isCowTakeAwayed;
	}

	/** 目標にしている牛を消す関数 */
	void ReMoveTargetCow()
	{
		m_targetCow = nullptr;
	}

	enum EnUFOState
	{
		EnUFOState_Move,
		EnUFOState_Idle,
		EnUFOState_Num
	};
	EnUFOState m_UFOState = EnUFOState_Move;//プレイヤーステート


private:
	/* 牛を連れていく関数 */
	void TakeAwayTheCow();

	/** 牛を見つける関数 */
	void FindTheCow();

private:
	ModelRender m_ufomodelRender;
	Vector3 m_UfOpos = Vector3(0.0f, 70.0f, 0.0f);
	Vector3 m_moveDir = Vector3::Zero;//移動方向
	float m_moveSpeed = 50.0f;//移動速度
	int m_moveTimer = 0;//移動タイマー
	CountDown* m_countdown;

	/** UFOに見つかった牛 */
	Cow* m_targetCow = nullptr;
	/*スコア*/
	Score* m_score;
	/** 牛を連れて行けるかどうかのフラグ */
	bool m_isCowTakeAwayed = false;
};

