#include "stdafx.h"
#include "Player.h"
#include "Rope/Rope.h"
#include "GameScene/Game.h"
#include "Transform/Transform.h"
#include "CountDown/CountDown.h"
#include "Pause/Pause.h"
#include "SoundManager/SoundManager.h"

namespace
{
	/** プレイヤーモデルのファイルパス */
	const char* PLAYER_FILEPATH = "Assets/modelData/CowBoy/CowBooy.tkm";
	
	/** アニメーションのファイルパス */
	/** 待機アニメーション */
	const char* ANIMATION_IDLEFILEPATH = "Assets/modelData/CowBoy/Idle.tka";
	
	/** 走るアニメーション */
	const char* ANIMATION_RUNFILEPATH = "Assets/modelData/CowBoy/Run.tka";

	/** ロープを引っ張るアニメーション(左) */
	const char* ANIMATION_PULLLEFT_FILEPATH = "Assets/modelData/CowBoy/PullLeft.tka";

	/** ロープを引っ張るアニメーション(右) */
	const char* ANIMATION_PULLRIGHT_FILEPATH = "Assets/modelData/CowBoy/PullRight.tka";
	
	/** ロープを引っ張る画像のファイルパス */
	const char* PULLROPEFILEPATH = "Assets/sprite/PullRopeButton/PullRope.dds";

	/** ロープを引っ張る画像のファイルパス(RBを押しているとき) */
	const char* PULLROPEFILEPATHLB = "Assets/sprite/PullRopeButton/PullRopePushLB.dds";

	/** ロープを引っ張る画像のファイルパス(LBを押しているとき) */
	const char* PULLROPEFILEPATHRB = "Assets/sprite/PullRopeButton/PullRopePushRB.dds";

	/**	ロープを引っ張る画像の位置 */
	Vector3 PULLROPE_SPRITE_POS = Vector3(0.0f, -350.0f, 0.0f);

	/** キャラクターコントローラーの幅 */
	constexpr float CHRACTER_CONTROLLER_WIDTH = 25.0f;
	
	/** キャラクターコントローラーの高さ */
	constexpr float CHRACTER_CONTROLLER_HIGHT = 75.0f;
	
	/** プレイヤーの移動できる範囲の半径 */
	constexpr float PLAYER_MOVE_LIMIT_RADIUS = 1450.0f;

	/** ロープを引っ張る画像の幅 */
	constexpr float PULLROPE_SPRITE_WIDTH = 400.0f;

	/** ロープを引っ張る画像の高さ */
	constexpr float PULLROPE_SPRITE_HEIGHT = 250.0f;
}

Player::Player()
{
	//アニメーションクリップの再生
	animationClips[enAnimationClip_Idle].Load(ANIMATION_IDLEFILEPATH);
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);

	animationClips[enAnimationClip_Run].Load(ANIMATION_RUNFILEPATH);
	animationClips[enAnimationClip_Run].SetLoopFlag(true);

	animationClips[enAnimationClip_PullLeft].Load(ANIMATION_PULLLEFT_FILEPATH);
	animationClips[enAnimationClip_PullLeft].SetLoopFlag(true);

	animationClips[enAnimationClip_PullRight].Load(ANIMATION_PULLRIGHT_FILEPATH);
	animationClips[enAnimationClip_PullRight].SetLoopFlag(true);

	m_playerModelRender.Init(PLAYER_FILEPATH, animationClips, enAnimationClip_Num, enModelUpAxisZ);

}

Player::~Player()
{
	DeleteGO(m_runSE);
	m_runSE = nullptr;

	DeleteGO(m_rope);
}

bool Player::Start()
{
	m_rope = NewGO<Rope>(0, "rope");

    m_characterController.Init(CHRACTER_CONTROLLER_WIDTH, CHRACTER_CONTROLLER_HIGHT, m_transform.GetPosition());
	m_playerModelRender.SetPosition(m_transform.GetPosition());
	m_playerModelRender.Update();

	/** ロープを引っ張る画像の初期化 */
	m_pullRopeSprite.Init(PULLROPEFILEPATH,PULLROPE_SPRITE_WIDTH,PULLROPE_SPRITE_HEIGHT);
	m_pullRopeSprite.SetPosition(PULLROPE_SPRITE_POS);
	m_pullRopeSprite.Update();

	/** ロープを引っ張る画像の初期化(LBを押しているとき) */
	m_pullRopeSpriteLB.Init(PULLROPEFILEPATHLB,PULLROPE_SPRITE_WIDTH,PULLROPE_SPRITE_HEIGHT);
	m_pullRopeSpriteLB.SetPosition(PULLROPE_SPRITE_POS);
	m_pullRopeSpriteLB.Update();

	/** ロープを引っ張る画像の初期化(RBを押しているとき) */
	m_pullRopeSpriteRB.Init(PULLROPEFILEPATHRB,PULLROPE_SPRITE_WIDTH,PULLROPE_SPRITE_HEIGHT);
	m_pullRopeSpriteRB.SetPosition(PULLROPE_SPRITE_POS);
	m_pullRopeSpriteRB.Update();

	/** 初期化時にステートを待機状態にする */
	m_playerState = 0;
	m_playerModelRender.Update();

	return true;
}

void Player::Update()
{
	/*アニメーション*/
	PlayAnimation();

	/*アップデートできるかどうかを判断する関数*/
	if (!CanPlayerUpdate())
	{
		return;
	}

	/*ロープを引っ張る処理*/
	PullRope();
	/*移動*/
	Move();
	/*回転*/
	Rotation();
	/*ロープを投げる処理*/
	ThrowRope();
	/*ステート管理*/
	ManageState();

	/*プレイヤーが動いているかどうかのフラグを設定する*/
	m_isMoving = (m_moveSpeed.LengthSq() >= 0.0001f);

	/** 走るSEの処理 */
	PlayRunSE();

	m_playerModelRender.Update();
}

void Player::Move()
{

	if (m_rope->GetIsThrowRope() or m_rope->GetIsHitCow())
	{
		m_moveSpeed = Vector3::Zero;
		//ロープを投げているときとロープが牛に当たっているときは移動できないようにする
		return;
	}

	//左スティックの入力量を取得
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.z = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right   = g_camera3D->GetRight();

	//正規化
	right.y = 0.0f;
	forward.y = 0.0f;
	forward.Normalize();
	right.Normalize();

	//入力量を反映
	Vector3 moveDir = forward * stickL.z + right * stickL.x;
	moveDir *= 150.0f;
	m_moveSpeed = moveDir;

	//左スティックの入力量と120.0fを乗算
	right*= stickL.x * 150.0f;
	forward *= stickL.z * 150.0f;

	//移動速度にカメラの前方向と右方向を加算
	m_moveSpeed += right + forward;
	//キャラクターコントローラーを使って座標を移動させる
	m_transform.GetPosition() = m_characterController.Execute(m_moveSpeed,g_gameTime->GetFrameDeltaTime());

	Vector3 position = m_transform.GetPosition();

	/** XZ平面の距離計算 */
	Vector3 posXZ = position;
	posXZ.y = 0.0f;
	float distsance = posXZ.Length();

	//モデルの座標をキャラクターコントローラーの座標に合わせる
	m_playerModelRender.SetPosition(m_transform.GetPosition());
}

void Player::Rotation()
{
	if (m_rope->GetIsThrowRope())
	{
		//ロープを投げているときは回転できないようにする
		return;
	}
	if (fabsf(m_moveSpeed.x) >= 0.0001f || fabsf(m_moveSpeed.z) >= 0.0001f)
	{
		//キャラクターの方向を変える
		m_transform.GetRotation().SetRotationYFromDirectionXZ(m_moveSpeed);

		//モデルの回転をキャラクターの回転に合わせる
		m_playerModelRender.SetRotation(m_transform.GetRotation());
	}
}

void Player::PlayRunSE()
{
	/** 牛を引っ張っている時はSEを止める */
	if (m_rope->GetIsHitCow())
	{
		if (m_isPlayRunSE && m_runSE != nullptr)
		{
			m_runSE->Stop();
			m_isPlayRunSE = false;
		}

		return;
	}

	/** 移動中なら */
	if (m_isMoving)
	{
		/** まだ再生していないなら再生させる。 */
		if (!m_isPlayRunSE)
		{
			SoundManager* soundManager = FindGO<SoundManager>("soundmanager");

			/** trueでSEをループ再生させる。 */
			m_runSE = soundManager->PlayingSE(SoundSE::enRunSE, true);

			m_isPlayRunSE = true;
		}
	}
	else
	{
		/** 走ってるSEが再生中かつSEオブジェクトが存在しているかどうか */
		if (m_isPlayRunSE && m_runSE != nullptr)
		{
			/** 止まったらSE停止 */
			m_runSE->Stop();

			m_isPlayRunSE = false;
		}
	}
}

void Player::ThrowRope()
{
	if (m_throwRopeCoolTime > 0.0f) {
		m_throwRopeCoolTime -= g_gameTime->GetFrameDeltaTime();

		if(m_throwRopeCoolTime < 0.0f) {
			m_throwRopeCoolTime = 0.0f;
		}
	}

	if (m_rope->GetIsHitCow() or !m_rope)
	{
		//ロープが存在しない、ロープが牛に当たっているときはロープを投げられないようにする
		return;
	}

	/** クールダウン中はロープを投げられない */
	if (m_throwRopeCoolTime > 0.0f) return;

	/** RB2ボタンが押されていて、ロープを投げていないとき */
	if (g_pad[0]->IsTrigger(enButtonRB2) && !m_rope->GetIsThrowRope())
	{
		/** ロープを投げる */
		m_rope->SetIsThrowRope(true);

		/** クールダウン開始 */
		m_throwRopeCoolTime = 1.5f;

		/** ロープを投げる音を再生 */
		SoundManager* soundManager = FindGO<SoundManager>("soundmanager");
		m_throwRopeSE = soundManager->PlayingSE(SoundSE::enThrowRopeSE, false);
	}
}

void Player::PullRope()
{

	if (m_rope)
	{
		//ロープが牛に当たっているとき
		if (m_rope->GetIsHitCow())
		{
			if (g_pad[0]->IsTrigger(enButtonRB1))
			{
				m_isRightButton1 = true;
				m_isLeftButton1 = false;

				m_isRightButton1_Trigger = true;
				m_isLeftButton1_Trigger = false;
			}

			else if (g_pad[0]->IsTrigger(enButtonLB1))
			{
				m_isLeftButton1 = true;
				m_isRightButton1 = false;

				m_isLeftButton1_Trigger = true;
				m_isRightButton1_Trigger = false;
			}

			if (g_pad[0]->IsTrigger(enButtonRB1) || g_pad[0]->IsTrigger(enButtonLB1))
			{
				if (m_isPullAnimation)
				{
					m_playerState = 2;
				}
				else
				{
					m_playerState = 3;
				}

				m_isPullAnimation = !m_isPullAnimation;

				if (m_isPullAnimation || !m_isPullAnimation)
				{
					SoundManager* soundManager = FindGO<SoundManager>("soundmanager");
					m_pullRopeSE = soundManager->PlayingSE(SoundSE::enRopePullSE, false);
				}
				
			}
		}
	}
}

bool Player::CanPlayerUpdate()
{
	m_game = FindGO<Game>("game");
	m_countDown = FindGO<CountDown>("countdown");
	m_pause = FindGO<Pause>("pause");

	if (m_pause == nullptr ||
		m_countDown == nullptr ||
		m_game == nullptr ||
		m_game->GetIsTimeOut() ||
		m_pause->GetIsPause() ||
		m_countDown->GetCountDown())
	{
		/** 走ってるSEが再生中かつSEオブジェクトが存在しているかどうか */
		if (m_isPlayRunSE && m_runSE != nullptr)
		{
			/** 止まったらSE停止 */
			m_runSE->Stop();

			m_isPlayRunSE = false;
		}

		m_isMoving = false;
		return false;
	}

	return true;
}

void Player::ManageState()
{
	/** 牛を引っ張っている最中は再生しないようにする。 */
	if (m_rope->GetIsHitCow())
	{
		return;
	}
	if (fabsf(m_moveSpeed.x) >= 0.01f || fabsf(m_moveSpeed.z) >= 0.01f)
	{
		/** 移動しているときは走るアニメーションにする */
		m_playerState = 1;
	}
	else
	{
		/** 移動していないときは待機アニメーションにする */
		m_playerState = 0;
	}

}

void Player::PlayAnimation()
{
	/** 前の状態と同じ状態だったらアニメーションを切り替えない */
	if (m_playerState == m_prevPlayerState)
	{
		return;
	}
	m_prevPlayerState = m_playerState;
	switch (m_playerState)
	{
	case 0:
		/** 待機アニメーション */
		m_playerModelRender.PlayAnimation(enAnimationClip_Idle,0.15f);

		m_playerModelRender.Update();
		break;
	case 1:
		//走るアニメーション
		/** 走るアニメーション */
		m_playerModelRender.PlayAnimation(enAnimationClip_Run,0.15f);

		m_playerModelRender.Update();
		break;
	case 2:
		/** 縄を引っ張るアニメーション(左) */
		m_playerModelRender.PlayAnimation(enAnimationClip_PullLeft,0.15f);

		m_playerModelRender.Update();
		break;
	case 3:
		/** 縄を引っ張るアニメーション(右) */
		m_playerModelRender.PlayAnimation(enAnimationClip_PullRight,0.15f);

		m_playerModelRender.Update();
		break;

	}
}

void Player::Render(RenderContext& rc)
{
		m_playerModelRender.Draw(rc);

	/** 両方のボタンを押していなかったら */
	if (m_rope->GetIsHitCow() && !m_isLeftButton1_Trigger && !m_isRightButton1_Trigger)
	{
		/** ロープを引っ張る画像の描画 */
		m_pullRopeSprite.Draw(rc);		
	}

	/** 右ボタンを押しているとき */
	if(m_rope->GetIsHitCow() && m_isLeftButton1_Trigger)
	{
		/** ロープを引っ張る画像の描画(LBを押しているとき) */
		m_pullRopeSpriteLB.Draw(rc);
	}

	/** 左ボタンを押しているとき */
	if (m_rope->GetIsHitCow() && m_isRightButton1_Trigger)
	{
		/** ロープを引っ張る画像の描画(RBを押しているとき) */
		m_pullRopeSpriteRB.Draw(rc);
	}
}