#pragma once
#include"Source/Actor/Character/Character.h"
#include "Source/Actor/Character/UFO/CowCaptureController.h"

class CountDown;
class Cow;
class Score;
class CowCaptureController;
class Pause;
class Game;
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
		m_ufomodelRender.SetPosition(pos);
	}

	Vector3 GetPosition()
	{
		return m_transform.GetPosition();
	}

	/** 牛を連れて行けるかどうかのフラグを設定する関数 */
	void SetIsCowTakeAwayed(bool isCowTakeAwayed)
	{
		m_isCowTakeAwayed = isCowTakeAwayed;
	}

	/** 牛を連れて行けるかどうかのフラグを取得する関数 */
	bool GetIsCowTakeAwayed()
	{
		return m_isCowTakeAwayed;
	}

	CowCaptureController* GetCowCaptureController();

	/** 目標にしている牛を消す関数 */
	void ReMoveTargetCow()
	{
		m_targetCow = nullptr;
	}

	bool IsLightEmitting()const
	{
		return m_cowCaptureController->GetIsEmitting();
	}
	
	


private:
	/* 牛を連れていく関数 */
	void TakeAwayTheCow();

	/** 牛を見つける関数 */
	void FindTheCow();

	/** UFO同士の反発関数 */
	void ApplyUFOAvoidance(Vector3& pos);

	/** UFOが制限区域を出ないようにする関数 */
	void ClampToArea(Vector3& pos);

	/** UFOのアップデートができるかどうかを判断する関数 */
	bool CanUFOUpdate();

private:
	ModelRender m_ufomodelRender;
	
	Vector3 m_moveDir = Vector3::Zero;//移動方向
	
	float m_moveSpeed = 50.0f;//移動速度

	float m_moveTimer = 0.0f;//移動タイマー
	
	CountDown* m_countdown = nullptr;

	/** UFOに見つかった牛 */
	Cow* m_targetCow = nullptr;

	/** ゲームシーン */
	Game* m_game = nullptr;

	/*スコア*/
	Score* m_score = nullptr;

	/** UFOの牛捕獲コントローラー */
	CowCaptureController* m_cowCaptureController = nullptr;
	
	/** ポーズ */
	Pause* m_pause = nullptr;

	/** 牛を連れて行けるかどうかのフラグ */
	bool m_isCowTakeAwayed = false;

	/** 牛を追いかけるフラグ */
	bool m_isChasing = false;

	enum EnUFOState
	{
		EnUFOState_Move,
		EnUFOState_Idle,
		EnUFOState_Num
	};
	/** UFOの状態 */
	EnUFOState m_UFOState = EnUFOState_Move;
};

