#pragma once
#include"Source/Actor/Character/Character.h"
#include "Source/Actor/Character/UFO/CowCaptureController.h"

class CountDown;
class Cow;
class Score;
class CowCaptureController;
class Pause;
class Game;
class Timer;
class DecreaseTimerUI;
class CowLivesUI;
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

		if (m_spawnPos.LengthSq() == 0.0f)
		{
			m_spawnPos = pos;
		}
	}

	void StartSpawnAnimation(const Vector3& finalPos);


	Vector3 GetSpawnPosition() const
	{
		return m_spawnPos;
	}

	Vector3 GetPosition()
	{
		return m_transform.GetPosition();
	}

	/** UFOの移動状態を設定する関数 */
	void SetUFOMoveState()
	{
		m_UFOState = EnUFOState_Move;
	}

	/** 牛を連れて行けるかどうかのフラグを設定する関数 */
	void SetIsCowTakeAwayed(bool isCowTakeAwayed)
	{
		m_isCowTakeAwayed = isCowTakeAwayed;
	}

	/** 捕獲中かどうかをマネージャーに通知するためのフラグ取得関数*/
	bool GetIsCowTakeAwayed()const
	{
		return m_isCowTakeAwayed;
	}

	/** UFOの混乱エフェクトを再生する関数 */
	void PlayEffect();

	/** UFOの牛捕獲コントローラーの取得関数 */
	CowCaptureController* GetCowCaptureController();

	/** UFOの配列のインデックスを設定する関数 */
	void SetSlotIndex(int index)
	{
		m_slotIndex = index;
	}
	/** UFOの配列のインデックスを取得する関数 */
	int GetSlotIndex() const
	{
		return m_slotIndex;
	}
	/** 目標にしている牛を消す関数 */
	void ReMoveTargetCow()
	{
		m_targetCow = nullptr;
	}
	/** 光が出ているかどうかを取得する関数*/
	bool IsLightEmitting()const
	{
		return m_cowCaptureController.GetIsEmitting();
	}


	bool IsPendingKill()const
	{
		return m_isPendingkill;
	}
	/**  チュートリアル専用：UFOLightManagerの周期を無視して即座に光を出す関数*/
	void ForceEmitLightForTutorial();


private:
	/* 牛を連れていく関数 */
	void TakeAwayTheCow();

	/** 牛を見つける関数 */
	void FindTheCow();

	/** UFO同士の反発関数 */
	void ApplyUFOAvoidance(Vector3& pos);

	/** UFOが制限区域を出ないようにする関数 */
	void ClampToArea(Vector3& pos);

	/** UFOが牛を引っ張る際のSEの距離計算関数 */
	void UFOSEDistance();

	/** UFOのアップデートができるかどうかを判断する関数 */
	bool CanUFOUpdate();
  
	/** UFOのサウンドを更新する関数 */
	void UpdateUFOSound();

	/** 混乱後の上昇処理関数 */
	void ConfusedAscent();
	
	/** 降下処理*/
	void UpdateSpawning();

private:
	ModelRender m_ufomodelRender;
	
	Vector3 m_moveDir = Vector3::Zero;//移動方向
	
	float m_moveSpeed = 62.5f;//移動速度

	float m_moveTimer = 0.0f;//移動タイマー
	
	CountDown* m_countdown = nullptr;

	/** UFOに見つかった牛 */
	Cow* m_targetCow = nullptr;

	/** ゲームシーン */
	Game* m_game = nullptr;

	/*スコア*/
	Score* m_score = nullptr;
	/** タイマー*/
	Timer* m_timer = nullptr;
	/** UFOの牛捕獲コントローラー */
	CowCaptureController m_cowCaptureController;
	
	/** ポーズ */
	Pause* m_pause = nullptr;
	/** 減少用のタイマー*/
	DecreaseTimerUI* m_decreaseTimer = nullptr;
	/** UFOが牛を吸い込むSE */
	SoundSource* m_UFOCaptureSE = nullptr;
	/** プレイヤーの残機UI*/
	CowLivesUI* m_cowLivesUI = nullptr;

	/** 牛が鳴いたかどうか */
	bool m_hasCryed = false;

	/** 牛を連れて行けるかどうかのフラグ */
	bool m_isCowTakeAwayed = false;

	/** 牛を追いかけるフラグ */
	bool m_isChasing = false;
	/** UFOを一度だけ設定するフラグ */
	bool m_isSetUFO = false;
	/**安全に消すためのフラグ */
	bool m_isPendingkill = false;
	/** 一度だけ回転ステートにするフラグ*/
	bool m_hasRotatatedTargetCow = false;
	/** 出現演出*/
	bool m_isSpawning = false;
	float m_spawnTimer = 0.0f;
	/** 下降演出の秒数*/
	static constexpr float SPAWN__DURATION= 3.0f;
	/** 演出開始位置(画面上方)*/
	Vector3 m_spawnStartPos = Vector3::Zero;
	/** 演出終了位置(画面中央)*/
	Vector3 m_spawnEndPos = Vector3::Zero;

	enum EnUFOState
	{
		EnUFOState_Spawning,
		EnUFOState_Move,
		EnUFOState_Idle,
		EnUFOState_Num
	};
	/** UFOが牛の吸い込みに失敗した回数をカウントする変数*/
	uint8_t m_failCount = 0;
	/** 上昇中のフラグ*/
	bool m_isConfusedAscemding = false;
	/** 上昇タイマー*/
	float m_ascentTimer = 0.0f;
	/** 上昇速度*/
	static constexpr float ASCENT_SPEED = 2.0f;
	/** 上昇フレーム*/
	static constexpr float ASCENT_TIME = 120.0f;
	/**UFOの配列のインデックス */ 
	int  m_slotIndex = -1;
	/** スポーン位置*/
	Vector3 m_spawnPos = Vector3::Zero;

	/** UFOの状態 */
	EnUFOState m_UFOState = EnUFOState_Spawning;

	EffectEmitter* m_UFOConfusionEffect = nullptr;
};

