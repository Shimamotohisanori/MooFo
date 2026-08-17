#include "stdafx.h"
#include "Player.h"
#include "Rope/Rope.h"
#include "GameScene/Game.h"
#include "Transform/Transform.h"
#include "CountDown/CountDown.h"
#include "Pause/Pause.h"
#include "SoundManager/SoundManager.h"
#include "Source/Actor/Stage/CowFood.h"
#include "EffectManager/EffectManager.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "SoundManager/VoiceManager.h"
#include"CowLivesUI/CowLivesUI.h"
namespace
{
	/** プレイヤーモデルのファイルパス */
	const char* PLAYER_FILEPATH = "Assets/modelData/CowBoy/CowBooy.tkm";

	/** アニメーションのファイルパス */
	/** 待機アニメーション */
	const char* ANIMATION_IDLEFILEPATH = "Assets/animData/CowBoyAnimation/Idle.tka";

	/** 走るアニメーション */
	const char* ANIMATION_RUNFILEPATH = "Assets/animData/CowBoyAnimation/Run.tka";

	/** ロープを引っ張るアニメーション(左) */
	const char* ANIMATION_PULLLEFT_FILEPATH = "Assets/animData/CowBoyAnimation/PullLeft.tka";

	/** ロープを引っ張るアニメーション(右) */
	const char* ANIMATION_PULLRIGHT_FILEPATH = "Assets/animData/CowBoyAnimation/PullRight.tka";

	/** プレイヤーが屈むアニメーション */
	const char* ANIMATION_SQUAT_FILEPATH = "Assets/animData/CowBoyAnimation/Squat.tka";

	/** プレイヤーがロープを回すアニメーション */
	const char* ANIMATION_ROTATEROPE_FILEPATH = "Assets/animData/CowBoyAnimation/RotateRope.tka";

	/** プレイヤーがロープを投げるアニメーション */
	const char* ANIMATION_THROWROPE_FILEPATH = "Assets/animData/CowBoyAnimation/ThrowRope.tka";

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

	const Vector3 EFFECT_SCALE = {5.0f,5.0f,5.0f};
}

Player::Player()
{
	/** 待機アニメーションクリップを読み込み、ループ再生に設定する */
	animationClips[enAnimationClip_Idle].Load(ANIMATION_IDLEFILEPATH);
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);

	/** 走るアニメーションクリップを読み込み、ループ再生に設定する */
	animationClips[enAnimationClip_Run].Load(ANIMATION_RUNFILEPATH);
	animationClips[enAnimationClip_Run].SetLoopFlag(true);

	/** ロープを引っ張る(左)アニメーションクリップを読み込み、ループ再生に設定する */
	animationClips[enAnimationClip_PullLeft].Load(ANIMATION_PULLLEFT_FILEPATH);
	animationClips[enAnimationClip_PullLeft].SetLoopFlag(true);

	/** ロープを引っ張る(右)アニメーションクリップを読み込み、ループ再生に設定する */
	animationClips[enAnimationClip_PullRight].Load(ANIMATION_PULLRIGHT_FILEPATH);
	animationClips[enAnimationClip_PullRight].SetLoopFlag(true);

	/** 屈むアニメーションクリップを読み込み、ループしない(1回だけ再生)に設定する */
	animationClips[enAnimationClip_Squat].Load(ANIMATION_SQUAT_FILEPATH);
	animationClips[enAnimationClip_Squat].SetLoopFlag(false);

	/** ロープを投げるアニメーションクリップを読み込み、ループ再生に設定する */
	animationClips[enAnimationClip_RotateRope].Load(ANIMATION_ROTATEROPE_FILEPATH);
	animationClips[enAnimationClip_RotateRope].SetLoopFlag(true);

	/** ロープを投げるアニメーションクリップを読み込み、ループしない(1回だけ再生)に設定する */
	animationClips[enAimationClip_ThrowRope].Load(ANIMATION_THROWROPE_FILEPATH);
	animationClips[enAimationClip_ThrowRope].SetLoopFlag(false);

	/** プレイヤーモデルを初期化する。Z軸を上方向としてモデルを読み込む */
	m_playerModelRender.Init(PLAYER_FILEPATH, animationClips, enAnimationClip_Num, enModelUpAxisZ);
}

Player::~Player()
{
	/** 走るSEオブジェクトを削除してnullptrにする */
	DeleteGO(m_runSE);
	m_runSE = nullptr;

	/** ロープオブジェクトを削除する */
	DeleteGO(m_rope);

	/** ロープを回すSEオブジェクトを削除してnullptrにする */
	DeleteGO(m_rotationRopeSE);
	m_rotationRopeSE = nullptr;
}

bool Player::Start()
{
	/** ロープオブジェクトを生成する */
	m_rope = NewGO<Rope>(0, "rope");

	/** キャラクターコントローラーを初期化する。幅・高さ・初期座標を渡す */
	m_characterController.Init(CHRACTER_CONTROLLER_WIDTH, CHRACTER_CONTROLLER_HIGHT, m_transform.GetPosition());

	/** モデルの座標を初期座標に合わせて更新する */
	m_playerModelRender.SetPosition(m_transform.GetPosition());
	m_playerModelRender.Update();

	/** ロープを引っ張る画像の初期化 */
	m_pullRopeSprite.Init(PULLROPEFILEPATH, PULLROPE_SPRITE_WIDTH, PULLROPE_SPRITE_HEIGHT);
	m_pullRopeSprite.SetPosition(PULLROPE_SPRITE_POS);
	m_pullRopeSprite.Update();

	/** ロープを引っ張る画像の初期化(LBを押しているとき) */
	m_pullRopeSpriteLB.Init(PULLROPEFILEPATHLB, PULLROPE_SPRITE_WIDTH, PULLROPE_SPRITE_HEIGHT);
	m_pullRopeSpriteLB.SetPosition(PULLROPE_SPRITE_POS);
	m_pullRopeSpriteLB.Update();

	/** ロープを引っ張る画像の初期化(RBを押しているとき) */
	m_pullRopeSpriteRB.Init(PULLROPEFILEPATHRB, PULLROPE_SPRITE_WIDTH, PULLROPE_SPRITE_HEIGHT);
	m_pullRopeSpriteRB.SetPosition(PULLROPE_SPRITE_POS);
	m_pullRopeSpriteRB.Update();

	/** 初期化時にステートを待機状態にする */
	m_playerState = 0;
	m_playerModelRender.Update();

	/** ボイスマネージャーを取得 */
	m_voiceManager = FindGO<VoiceManager>("voicemanager");

	return true;
}

void Player::Update()
{
	/** CowFoodオブジェクトがまだ取得できていない場合、毎フレーム検索して取得を試みる */
	if (m_CowFood == nullptr)
	{
		m_CowFood = FindGO<CowFood>("cowfood");
	}
	
	/** 「FAILED　RESCUE」のテキストが出ていたらこの下の処理を止める*/
	m_cowLivesUI = FindGO<CowLivesUI>("cowlivesui");
	if (m_cowLivesUI && m_cowLivesUI->IsNotOperetion())
	{
		/** 走っているSEが再生中なら*/
		if (m_isPlayRunSE && m_runSE != nullptr)
		{
			m_runSE->Stop();
			m_isPlayRunSE = false;
		}
		/** 移動速度と移動中フラグをリセットする*/
		m_moveSpeed = Vector3::Zero;
		m_isMoving = false;

		/** アニメーションを待機状態に戻して固まったままにしない*/
		m_playerState = 0;
		PlayAnimation();
		m_playerModelRender.Update();
		
		return;
	}

	/*アップデートできるかどうかを判断する関数 */
	if (!CanPlayerUpdate())
	{
		return;
	}



	/*ロープを引っ張る処理 */
	PullRope();
	/*移動*/
	Move();
	/*回転*/
	Rotation();
	/*ロープを投げる処理 */
	ThrowRope();
	/*ステート管理 */
	ManageState();

	/** 屈むアニメーション */
	SquatAnimation();

	/* プレイヤーが動いているかどうかのフラグを設定する */
	m_isMoving = (m_moveSpeed.LengthSq() >= 0.0001f);

	/** 走るSEの処理 */
	PlayRunSE();

	/* アニメーション */
	PlayAnimation();

	m_playerModelRender.Update();
}

void Player::Move()
{
	/* ロープを投げているとき
	 * ロープが牛に当たっているとき
	 * 屈むアニメーション中
	 * ロープのアニメーションが終了しているとき
	 * RB2ボタンが押されているときは移動できないようにする */
	if (m_rope->GetIsThrowRope() ||
		m_rope->GetIsHitCow() ||
		m_isSquatAnimation ||
		m_rope->GetIsEndRopeAnimation() ||
		g_pad[0]->IsPress(enButtonRB2))
	{
		m_moveSpeed = Vector3::Zero;
		/*ロープを投げているときとロープが牛に当たっているときは移動できないようにする */
		return;
	}

	/* 左スティックの入力量を取得 */
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.z = g_pad[0]->GetLStickYF();

	/* カメラの前方向と右方向のベクトルを持ってくる */
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	/* 正規化 */
	right.y = 0.0f;
	forward.y = 0.0f;
	forward.Normalize();
	right.Normalize();

	/* 入力量を反映 */
	Vector3 moveDir = forward * stickL.z + right * stickL.x;
	moveDir *= 150.0f;
	m_moveSpeed = moveDir;

	/* 左スティックの入力量と150.0fを乗算 */
	right *= stickL.x * 150.0f;
	forward *= stickL.z * 150.0f;

	/* 移動速度にカメラの前方向と右方向を加算 */
	m_moveSpeed += right + forward;

	/* キャラクターコントローラーを使って座標を移動させる */
	m_transform.GetPosition() = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	Vector3 position = m_transform.GetPosition();

	/** XZ平面の距離計算(現在は使用していないが、移動範囲制限などに使える) */
	Vector3 posXZ = position;
	posXZ.y = 0.0f;
	float distsance = posXZ.Length();

	//モデルの座標をキャラクターコントローラーの座標に合わせる
	m_playerModelRender.SetPosition(m_transform.GetPosition());
}

void Player::Rotation()
{

	/** 牛を引っ張っているときは牛の方向を向く */
	if (m_rope->GetIsHitCow())
	{
		/** ロープが当たっている牛を取得する */
		Cow* hitcow = m_rope->GetHitCow();

		if (hitcow)
		{
			/** プレイヤーから牛への方向ベクトルを計算する */
			Vector3 dir = hitcow->GetPosition() - m_transform.GetPosition();

			/** 上下方向の回転は無視する */
			dir.y = 0.0f;

			/** ベクトルの長さが十分ある場合のみ処理を行う */
			if(dir.LengthSq() >= 0.0001f)
			{
				/** 方向ベクトルを正規化する */
				dir.Normalize();

				/** ロープが当たっている牛の方向を向くようにプレイヤーの回転を設定する */
				m_transform.GetRotation().SetRotationYFromDirectionXZ(dir);

				/** 設定した回転をモデルに反映させる */
				m_playerModelRender.SetRotation(m_transform.GetRotation());
			}
		}
		return;
	}

	/** 移動速度のXまたはZ成分がある程度あるときだけ回転処理を行う(停止中は回転させない) */
	if (fabsf(m_moveSpeed.x) >= 0.0001f || fabsf(m_moveSpeed.z) >= 0.0001f)
	{
		/* キャラクターの方向を変える */
		m_transform.GetRotation().SetRotationYFromDirectionXZ(m_moveSpeed);

		/* モデルの回転をキャラクターの回転に合わせる */
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
	/** クールタイムが残っている場合、デルタタイムで減算していく */
	if (m_throwRopeCoolTime > 0.0f) {
		m_throwRopeCoolTime -= g_gameTime->GetFrameDeltaTime();

		/** クールタイムが0未満にならないようにクランプする */
		if (m_throwRopeCoolTime < 0.0f) {
			m_throwRopeCoolTime = 0.0f;
		}
	}

	/** ロープが存在しない、またはロープが牛に当たっているときはロープを投げられないようにする */
	if (!m_rope || m_rope->GetIsHitCow())
	{
		/** ロープがあたっている間は押下状態の記録だけを更新する */
		m_wasRB2Pressed = g_pad[0]->IsPress(enButtonRB2);

		/** ロープを回すSEが再生中なら止める */
		if(m_isPlayRotationRopeSE && m_rotationRopeSE)
		{
			m_rotationRopeSE->Stop();
			m_isPlayRotationRopeSE = false;
		}

		return;
	}

	/** 現在のRB2の押下状態を取得 */
	bool isrb2pressed = g_pad[0]->IsPress(enButtonRB2);

	/** 前フレーム押していて、今は離れているフラグ */
	bool isreleased = m_wasRB2Pressed && !isrb2pressed;

	/** 現在のRB2の押下状態を記録しておく */
	m_wasRB2Pressed = isrb2pressed;

	/** 現在のRB2の押下状態を記録しておく */
	m_wasRB2Pressed = isrb2pressed;

	/** ロープを回す音の再生・停止処理
	 * 縮みアニメーション中やロープアニメ終了直後は鳴らさない */
	if (isrb2pressed &&
		!m_isSquatAnimation &&
		!m_rope->GetIsThrowRope() &&
		!m_rope->GetIsEndRopeAnimation())
	{
		/** まだ再生していないなら再生する */
		if (!m_isPlayRotationRopeSE)
		{
			SoundManager* soundManager = FindGO<SoundManager>("soundmanager");

			/** trueでループ再生させる */
			m_rotationRopeSE = soundManager->PlayingSE(SoundSE::enRotationRopeSE, true);

			m_isPlayRotationRopeSE = true;
		}
	}
	else
	{
		/** ボタンを離した、または鳴らせない状態になったら停止する */
		if (m_isPlayRotationRopeSE && m_rotationRopeSE != nullptr)
		{
			m_rotationRopeSE->Stop();
			m_isPlayRotationRopeSE = false;
		}
	}

	/** クールダウン中はロープを投げられない */
	if (m_throwRopeCoolTime > 0.0f) return;

	/** RB2ボタンが離されていて、ロープを投げていない
	 * かつ縮みアニメーションが終了しているとき */
	if (isreleased &&
		!m_rope->GetIsThrowRope() &&
		!m_rope->GetIsEndRopeAnimation() &&
		!m_isSquatAnimation)
	{
		/** 投げる瞬間、カメラの前方向(水平のみ)にプレイヤーを向かせる */
		Vector3 camForward = g_camera3D->GetForward();
		camForward.y = 0.0f;

		if (camForward.LengthSq() > 0.0001f)
		{
			camForward.Normalize();
			m_transform.GetRotation().SetRotationYFromDirectionXZ(camForward);
			m_playerModelRender.SetRotation(m_transform.GetRotation());
		}

		/** ロープを投げる */
		m_rope->SetIsThrowRope(true);

		/** クールダウン開始 */
		m_throwRopeCoolTime = 2.0f;

		/** ロープを投げる音を再生 */
		SoundManager* soundManager = FindGO<SoundManager>("soundmanager");
		m_throwRopeSE = soundManager->PlayingSE(SoundSE::enThrowRopeSE, false);

		/** ボイスの再生(ランダムで2種類切り替え) */
		SoundVoice voice = (rand() % 2 == 0) ? SoundVoice::enVoice_ThrowRope1 : SoundVoice::enVoice_ThrowRope2;

		/** ボイスを再生 */
		m_voiceManager->PlayingVoice(voice, false);
	}
}

void Player::PullRope()
{
	/** ロープオブジェクトが存在しているか確認する */
	if (m_rope)
	{
		//ロープが牛に当たっているとき
		if (m_rope->GetIsHitCow())
		{
			/** RB1ボタンが押されたとき */
			if (g_pad[0]->IsTrigger(enButtonRB1))
			{
				/** コントローラーをバイブレーションさせる */
				g_pad[0]->SetVibration(0.5f, 1.0f);

				/** 右ボタンフラグをON、左ボタンフラグをOFFにする */
				m_isRightButton1 = true;
				m_isLeftButton1 = false;

				m_isRightButton1_Trigger = true;
				m_isLeftButton1_Trigger = false;
				/** 汗をかくエフェクトを再生させる。 */
				m_sweatEffect = NewGO<nsK2EngineLow::EffectEmitter>(0);
				m_sweatEffect->Init((int)EffectID::EffectID_PlayerSweat);

				Vector3 pos =
				{
					m_transform.GetPosition().x,m_transform.GetPosition().y + 60.0f,m_transform.GetPosition().z
				};

				m_sweatEffect->SetPosition(pos);
				m_sweatEffect->SetScale(EFFECT_SCALE);
				m_sweatEffect->Play();
				m_sweatEffect->Update();
			}
			/** LB1ボタンが押されたとき */
			else if (g_pad[0]->IsTrigger(enButtonLB1))
			{
				/** コントローラーをバイブレーションさせる */
				g_pad[0]->SetVibration(0.5f, 1.0f);

				/** 左ボタンフラグをON、右ボタンフラグをOFFにする */
				m_isLeftButton1 = true;
				m_isRightButton1 = false;

				m_isLeftButton1_Trigger = true;
				m_isRightButton1_Trigger = false;

				/** 汗をかくエフェクトを再生させる。 */
				m_sweatEffect = NewGO<nsK2EngineLow::EffectEmitter>(0);
				m_sweatEffect->Init((int)EffectID::EffectID_PlayerSweat);

				Vector3 pos = 
				{ 
					m_transform.GetPosition().x,m_transform.GetPosition().y + 60.0f,m_transform.GetPosition().z 
				};

				m_sweatEffect->SetPosition(pos);
				m_sweatEffect->SetScale(EFFECT_SCALE);
				m_sweatEffect->Play();
				m_sweatEffect->Update();
			}

			/** RB1かLB1のどちらかが押されたとき、引っ張りアニメーションを交互に切り替える */
			if (g_pad[0]->IsTrigger(enButtonRB1) || g_pad[0]->IsTrigger(enButtonLB1))
			{
				/** フラグに応じて左右の引っ張りアニメーションを交互に切り替える */
				if (m_isPullAnimation)
				{
					m_playerState = 2;
				}
				else
				{
					m_playerState = 3;
				}

				/** 次回呼び出し時に反対のアニメーションが再生されるようにフラグを反転する */
				m_isPullAnimation = !m_isPullAnimation;

				/** ロープを引っ張るSEを再生する(条件式は常にtrueだが、SEを必ず再生させる意図) */
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
	/** 毎フレーム各オブジェクトを検索して取得する */
	m_game = FindGO<Game>("game");
	m_countDown = FindGO<CountDown>("countdown");
	m_pause = FindGO<Pause>("pause");

	/** いずれかのオブジェクトが存在しない、またはゲーム終了・ポーズ・カウントダウン中はUpdateを止める */
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
	/** しゃがみアニメーションが再生されていない場合、しゃがみ中フラグをリセットする */
	if (!m_playerModelRender.IsPlayingAnimation())
	{
		m_isSquatAnimation = false;
		m_playerState = 0;
	}

	/** ロープを投げている最中はステートを6(ロープを投げる)にする */
	if (m_rope->GetIsThrowRope())
	{
		m_playerState = 6;
		return;
	}

	/** 牛を引っ張っている最中は再生しないようにする。 */
	if (m_rope->GetIsHitCow())
	{
		return;
	}

	/** しゃがみアニメーションが再生中は走る/待機への切り替えを防ぐ */
	if (m_isSquatAnimation)
	{
		return;
	}

	/** RB2ボタンが押されている間はステートを5(ロープを回す)にする */
	if(g_pad[0]->IsPress(enButtonRB2))
	{
		m_playerState = 5;
		return;
	}

	/** 移動速度のXまたはZ成分が一定以上あれば走りステートにする */
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

void Player::SquatAnimation()
{
	/** 牛の餌をおいたら */
	if (m_CowFood->GetIsPutFood())
	{
		/** コントローラーをバイブレーションさせる */
		g_pad[0]->SetVibration(0.5f, 1.0f);
	
		/** 屈むアニメーションを再生する */
		m_playerState = 4;
		/** しゃがみ中フラグを立てる */
		m_isSquatAnimation = true;
		/** 連続してトリガーされないようにフラグをリセットする */
		m_CowFood->SetIsPutFood(false);

		/** ボイスの再生 */
		m_voiceManager->PlayingVoice(SoundVoice::enVoice_PutHay, false);
	}
}

void Player::PlayAnimation()
{
	/** 前の状態と同じ状態だったらアニメーションを切り替えない */
	if (m_playerState == m_prevPlayerState)
	{
		return;
	}

	/** 現在のステートを前のステートとして保存する */
	m_prevPlayerState = m_playerState;

	switch (m_playerState)
	{
	case 0:
		/** 待機アニメーション */
		m_playerModelRender.PlayAnimation(enAnimationClip_Idle, 0.15f);
		m_playerModelRender.Update();
		break;
	case 1:
		/** 走るアニメーション */
		m_playerModelRender.PlayAnimation(enAnimationClip_Run, 0.15f);
		m_playerModelRender.SetAnimationSpeed(1.551f);
		m_playerModelRender.Update();
		break;
	case 2:
		/** 縄を引っ張るアニメーション(左) */
		m_playerModelRender.PlayAnimation(enAnimationClip_PullLeft, 0.15f);
		m_playerModelRender.Update();
		break;
	case 3:
		/** 縄を引っ張るアニメーション(右) */
		m_playerModelRender.PlayAnimation(enAnimationClip_PullRight, 0.15f);
		m_playerModelRender.Update();
		break;
	case 4:
		/** 屈むアニメーション(ループなしで1回だけ再生される) */
		m_playerModelRender.PlayAnimation(enAnimationClip_Squat, 0.30f);
		m_playerModelRender.Update();
		break;
	case 5:
		/** ロープを回すアニメーション */
		m_playerModelRender.PlayAnimation(enAnimationClip_RotateRope, 0.15f);
		m_playerModelRender.Update();
		break;
	case 6:
		/** ロープを投げるアニメーション(ループなしで1回だけ再生される) */
		m_playerModelRender.PlayAnimation(enAimationClip_ThrowRope, 0.15f);
		m_playerModelRender.Update();
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	/** プレイヤーモデルを描画する */
	m_playerModelRender.Draw(rc);

	/** 両方のボタンを押していなかったら */
	if (m_rope->GetIsHitCow() && !m_isLeftButton1_Trigger && !m_isRightButton1_Trigger)
	{
		/** ロープを引っ張る画像の描画 */
		m_pullRopeSprite.Draw(rc);
	}

	/** 右ボタンを押しているとき */
	if (m_rope->GetIsHitCow() && m_isLeftButton1_Trigger)
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