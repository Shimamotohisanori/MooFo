#include "stdafx.h"
#include "Rope.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "GameCamera/GameCamera.h"
#include "Source/Actor/Character/Player/Player.h"
#include "GameTimer/Timer.h"

namespace
{
	/** ロープのモデルファイルパス */
	const char* ROPE_MODEL_FILEPATH = "Assets/modelData/Rope/NewRope.tkm";

	/** 捕まった牛用のロープモデルファイルパス */
	const char* CAPTURED_COW_FILEPATH = "Assets/modelData/Rope/CapturedCowRope.tkm";

	/** ロープが縮むアニメーション */
	const char* ROPE_SHRINK_ANIMATION_FILEPATH = "Assets/animData/Rope/Shrink_ThrowRope.tka";

	/** ロープが伸びるアニメーション */
	const char* ROPE_EXTEND_ANIMATION_FILEPATH = "Assets/animData/Rope/Stretch_ThrowRope.tka";

	/** アニメーションを再生させるロープモデルのファイルパス */
	const char* ROPE_ANIMATION_MODEL_FILEPATH = "Assets/modelData/Rope/animationRope.tkm";

	/** ロープの初期の大きさ */
	const Vector3 ROPE_INITIAL_SCALE = { 1.0f, 1.0f, 5.0f };

	/** 牛に当たっていない時の巻かれたロープの大きさ */
	const Vector3 NO_HIT_COW_ROLL_ROPE_SCALE = { 0.5f, 0.5f, 0.5f };

	/** 牛に当たった時の巻かれたロープの大きさ */
	const Vector3 HIT_COW_ROLL_ROPE_SCALE = { 1.0f, 1.0f, 1.0f };

	/** プレイヤーの右手からロープが出る位置のオフセット */
	constexpr float ROPE_OFFSET_RIGHT = 10.0f;

	/** プレイヤーの前からロープが出る位置のオフセット */
	constexpr float ROPE_OFFSET_FORWARD = 0.5f;

	/** プレイヤーの上からロープが出る位置のオフセット */
	constexpr float ROPE_OFFSET_UP = 30.0f;

	/** 牛の位置の少し上を狙うためのオフセット */
	constexpr float ROPE_AIM_UP_OFFSET = 30.0f;

	/** ロープの伸び縮みのスケールを求めるための距離にかける係数 */
	constexpr float ROPE_SCALE_FACTOR = 0.055f;

	/** ロープの伸び縮みのスケールの最小値 */
	constexpr float ROPE_MIN_SCALE_Z = 0.002f;

	/** ロープが牛に当たったとみなす距離 */
	constexpr float ROPE_HIT_DISTANCE = 50.0f;

	/** ロープのたるみアニメーション継続時間 */
	constexpr float ROPE_SLACK_DURATION = 0.8f;

	/** ロープのたるみアニメーションの時間をスケールする係数 */
	constexpr float DELTA_TIME_SCALE = 5.0f;

	/** UFOが牛を捕獲する高さ */
	constexpr float UFO_CATCH_HEIGHT = 400.0f;

	/** 牛が連れ去られる時のロープの最小スケール */
	constexpr float COW_MIN_SCALE = 0.1f;
}


Rope::Rope()
{
	

}


Rope::~Rope()
{

}


bool Rope::Start()
{
	m_ropeModelRender.Init(ROPE_MODEL_FILEPATH);
	m_rollModelRender.Init(CAPTURED_COW_FILEPATH);

	/** セグメント初期化 */
	for (int i = 0; i < ROPE_SEGMENT_COUNT; ++i)
	{
		m_ropeSegments[i].Init(ROPE_MODEL_FILEPATH);
		/** ロード完了を待つために一度Updateを呼ぶ */
		m_ropeSegments[i].SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		m_ropeSegments[i].SetScale(Vector3(0.0f, 0.0f, 0.0f));
		m_ropeSegments[i].Update();
	}

	m_player = FindGO<Player>("player");
	m_ropeScale = ROPE_INITIAL_SCALE;
	m_ropeModelRender.SetScale(m_ropeScale);
	m_ropeRot = Quaternion::Identity;
	m_rollModelRender.SetScale(NO_HIT_COW_ROLL_ROPE_SCALE);

	/** ロープが縮むアニメーションのロード */
	m_ropeAnimationClips[enRopeAnimation_Shrink_Throw].Load(ROPE_SHRINK_ANIMATION_FILEPATH);
	m_ropeAnimationClips[enRopeAnimation_Shrink_Throw].SetLoopFlag(false);

	/** ロープが伸びるアニメーションのロード */
	m_ropeAnimationClips[enRopeAnimation_Stretch_Throw].Load(ROPE_EXTEND_ANIMATION_FILEPATH);
	m_ropeAnimationClips[enRopeAnimation_Stretch_Throw].SetLoopFlag(false);

	/** アニメーションを再生するロープモデルの初期化 */
	m_animationRopeModelRender.Init(ROPE_ANIMATION_MODEL_FILEPATH,m_ropeAnimationClips,enRopeAnimation_Num,enModelUpAxisZ);

	m_animationRopeModelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	return true;
}


void Rope::Update()
{
	m_timer = FindGO<Timer>("timer");
	
	/** タイマーが存在しないなら処理しない */
	if (m_timer == nullptr) return;

	/** タイマーが1秒未満なら処理しない */
	if (m_timer->GetTimer() < 1.0f)
	{
		if (m_hitCow != nullptr && m_hitCow->IsDead())
		{
			m_isHitCow = false;
			m_hitCow = nullptr;
		}
		return;
	}

	/** プレイヤーが存在しないなら処理しない */
	if (!m_player) return;

	if (!m_animationRopeModelRender.IsPlayingAnimation() && m_currentAnimation == enRopeAnimation_Shrink_Throw)
	{
		m_animationRopeModelRender.PlayAnimation(enRopeAnimation_Shrink_Throw, 1.5f);
		m_currentAnimation = enRopeAnimation_Stretch_Throw;
	}

	if (!m_animationRopeModelRender.IsPlayingAnimation() && m_currentAnimation == enRopeAnimation_Stretch_Throw)
	{
		m_animationRopeModelRender.PlayAnimation(enRopeAnimation_Stretch_Throw, 1.5f);
		m_currentAnimation = enRopeAnimation_Num;
	}

	m_animationRopeModelRender.Update();

	/** Dead になった牛のポインタをリセット */
	if (m_hitCow != nullptr && m_hitCow->IsDead())
	{
		m_isHitCow = false;
		m_hitCow = nullptr;
	}

	/** プレイヤーがロープを投げる処理 */
	PlayerThrowsRope();

	/** ロープアニメーション(ロープを投げた時のフラグ)がfalseなら */
	if (!m_isThrowRope && !m_isHitCow)
	{
		/** プレイヤーの右手の位置のロープが常にある関数 */
		FollowRightHand();
	}

	/** ロープの回転に関する関数 */
	RotateRope();

	if (m_isHitCow && m_hitCow != nullptr)
	{

		/** たるみを時間経過で50から0に減らす */
		if (m_ropeSlackTime < ROPE_SLACK_DURATION)
		{
			m_ropeSlackTime += g_gameTime->GetFrameDeltaTime() / DELTA_TIME_SCALE;
			
			float t = m_ropeSlackTime / ROPE_SLACK_DURATION;
			
			/** イーズアウト：最初ゆっくり、後半はビシっと戻る */
			t = 1.0f - (1.0f - t) * (1.0f - t);
			m_ropeSlack = 50.0f * (1.0f - t);
		}
		else
		{
			m_ropeSlack = 0.0f;
		}

		/** 既存のStretchRope/RotateStretchRopeの代わりに */
		/** セグメントの位置の計算関数 */		
		CalcCatenarySegments();
		/** セグメントの更新 */
		UpdateSegments();

		/** 牛の位置と回転を取得 */
		Vector3 cowPos = m_hitCow->GetPosition();
		cowPos.y += 30.0f;
		Quaternion cowRot = m_hitCow->GetRotation();

		/** 捕まった牛用のロープモデルに反映 */
		m_rollModelRender.SetPosition(cowPos);
		m_rollModelRender.SetRotation(cowRot);

		/** 牛が連れ去られている時はロープのスケールを変える */
		if (m_hitCow->GetIsTakeAwayed())
		{
			/** 牛の高さを取得 */
			float currentHeight = m_hitCow->GetPosition().y;

			/** 牛の高さをUFO_CATCH_HEIGHTで割って0~1に正規化 */
			float t = currentHeight / UFO_CATCH_HEIGHT;

			/** 0~1の範囲に制限 */
			t = max(0.0f, min(1.0f, t));

			/** 牛がUFOに近づくにつれてロープのスケールを小さくする */
			float scale = max(1.0f - t, COW_MIN_SCALE);
			m_rollModelRender.SetScale(Vector3(scale, scale, scale));

			/** スケールが小さくなるにつれてロープの位置を下げる */
			cowPos.y -= (1.0f - scale) * 30.0f;
			m_rollModelRender.SetPosition(cowPos);
		}
		else
		{
			/** 通常時はHIT_COW_ROLL_ROPE_SCALEに戻す */
			m_rollModelRender.SetScale(HIT_COW_ROLL_ROPE_SCALE);
		}

	}

	/** 牛に当たっているかのフラグがtrueなら */
	if (m_isThrowRope) {
		m_rollModelRender.SetScale(HIT_COW_ROLL_ROPE_SCALE);
	}

	m_rollModelRender.Update();
	m_ropeModelRender.Update();
}


void Rope::OnHitCow(Cow* cow)
{
	m_isHitCow = true;
	m_hitCow = cow;

	cow->SetIsCaptured(true);

	/** 牛を捕まえた瞬間にたるみをリセット */
	m_ropeSlack = 50.0f;
	m_ropeSlackTime = 0.0f;

	/** 投げアニメーション中に命中した場合に備えて強制終了する */
	m_isThrowRope = false;
	m_isStartRopeAnimation = false;
	m_ropeAnimationTime = 0.0f;
}


void Rope::PlayerThrowsRope()
{
	/** ロープが投げられていたら */
	if (m_isThrowRope && !m_isStartRopeAnimation)
	{
		/** ロープアニメーション開始のフラグを立てる */
		m_isStartRopeAnimation = true;

		/** プレイヤーが向いている方向にロープを出す */
		m_ropeModelRender.SetRotation(m_player->GetRotation());

		m_ropeRot = m_player->GetRotation();
	}

	/** ロープアニメーションが開始していたら */
	if (m_isStartRopeAnimation)
	{
		/** ロープアニメーション時間を加算する */
		m_ropeAnimationTime += g_gameTime->GetFrameDeltaTime();

		/** ロープアニメーションが1秒より大きかったら */
		if (m_ropeAnimationTime > 1.0f)
		{
			m_ropeAnimationTime = 0.0f;
			m_isStartRopeAnimation = false;
			m_isThrowRope = false;
		}
	}
}


void Rope::FollowRightHand()
{
	Vector3 playerPos = m_player->GetPosition();
	Quaternion playerRot = m_player->GetRotation();

	Vector3 right = Vector3::AxisX;
	playerRot.Apply(right);

	Vector3 forward = Vector3::AxisZ;
	playerRot.Apply(forward);
	
	Vector3 ropePos =
		playerPos
		+ right * ROPE_OFFSET_RIGHT
		+ forward * ROPE_OFFSET_FORWARD
		+ Vector3(0.0f, ROPE_OFFSET_UP, 0.0f);

	m_ropePos = ropePos;
	m_ropeModelRender.SetPosition(ropePos);
	m_rollModelRender.SetScale(NO_HIT_COW_ROLL_ROPE_SCALE);
	m_rollModelRender.SetPosition(ropePos);
	m_rollModelRender.SetRotation(playerRot);

}


void Rope::RotateRope()
{
	/** カメラの前にモデルを出す */
	GameCamera* gameCamera = FindGO<GameCamera>("gameCamera");

	if(gameCamera == nullptr) return;

	/** 牛に当たっているかのフラグがtrueならこの処理はしない */
	if (gameCamera->GetIsCowCaptured())
	{
		return;
	}

	/** ロープを投げている最中なら */
	if (m_isThrowRope)
	{
		/** カメラの前方向を取得 */
		Vector3 camForward = gameCamera->GetCameraForward();

		/** カメラの前にロープを出す */
		Vector3 camPos = gameCamera->GetCameraPosition();
		Vector3 ropePos = camPos + camForward * 50.0f;

		/** モデルの回転補正90度回転 */
		Quaternion fixRot;
		fixRot.SetRotation(Vector3::AxisX, 90.0f);

		/** forwardから回転を作る */
		Quaternion camRot;
		camRot.SetRotation(Vector3::AxisZ, camForward);

		m_ropeRot = camRot;

		/** ロープをスピンさせるための回転 */
		Quaternion spinRot;
		spinRot.SetRotation(Vector3::AxisY, m_ropeAnimationTime * 10.0f);
		m_ropeRot *= spinRot * fixRot;		

		/** ロープの位置と回転を反映 */
		m_rollModelRender.SetPosition(ropePos);

		m_rollModelRender.SetRotation(m_ropeRot);
	}
}

void Rope::CalcCatenarySegments()
{
	/** 牛に当たっていないなら処理しない */
	if (m_hitCow == nullptr) return;

	/** プレイヤーの手元 */
	Vector3 start = m_ropePos;

	/** 牛の少し上 */
	Vector3 end = m_hitCow->GetPosition();
	end.y += ROPE_AIM_UP_OFFSET;

	/** 牛が連れ去られている時は
	終点をスケール補正後の位置に合わせる */
	if (m_hitCow->GetIsTakeAwayed())
	{
		float currentHeight = m_hitCow->GetPosition().y;
		float t = currentHeight / UFO_CATCH_HEIGHT;
		t = max(0.0f, min(1.0f, t));
		float scale = max(1.0f - t, COW_MIN_SCALE);

		/** m_rollModelRender と同じ位置計算 */
		end.y -= (1.0f - scale) * 30.0f;
	}

	/** 終点を少し手前にずらして貫通を防ぐ */
	Vector3 dir = end - start;
	dir.Normalize();

	/** 終点を20ユニット手前にずらす */
	/** この20は調整してください */
	end -= dir * 20.0f; 


	/** セグメントの位置を計算 */
	/** これをすることでロープの形状を計算する */
	for (int i = 0; i <= ROPE_SEGMENT_COUNT; i++)
	{
		/** 線形補間の割合を計算 */
		float t = (float)i / (float)ROPE_SEGMENT_COUNT;

		/** 線形補間でXZ方向は均等に分割 */
		Vector3 pos = start + (end - start) * t;

		/** 放物線でY方向に垂れ下がりを計算 */
		/** t=0とt=1の端点では0, 中間(0.5)では最大値 */
		float sag = m_ropeSlack * t * (1.0f - t) * 4.0f;
		pos.y -= sag;

		/** 計算したセグメントの位置を保存 */
		m_ropeSegmentPositions[i] = pos;
	}
}

void Rope::UpdateSegments()
{
	/** セグメントの位置を計算 */
	for (int i = 0; i < ROPE_SEGMENT_COUNT; i++)
	{
		/** セグメントの開始位置と終了位置を取得 */
		Vector3 segStart = m_ropeSegmentPositions[i];
		Vector3 segEnd = m_ropeSegmentPositions[i + 1];

		/** セグメントの中心位置 */
		Vector3 center = (segStart + segEnd) * 0.5f;

		/** セグメントの向き */
		Vector3 dir = segEnd - segStart;
		float   len = dir.Length();
		dir.Normalize();

		/** Z軸をdirに向ける回転 */
		Quaternion rot;
		rot.SetRotation(Vector3::AxisZ, dir);

		/** スケール：Z方向にセグメントの長さ分だけ伸ばす */
		float scaleZ = max(len * ROPE_SCALE_FACTOR, ROPE_MIN_SCALE_Z);
		Vector3 scale(1.0f, 1.0f, scaleZ);

		/** セグメントのモデルレンダーに反映 */
		m_ropeSegments[i].SetPosition(center);
		m_ropeSegments[i].SetRotation(rot);
		m_ropeSegments[i].SetScale(scale);
		m_ropeSegments[i].Update();
	}
}


void Rope::Render(RenderContext& rc)
{
	
	m_animationRopeModelRender.Draw(rc);

	m_rollModelRender.Draw(rc);

	if (m_isHitCow)
	{
		/** 牛に当たっているならセグメントを描画する */
		for (int i = 0; i < ROPE_SEGMENT_COUNT; i++)
		{
			m_ropeSegments[i].Draw(rc);
		}
	}

	/** ロープを投げている最中ならロープモデルを描画する */
	else if (m_isThrowRope)
	{
		m_ropeModelRender.Draw(rc);
	}

	
}