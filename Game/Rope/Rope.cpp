#include "stdafx.h"
#include "Rope.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "GameCamera/GameCamera.h"
#include "Source/Actor/Character/Player/Player.h"
#include "GameTimer/Timer.h"
#include "SoundManager/SoundManager.h"
#include "SoundManager/VoiceManager.h"

namespace
{
	/** ロープのモデルファイルパス */
	const char* ROPE_MODEL_FILEPATH = "Assets/modelData/Rope/NewRope.tkm";

	/** 捕まった牛用のロープモデルファイルパス */
	const char* CAPTURED_COW_FILEPATH = "Assets/modelData/Rope/CapturedCowRope.tkm";

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

	/** ロープの縮むアニメーション継続時間 */
	const float ROPE_SHRINK_DURATION = 1.0f;

	/** ロープのたるみアニメーションの時間をスケールする係数 */
	constexpr float DELTA_TIME_SCALE = 5.0f;

	/** UFOが牛を捕獲する高さ */
	constexpr float UFO_CATCH_HEIGHT = 400.0f;

	/** 牛が連れ去られる時のロープの最小スケール */
	constexpr float COW_MIN_SCALE = 0.1f;

	/** ゴーストの前進速度 */
	constexpr float ROPE_THROW_SPEED = 1000.0f;

	/** ロープが伸びきる最大距離 */
	constexpr float ROPE_MAX_THROW_DISTANCE = 400.0f;

	/** ゴーストの当たり判定半径 */
	constexpr float GHOST_RADIUS = 30.0f;
}


Rope::Rope()
{
	

}


Rope::~Rope()
{

}


bool Rope::Start()
{
	m_ropeModelRender.Init(CAPTURED_COW_FILEPATH);

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
	m_ropeRot = Quaternion::Identity;
	m_ropeModelRender.SetScale(NO_HIT_COW_ROLL_ROPE_SCALE);

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

	/** 投げている最中はセグメントを伸ばす */
	if (m_isThrowRope || m_isEndRopeAnimation)
	{
		UpdateHandPosition();

		if (m_isThrowRope)
		{
			UpdateThrowGhost();
		}

		CalcThrowSegments();
		UpdateSegments();
		
		if (m_isThrowRope)
		{
			CheckHitCowByGhost();
		}
		
	}

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
		m_ropeModelRender.SetPosition(cowPos);
		m_ropeModelRender.SetRotation(cowRot);

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
			m_ropeModelRender.SetScale(Vector3(scale, scale, scale));

			/** スケールが小さくなるにつれてロープの位置を下げる */
			cowPos.y -= (1.0f - scale) * 30.0f;
			m_ropeModelRender.SetPosition(cowPos);
		}
		else
		{
			/** 通常時はHIT_COW_ROLL_ROPE_SCALEに戻す */
			m_ropeModelRender.SetScale(HIT_COW_ROLL_ROPE_SCALE);
		}

	}

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

	/** 牛を捕まえた時のカメラ演出フラグを立てる */
	GameCamera* gameCamera = FindGO<GameCamera>("gameCamera");
	if (gameCamera != nullptr)
	{
		gameCamera->SetIsCowCaptured(true);

		/** 牛を捕まえた瞬間のカメラ位置を記録する */
		gameCamera->InitHitCowCameraPos();
	}

	/** 牛を捕まえた音を再生 */
	SoundManager* soundManager = FindGO<SoundManager>("soundmanager");
	if (soundManager)
	{
		soundManager->PlayingSE(SoundSE::enCowCatchSE, false);
	}

	/** 牛を捕まえたボイスを再生(ランダム) */
	VoiceManager* voiceManager = FindGO<VoiceManager>("voicemanager");
	if (voiceManager)
	{
		SoundVoice voice = (rand() % 2 == 0) ? SoundVoice::enVoice_Catch1 : SoundVoice::enVoice_Catch2;
		voiceManager->PlayingVoice(voice, false);
	}
}


void Rope::PlayerThrowsRope()
{
	/** ロープを投げる処理 */
	if (m_isThrowRope && !m_isStartRopeAnimation && !m_isEndRopeAnimation)
	{
		/** ロープアニメーション開始のフラグを立てる */
		m_isStartRopeAnimation = true;

		m_ropeRot = m_player->GetRotation();

		m_ropeAnimationTime = 0.0f;

		m_ropeThrowDistance = 0.0f;

		/** カメラを取得して、投げた瞬間の前方向を固定する */
		GameCamera* gameCamera = FindGO<GameCamera>("gameCamera");
		Vector3 forward = Vector3::AxisZ; // 念のためのデフォルト

		if (gameCamera != nullptr)
		{
			forward = gameCamera->GetCameraForward();

			if (forward.LengthSq() > 0.0001f)
			{
				forward.Normalize();
			}
			else
			{
				forward = Vector3::AxisZ;
			}
		}

		/** 投げ始めの瞬間、プレイヤーの位置を基準に輪っかの位置を1回だけ決める */
		Vector3 playerPos = m_player->GetPosition();

		m_ropePos = playerPos + forward * ROPE_OFFSET_FORWARD + Vector3(0.0f, ROPE_OFFSET_UP, 0.0f);

		/** 飛んでいく起点として固定保存する(ゴースト・輪っか前進の基準) */
		m_ropeThrowOrigin = m_ropePos;

		/** 投げる方向は「カメラの前方向」で固定する */
		m_throwDirection = forward;

		/** 輪っかの回転も、この固定した方向を基準にする */
		m_ropeRot.SetRotation(Vector3::AxisZ, forward);


		/** ゴーストを輪っかの位置に生成 */
		if (!m_isGhostCreated)
		{
			/** ゴーストを生成 */
			m_ropeGhostObject.CreateSphere(m_ropePos, Quaternion::Identity, GHOST_RADIUS);
			m_isGhostCreated = true;
		}

		/** ゴーストの位置を輪っかの位置に設定 */
		else
		{
			m_ropeGhostObject.SetPosition(m_ropePos);
		}
	}

	/** ロープアニメーションが開始していたら */
	if (m_isStartRopeAnimation)
	{
		/** ロープアニメーション時間を加算する */
		m_ropeAnimationTime += g_gameTime->GetFrameDeltaTime();

		/** ロープアニメーションが1秒より大きかったら */
		if (m_ropeAnimationTime > 1.0f)
		{
			m_ropeThrowDistance = 0.0f;
			m_ropeAnimationTime = 0.0f;
			m_isStartRopeAnimation = false;
			m_isThrowRope = false;

			/** 牛に当たっていない場合はロープアニメーションを終了する */
			if (!m_isHitCow)
			{
				/** ロープアニメーション終了のフラグを立てる */
				m_isEndRopeAnimation = true;
				m_ropeShrinkTime = 0.0f;
			}
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
	m_ropeModelRender.SetScale(NO_HIT_COW_ROLL_ROPE_SCALE);
	m_ropeModelRender.SetPosition(ropePos);
	m_ropeModelRender.SetRotation(playerRot);

}


void Rope::RotateRope()
{
	/** 縮み中ならこちらを優先する */
	if (m_isEndRopeAnimation)
	{
		/** 時間加算はここだけで行う */
		m_ropeShrinkTime += g_gameTime->GetFrameDeltaTime();
		float t = min(m_ropeShrinkTime / ROPE_SHRINK_DURATION, 1.0f);

		Vector3 start = m_ropePos;
		Vector3 end = m_ropeGhostPosition;
		Vector3 shrinkPos = start + (end - start) * (1.0f - t);

		m_ropeModelRender.SetPosition(shrinkPos);
		m_ropeModelRender.SetRotation(m_player->GetRotation());
		m_ropeModelRender.SetScale(NO_HIT_COW_ROLL_ROPE_SCALE);

		if (t >= 1.0f)
		{
			/** 完全に縮み終わったらここでフラグを下ろす(唯一の場所) */
			m_isEndRopeAnimation = false;
		}

		return;
	}
	if (m_isThrowRope)
	{
		Quaternion fixRot;
		fixRot.SetRotation(Vector3::AxisX, 90.0f);

		Quaternion camRot;
		camRot.SetRotation(Vector3::AxisZ, m_throwDirection); /** 固定した方向を使う */

		m_ropeRot = camRot;

		Quaternion spinRot;
		spinRot.SetRotation(Vector3::AxisY, m_ropeAnimationTime * 10.0f);
		m_ropeRot *= spinRot * fixRot;

		/** 輪っかの位置を、ゴースト(先端)の位置に合わせて前進させる */
		m_ropeModelRender.SetPosition(m_ropeGhostPosition);
		m_ropeModelRender.SetRotation(m_ropeRot);
		m_ropeModelRender.SetScale(NO_HIT_COW_ROLL_ROPE_SCALE);
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

void Rope::CalcThrowSegments()
{
	Vector3 start = m_ropePos;
	Vector3 end = m_ropeGhostPosition;

	/** ロープ縮みアニメーション */
	if (m_isEndRopeAnimation)
	{
		/** m_ropeShrinkTimeを参照 */
		float t = min(m_ropeShrinkTime / ROPE_SHRINK_DURATION, 1.0f);
		end = start + (end - start) * (1.0f - t);
	}

	/** セグメント計算（既存） */
	for (int i = 0; i <= ROPE_SEGMENT_COUNT; i++)
	{
		float segT = (float)i / (float)ROPE_SEGMENT_COUNT;
		Vector3 pos = start + (end - start) * segT;
		m_ropeSegmentPositions[i] = pos;
	}
}

void Rope::UpdateThrowGhost()
{
	m_ropeThrowDistance += ROPE_THROW_SPEED * g_gameTime->GetFrameDeltaTime();
	m_ropeThrowDistance = min(m_ropeThrowDistance, ROPE_MAX_THROW_DISTANCE);

	/** 固定した起点から前進させる */
	m_ropeGhostPosition = m_ropeThrowOrigin + m_throwDirection * m_ropeThrowDistance;
	m_ropeGhostObject.SetPosition(m_ropeGhostPosition);

	/** 地面に当たったら縮み開始 */
	if (m_ropeGhostPosition.y <= 0.0f)
	{
		/** 伸びるアニメーションを強制終了 */
		m_isThrowRope = false;
		m_isStartRopeAnimation = false;
		m_ropeAnimationTime = 0.0f;

		/** 牛に当たっていないなら縮み開始 */
		if (!m_isHitCow)
		{
			m_isEndRopeAnimation = true;
			m_ropeShrinkTime = 0.0f;
		}
	}
}

void Rope::CheckHitCowByGhost()
{
	/** ゴーストが作成されていないなら処理しない */
	if (!m_isGhostCreated) return;

	/** ゴーストが牛に当たったか判定 */
	for (auto cow : m_cowList)
	{
		/** 牛が存在しないなら処理しない */
		if (!cow) continue;
		/** 牛が連れ去られていないなら処理しない */
		if (!cow->GetIsTakeAwayed()) continue;
		
		/** 牛が削除予定なら処理しない */
		if (cow->GetIsPendingKill()) continue;
		/** 牛とゴーストの当たり判定 */
		PhysicsWorld::GetInstance()->ContactTest(
			cow->GetCharacterController(),
			[this, cow](const btCollisionObject& contactObj)
			{
				if (m_ropeGhostObject.IsSelf(contactObj))
				{
					OnHitCow(cow);
				}
			}
		);

		if (m_isHitCow) break;
	}
}

void Rope::UpdateHandPosition()
{
	Vector3 playerPos = m_player->GetPosition();
	Quaternion playerRot = m_player->GetRotation();

	Vector3 right = Vector3::AxisX;
	playerRot.Apply(right);

	Vector3 forward = Vector3::AxisZ;
	playerRot.Apply(forward);

	m_ropePos =
		playerPos
		+ right * ROPE_OFFSET_RIGHT
		+ forward * ROPE_OFFSET_FORWARD
		+ Vector3(0.0f, ROPE_OFFSET_UP, 0.0f);
}


void Rope::Render(RenderContext& rc)
{

	m_ropeModelRender.Draw(rc);

	if (m_isHitCow || m_isThrowRope || m_isEndRopeAnimation)
	{
		/** 牛に当たっているならセグメントを描画する */
		for (int i = 0; i < ROPE_SEGMENT_COUNT; i++)
		{
			m_ropeSegments[i].Draw(rc);
		}
	}
	
}