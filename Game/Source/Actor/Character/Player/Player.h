#pragma once
#include"Source/Actor/Character/Character.h"
/*
 * Playerクラス
 */
class Rope;
class CountDown;
class Game;
class Pause;
class CowFood;
class VoiceManager;
class CowLivesUI;
class Player : public Character
{
public:
	Player();
	~Player();

public:
    bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
public:

	/** 移動 */
	void Move();

	/** 回転 */
	void Rotation();

	/** ステート管理 */
	void ManageState();

	/** アニメーション */
	void PlayAnimation();

	/** プレイヤーのポジション取得関数 */
	Vector3 GetPosition()
	{
		return m_transform.GetPosition();
	};

	/** プレイヤーの位置を外部から設定する関数(チュートリアル設定用)*/
	void SetPosition(const Vector3& pos)
	{
		m_transform.SetPosition(pos);
		m_characterController.SetPosition(pos);
		m_playerModelRender.SetPosition(pos);
	}


	/** プレイヤーの回転取得関数 */
	Quaternion GetRotation()
	{
		return m_transform.GetRotation();
	};

	/** プレイヤーの回転を外部から設定する関数(チュートリアル設定用) */
	void SetRotation(const Quaternion& rot)
	{
		m_transform.SetRotation(rot);
		m_playerModelRender.SetRotation(rot);
	}

	/** 右ボタンが押されているかどうかのフラグを設定する関数 */
	void SetGetRightButton1(bool isRightButton1)
	{
		m_isRightButton1 = isRightButton1;
	}

	/** 左ボタンが押されているかどうかのフラグを設定する関数 */
	void SetGetLeftButton1(bool isLeftButton1)
	{
		m_isLeftButton1 = isLeftButton1;
	}

	/** 右ボタンが押されているかどうかのフラグを取得する関数 */
	bool GetIsRightButton1()const
	{
		return m_isRightButton1;
	}

	/** 左ボタンが押されているかどうかのフラグを取得する関数 */
	bool GetIsLeftButton1()const
	{
		return m_isLeftButton1;
	}
	
	/** プレイヤーが動いているかどうかのフラグを取得する関数 */
	bool GetIsMoving()const
	{
		return m_isMoving;
	}

	/** 屈むアニメーションが再生できるかどうかのフラグを取得する関数 */
	bool GetIsSquatAnimation() const
	{
		return m_isSquatAnimation;
	}


private:
	/** ロープを投げる関数*/
	void ThrowRope();

	/** ループを引っ張る関数*/
	void PullRope();

	/** 走るSEの管理関数 */
	void PlayRunSE();

	/** アップデートできるかどうかを判断する関数 */
	bool CanPlayerUpdate();

	void SquatAnimation();

private:
	/** プレイヤーモデル */
	ModelRender m_playerModelRender;

	/** ロープを引っ張るまでのクールタイム */
	CharacterController m_characterController;

	/** プレイヤーの移動速度 */
	Vector3 m_moveSpeed = Vector3::Zero;

	/** ロープを引っ張る操作の画像 */
	SpriteRender m_pullRopeSprite;

	/** ロープを引っ張る操作の画像(LBを押している時) */
	SpriteRender m_pullRopeSpriteLB;

	/** ロープを引っ張る操作の画像(RBを押している時) */
	SpriteRender m_pullRopeSpriteRB;

	/** ロープ */
	Rope* m_rope = nullptr;

	/** ゲームクラス */
	Game* m_game = nullptr;

	/** カウントダウン */
	CountDown* m_countDown = nullptr;

	/** ポーズ */
	Pause* m_pause = nullptr;

	/** ロープを投げる音 */
	SoundSource* m_throwRopeSE = nullptr;

	/** ロープを引っ張る音 */
	SoundSource* m_pullRopeSE = nullptr;

	/** プレイヤーが走る音 */
	SoundSource* m_runSE = nullptr;

	/** 牛の餌 */
	CowFood* m_CowFood = nullptr;

	/** 汗のエフェクト */
	EffectEmitter* m_sweatEffect = nullptr;

	/** ボイスマネージャー */
	VoiceManager* m_voiceManager = nullptr;
	/** プレイヤーの残機*/
	CowLivesUI* m_cowLivesUI = nullptr;
	/** プレイヤーの状態 */
	uint8_t m_playerState = 0;

	/** 前のプレイヤーの状態 */
	int m_prevPlayerState = -1;

	/** ロープを投げるまでのクールタイム */
	float m_throwRopeCoolTime = 0.0f;

	/** 右ボタンが押されているかどうかのフラグ */
	bool m_isRightButton1 = false;

	/** 左ボタンが押されているかどうかのフラグ */
	bool m_isLeftButton1 = false;

	/** 左ボタンが押されているかどうかのフラグ(UI用) */
	bool m_isLeftButton1_Trigger = false;

	/** 右ボタンが押されているかどうかのフラグ(UI用) */
	bool m_isRightButton1_Trigger = false;

	/** プレイヤーが動いているかどうかのフラグ */
	bool m_isMoving = false;

	/** 縄を引っ張っているアニメーションが交互に呼び出されているか確認するフラグ */
	bool m_isPullAnimation = false;

	/** 走るSEが再生中かどうか */
	bool m_isPlayRunSE = false;

	/** 屈むアニメーションが再生できるかどうかのフラグ */
	bool m_isSquatAnimation = false;

	/** 前のフレームでRB2が押されていたかどうか */
	bool m_wasRB2Pressed = false;

	/** アニメーション */
	enum EnPlayAnimation
	{
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_PullLeft,
		enAnimationClip_PullRight,
		enAnimationClip_Squat,
		enAnimationClip_RotateRope,
		enAimationClip_ThrowRope,
		enAnimationClip_Num,
	};

	AnimationClip animationClips[enAnimationClip_Num];
};

