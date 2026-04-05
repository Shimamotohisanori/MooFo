#pragma once
#include"Source/Actor/Character/Character.h"
/**
*Playerクラス
*/
class Rope;
class CountDown;
class Game;
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
	void Move();
	void Rotation();
	/*ステート管理*/
	void ManageState();
	/*アニメーション*/
	void PlayAnimation();
	Vector3 GetPosition()
	{
		return m_transform.GetPosition();
	};

	Quaternion GetRotation()
	{
		return m_transform.GetRotation();
	};

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
	bool GetIsRightButton1()
	{
		return m_isRightButton1;
	}

	/** 左ボタンが押されているかどうかのフラグを取得する関数 */
	bool GetIsLeftButton1()
	{
		return m_isLeftButton1;
	}


private:
	/** ロープを投げる関数*/
	void ThrowRope();

	/** ループを引っ張る関数*/
	void PullRope();

private:
	ModelRender m_playerModelRender;

	Vector3 m_moveSpeed;//移動速度

	int m_playerState = 0;//プレイヤーの状態
	int m_prevPlayerState = -1;//プレイヤーの状態

	

	CharacterController m_characterController;//キャラコン

	Rope* m_rope;//ロープクラス

	Game* m_game;//ゲームクラス

	CountDown* m_countDown;

	//右ボタンが押されているかどうかのフラグ
	bool m_isRightButton1 = false;

	//左ボタンが押されているかどうかのフラグ
	bool m_isLeftButton1 = false;

	//アニメーション
	enum EnPlayAnimation
	{
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_Num,
	};
	AnimationClip animationClips[enAnimationClip_Num];
};

