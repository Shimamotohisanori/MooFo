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
class CowLuring;
class CowFoodManager;
class VoiceManager;

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
	enum class EnCowType
	{
		en_Random, //ランダムに動く牛
		en_Light,  //UFOに向かって動く牛
		en_Chase,  //プレイヤーを追いかける牛
		en_Bonus, //ボーナス牛
		en_Num
	};

	/** 移動関数 */
	void Move()override;

	/** 回転関数 */
	void Rotation();

	/** アニメーション再生関数 */
	void PlayAnimation();

	/** 状態管理関数 */
	void ManageState();

	/** 牛の行動タイプを設定する関数 */
	void SetCowType(EnCowType type);

	/** 牛の行動タイプを取得する関数 */
	EnCowType GetCowType() const
	{
		return m_cowType;
	}

	/** 牛の位置を設定する関数 */
	void SetPosition(const Vector3& pos)
	{
		m_transform.SetPosition(pos);
	}

	void SetUFOAttracted(bool flag)
	{
		m_isUFOAttracted = flag;
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

	/** 牛がプレイヤーに捕獲されたかどうかを設定する関数 */
	void SetIsCaptured(bool isCaptured)
	{
		m_isCaptured = isCaptured;
	}

	/** 牛がプレイヤーに捕獲されたかどうかを取得する関数 */
	bool GetIsCaptured() const
	{
		return m_isCaptured;
	}

	/** 牛のステートを回転に変更する関数 */
	void ChangeRotationState()
	{
		m_rotationState = EnRotateState_Spin;
	}

	/** 牛がUFOに捕獲されたかどうかを取得する関数 */
	bool GetIsTakeAwayed()const
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
	UFO* GetTakingUFO() const
	{
		return m_takingUFO;
	}
		
	/** 牛が死んだフラグを取得する関数 */
	bool GetIsDeadFlag()const 
	{
		return m_isDeadFlag;
	}

	/** 牛が死んだフラグを設定する関数 */
	void SetIsDeadFlag(bool isDeadFlag)
	{
		m_isDeadFlag = isDeadFlag;
	}
	/** 回転ステートを動きに変える関数*/
	void ResetRotationState()
	{
		m_rotationState = EnRotateState_MoveDir;
	}
	/** 牛の移動のみを行っている牛の速度を取る関数*/
	Vector3 IsMoving()const
	{
		return m_moving;
	}

	/** 牛の削除予約を取得する関数 */
	bool GetIsPendingKill() const
	{
		return m_isPendingKill;
	}

	/** 餌をターゲットにしているかどうかを設定する関数 */
	void SetIsTargetFood(bool flag)
	{
		m_isTargetFood = flag;
	}

	/** 餌をターゲットにしているかどうかを取得する関数 */
	bool GetIsTargetFood() const
	{
		return m_isTargetFood;
	}

	/** 現在餌を食べているかどうかを取得する関数 */
	bool GetIsEating() const
	{
		return m_isEating;
	}

	/** プレイヤーを追いかけているかどうかを設定する関数 */
	void SetIsChasingPlayer(bool isChasing)
	{
		m_isChasingPlayer = isChasing;

		if (isChasing)
		{
			m_isTargetFood = false;
		}
	}

	/** プレイヤーを追いかけているかどうかを取得する関数 */
	bool GetIsChasingPlayer() const
	{
		return m_isChasingPlayer;
	}

	/** 牛を削除予定を見る関数 */
	void RequestKill();

	/** UFOに連れ去らわれる処理 */
	void TakeAwayedByUFO(Vector3 direction, float speed);

	/** キャラクターコントローラーを取得(ロープのゴースト判定用) */
	CharacterController& GetCharacterController()
	{
		return m_cowCharacterController;
	}


private:
	/** プレイヤーに引っ張られる関数 */
	void PulledByPlayer();

	/** プレイヤーに捕獲される関数 */
	void CapturedByPlayer();

	/** プレイヤーから逃げる関数 */
	void AvoidPlayer();

	/** 牛の餌が一番近い牛を探す関数 */
	void SearchNearestFood();
	
	/** 牛の餌に近づく関数 */
	void MoveToFood();

	/** 牛が餌を食べる最中の関数 */
	void Eating();

	/** 牛が納屋に入る関数 */
	void EnterBarn();

	/** */
	void MoveTowardUFO(UFO* ufo);
	
	/** プレイヤーと牛の距離の関数 */
	void ChasePlayer();

	/** 牛の種類のモデル関数 */
	void ApplyCowModel();

	/** 牛の餌を表示させる関数 */
	void CowSweat();

	UFO* FindNearestEmittingUFO();

	/** 牛の大きさの更新関数 */
	void UpdateScale();

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

	/** 現在ロックオンしているUFO(光が消えるまで固定、牛側が自発的に向かう先) */
	UFO* m_targetUFO = nullptr;

	/** ポーズ */
	Pause* m_pause = nullptr;

	/** ゲーム */
	Game* m_game = nullptr;
	
	/** タイマー */
	Timer* m_timer = nullptr;

	/** 牛の鳴き声のSE*/
	SoundSource* m_cowCrySE = nullptr;

	/** 牛が餌を食べるときのSE */
	SoundSource* m_cowEatSE = nullptr;

	/** 偽の牛 */
	DummyCow* m_dummyCow = nullptr;

	/* 牛のサウンドマネージャー**/
	SoundManager* m_CowSound = nullptr;

	/** 牛の餌 */
	CowLuring* m_CowLuring = nullptr;

	/** 牛の餌のマネージャー */
	CowFoodManager* m_cowfoodmanager = nullptr;

	/** 満腹状態のエフェクト */
	EffectEmitter* m_overdoseEffect = nullptr;

	/** 牛の汗エフェクト */
	EffectEmitter* m_cowSweatEffect = nullptr;

	/** ボイスマネージャー */
	VoiceManager* m_voiceManager = nullptr;

	enum EnCowState
	{
		
		EnRotateState_MoveDir,/** 移動*/
		EnRotateState_Spin,/** 回転*/
		EnRotateState_Num
	};
	
	/** 牛の回転ステート */
	EnCowState m_rotationState = EnRotateState_MoveDir;
  
	/** 牛の種類 */

	EnCowType m_cowType = EnCowType::en_Random;
	
	/** 牛のモデルレンダラー */
	ModelRender m_cowmodelRender;

	/** 牛のキャラクターコントローラー(当たり判定) */
	CharacterController m_cowCharacterController;

	/** 牛のステート */
	uint8_t m_cowState = 0;;

	/** 牛の移動方向 */
	Vector3 m_moveDir = Vector3::Zero;
	/** 休憩しない牛の移動速度*/
	Vector3 m_moving = Vector3::Zero;
	/** 牛の移動速度 */
	float m_moveSpeed = 30.0f;

	/** 牛の移動時間 */
	float m_moveTimer = 0;

	/** 牛が餌を食べている間の時間 */
	float m_eatTimer = 0.0f;

	/** 出現時のスケール */
	float m_spawnScale = 0.0f;

	/** プレイヤーが引っ張る抵抗の倍率 */
	float m_pullResistance = 1.0f;

	/** 逃げ始める距離 */
	float m_avoidDistance = 80.0f;

	/** 逃げる強さ(引っ張り時) */
	float m_avoidPower = 3.0f;

	/** 出現演出中のフラグ */
	bool m_isSpawning = true;

	/** 牛が餌を食べているかどうかのフラグ */
	bool m_isEating = false;

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

	/** 近くの餌を追いかけるフラグ */
	bool m_isTargetFood = false;
	/** 牛がUFOに向かって進むフラグ*/
	bool m_isUFOAttracted = false;

	/** 追いかけてくる牛かどうか */
	bool m_isChasingPlayer = false;

	/** モデルが初期化済みかどうか */
	bool m_isModelInitialized = false;

	enum EnAnimation
	{
		EnAnimation_Idle,/** 待機*/
	    EnAnimation_Walk,/** 歩き*/
		EnAnimation_Num
	};
	AnimationClip animationClips[EnAnimation_Num];
};

