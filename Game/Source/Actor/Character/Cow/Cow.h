#pragma once
#include"Source/Actor/Character/Character.h"
class Rope;
class Player;
class CountDown;
class UFO;
class Pause;
class Game;
class SoundManager;
class DummyCow;
class Timer;
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
	/** 移動関数 */
	void Move()override;

	/** 回転関数 */
	void Rotation();

	/** アニメーション再生関数 */
	void PlayAnimation();

	/** 状態管理関数 */
	void ManageState();

	/** 牛の位置を設定する関数 */
	void SetPosition(const Vector3& pos)
	{
		m_transform.SetPosition(pos);
	}

	/** 牛の位置を取得する関数 */
	Vector3 GetPosition()
	{
		return m_transform.GetPosition();
	}

	/** 牛の回転を取得する関数 */
	Quaternion GetRotation()
	{
		return m_transform.GetRotation();
	}

	/** 牛の回転を設定する関数 */
	void SetRotation(const Quaternion& rot)
	{
		m_transform.SetRotation(rot);
	}

	/** 牛が捕獲されたかどうかを設定する関数 */
	void SetIsCaptured(bool isCaptured)
	{
		m_isCaptured = isCaptured;
	}

	/** 牛のステートを回転に変更する関数 */
	void ChangeRotationState()
	{
		m_rotationState = EnRotateState_Spin;
	}

	/** 牛が捕獲されたかどうかを取得する関数 */
	bool GetIsTakeAwayed()
	{
		return m_isTakeAwayed;
	}

	/** 牛がUFOに連れて行かれたかどうかを設定する関数 */
	void SetIsTakeAwayed(bool isTakeAwayed)
	{
		m_isTakeAwayed = isTakeAwayed;
	}

	/** 対象のUFOを設定する関数 */
	void SetTakingUFO(UFO* takingUFO)
	{
		m_takingUFO = takingUFO;
	}

	/** 対象のUFOを取得する関数 */
	UFO* GetTakingUFO()
	{
		return m_takingUFO;
	}
		
	/** 牛が死んだフラグを取得する関数 */
	bool GetIsDeadFlag()
	{
		return m_isDeadFlag;
	}

	/** 牛を削除予定を見る関数 */
	void RequestKill();


private:
	/** プレイヤーに引っ張られる関数 */
	void PulledByPlayer();

	/** プレイヤーに捕獲される関数 */
	void CapturedByPlayer();

	/** プレイヤーから逃げる関数 */
	void AvoidPlayer();
	
	/** アップデートできるかどうかを判断する関数 */
	bool CanUpdate();


private:
	/** ロープ */
	Rope* m_rope = nullptr;

	/** プレイヤー */
	Player* m_player = nullptr;

	/** カウントダウン */
	CountDown* m_countdown = nullptr;

	/** UFO */
	UFO* m_takingUFO = nullptr;

	/** ポーズ */
	Pause* m_pause = nullptr;

	/** ゲーム */
	Game* m_game = nullptr;
	
	/** タイマー */
	Timer* m_timer = nullptr;

	/** 牛の鳴き声のSE*/
	SoundSource* m_CowCrySE = nullptr;

	/** 偽の牛 */
	DummyCow* m_dummyCow = nullptr;

	/* 牛のサウンドマネージャー**/
	SoundManager* m_CowSound = nullptr;

	enum EnCowState
	{
		EnRotateState_MoveDir,
		EnRotateState_Spin,
		EnRotateState_Num
	};
	
	/** 牛の回転ステート */
	EnCowState m_rotationState = EnRotateState_MoveDir;
	
	/** 牛のモデルレンダラー */
	ModelRender m_cowmodelRender;

	/** 牛のステート */
	uint8_t m_cowState;

	/** 牛の移動方向 */
	Vector3 m_moveDir = Vector3::Zero;

	/** 牛の移動速度 */
	float m_moveSpeed = 50.0f;

	/** 牛の移動時間 */
	float m_moveTimer = 0;

	/** 牛が移動しているかどうか */
	bool m_isMove = false;

	/** 牛のSEを一度だけ鳴らすためのフラグ*/
	bool m_CowSE = false;

	/** 牛がロープに捕まったかどうか */
	bool m_isCaptured = false;

	/** 牛がUFOに連れ去られているかどうか */
	bool m_isTakeAwayed = false;
	
	/** 牛が死んだフラグ */
	bool m_isDeadFlag = false;

	/** 牛の削除フラグ */
	bool m_isPendingKill = false;

	enum EnAnimation
	{
		EnAnimation_Idle,
	    EnAnimation_Walk,
		EnAnimation_Num
	};
	AnimationClip animationClips[EnAnimation_Num];
};

