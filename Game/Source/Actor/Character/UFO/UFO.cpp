#include "stdafx.h"
#include "UFO.h"
#include <time.h>
#include "CountDown/CountDown.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "GameScene/Game.h"
#include "CowNumberOfRescues/CowNumberOfRescues.h"
#include "Rope/Rope.h"
#include "GameCamera/GameCamera.h"
#include "Score/Score.h"

namespace
{
	const char* FILEPATH = "Assets/modelData/UFO/UFO2.tkm"; //enModelUpAxis = enModelUpAxisZ;

	/** UFOの大きさ */
	constexpr float UFO_SCALE = 3.5f;

	/** UFOの移動方向を決めるための乱数の範囲 */
	constexpr int   MOVE_DIR_RANGE = 3;

	/** UFOの移動時間と休む時間 */
	constexpr float REST_TIME_SEC = 60.0f;
	constexpr float MOVE_TIME_SEC = 120.0f;

	/** UFOの回転の閾値 */
	constexpr float ROTATION_THRESHOLD = 0.0001f;

	/** 牛を連れていくときのUFOの高さと牛を連れていく速さ */
	constexpr float CAPTURE_HEIGHT = 150.0f;
	constexpr float CAPTURE_SPEED = 0.5f;

	/** 牛を見つけるときの距離の二乗 */
	constexpr float CAPTURE_RANGE_SQ = 600.0f;

	/**移動可能エリア（柵の内側）*/
	constexpr float AREA_MIN_X = -1400.0f;
	constexpr float AREA_MAX_X = 1400.0f;
	constexpr float AREA_MIN_Z = -1100.0f;
	constexpr float AREA_MAX_Z = 1100.0f;
}
UFO::UFO()
{

}

UFO::~UFO()
{

}


bool UFO::Start()
{
	//// ★ 強制的に +X 方向へ移動（テスト用）
	//m_moveDir = Vector3(0.0f, 0.0f, 1.0f);
	m_countdown = FindGO<CountDown>("countdown");
	m_score = FindGO<Score>("score");
	srand(time(nullptr));
	m_ufomodelRender.SetScale(UFO_SCALE,UFO_SCALE,UFO_SCALE);
	m_ufomodelRender.Init(FILEPATH);
	m_ufomodelRender.SetPosition(m_transform.GetPosition());
	m_ufomodelRender.Update();
	return true;
}

void UFO::Update()
{
	/** カウントダウン中はUFOを動かさない */
	if (m_countdown->GetCountDown())
	{
		return;
	}

	if (m_UFOState == EnUFOState_Move)
	{
		/** 移動 */
		Move();
		/** 回転 */
		Rotation();
	}

	/** 牛を見つける関数 */
	FindTheCow();

	/** 牛を連れていく関数 */
	TakeAwayTheCow();

	/** モデルの位置を反映 */
	m_ufomodelRender.SetPosition(m_transform.GetPosition());
	/** モデルの回転を反映 */
	m_ufomodelRender.SetRotation(m_transform.GetRotation());
	/** モデルの更新 */
	m_ufomodelRender.Update();
}
		

void UFO::Move()
{
	/** 牛を連れていってる最中は移動させない */
	if(m_isCowTakeAwayed)
	{
		return;
	}

	if (m_moveTimer <= 0)
	{
		Vector3 dir
		(
			//(0,1,2,から-1を引いているので)-1,0,1の範囲でランダムな値を生成
			rand() % MOVE_DIR_RANGE - 1,//x
			0,             //yは常に0
			rand() % MOVE_DIR_RANGE - 1//z
		);
		// 0,0,0になったら一秒休む
		if (dir.LengthSq() == 0)
		{
			m_moveDir = Vector3::Zero;
			m_moveTimer = REST_TIME_SEC;//1秒休む
		}
		else
		{
			dir.Normalize();
			m_moveDir = dir;
			m_moveTimer = MOVE_TIME_SEC;//2秒ごとに方向を変える
		}
	}
	//移動
	Vector3 pos = m_transform.GetPosition();
	//少しづつ位置を動かす
	pos += m_moveDir * m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	//四方の柵に当たったら反転させる
	if (pos.x < AREA_MIN_X)
	{
		pos.x = AREA_MIN_X;
		m_moveDir.x *= -1;//壁に当たったら反転させる
	}
	else if(pos.x > AREA_MAX_X)
	{
		pos.x = AREA_MAX_X;
		m_moveDir.x *= -1;//壁に当たったら反転させる
	}

	if (pos.z < AREA_MIN_Z)
	{
		pos.z = AREA_MIN_Z;
		m_moveDir.z *= -1;//壁に当たったら反転させる
	}
	else if (pos.z > AREA_MAX_Z)
	{
		pos.z = AREA_MAX_Z;
		m_moveDir.z *= -1;//壁に当たったら反転させる
	}
	//ポジションを更新
	m_transform.SetPosition(pos);
	//モデルに位置を反映
	m_ufomodelRender.SetPosition(m_transform.GetPosition());

	//タイマーを減らす。
	m_moveTimer--;

}
	
void UFO::Rotation()
{
	/** 牛を連れていってる最中は回転させない */
	if(m_isCowTakeAwayed)
	{
		return;
	}

	/** 少しでも動いたら移動方向に向きを回転させる */
	if (fabsf(m_moveDir.x) >= ROTATION_THRESHOLD || fabsf(m_moveDir.z) >= ROTATION_THRESHOLD)
	{
		//移動方向に回転させる
		m_transform.GetRotation().SetRotationYFromDirectionXZ(m_moveDir);
		m_transform.SetRotation(m_transform.GetRotation());
	}
}

void UFO::TakeAwayTheCow()
{
	/** 牛を連れていけるかどうかのフラグが立っていたら処理をする */
	if (!m_isCowTakeAwayed or m_targetCow == nullptr) return;

	/** 牛を回転状態にする */
	m_targetCow->ChangeRotationState();

	/** UFOの位置に牛を近づける */
	Vector3 cowPos = m_targetCow->GetPosition();
	Vector3 ufoPos = m_transform.GetPosition();
	ufoPos.y += CAPTURE_HEIGHT;

	Vector3 dir = ufoPos - cowPos;
	float dist = dir.Length();

	/** 牛とUFOの距離が0.5以上だったら牛をUFOに近づける */
	if (dist > 0.5f)
	{
		dir.Normalize();
		cowPos += dir * CAPTURE_SPEED;
	}

	//** 牛とUFOの距離が0.5未満だったら牛を削除し、牛の救出数を減らす */
	else
	{
		/** 牛の救出数を減らす */
		CowNumberOfRescues* cowNumberOfRescues = FindGO<CowNumberOfRescues>("cownumberofrescues");
		cowNumberOfRescues->SubRescue();

		/*牛が連れ去られたらスコアを減らす処理*/
		m_score->DecreaseScore(100);
		/* 牛の状態を連れていかれる前の状態に戻す */
		m_targetCow->SetIsTakeAwayed(false);

		/** Gameに通知してaliveCowsから外す */
		Game* game = FindGO<Game>("game");
		if (game)
		{
			game->ResetCombo();
			game->ReMoveCow(m_targetCow);
		}

		/** 牛を削除 */
		DeleteGO(m_targetCow);

		if (auto rope = FindGO<Rope>("rope"))
		{
			if (rope->GetHitCow() == m_targetCow)
			{
				rope->SetIsHitCow(false);
				rope->SetHitCow(nullptr);
			}
		}

		if (auto camera = FindGO<GameCamera>("gameCamera"))
		{
			camera->SetIsCowCaptured(false);
		}

		/** 状態をリセットする */
		m_targetCow = nullptr;
		m_isCowTakeAwayed = false;
		return;
	}

	m_targetCow->SetPosition(cowPos);

}

void UFO::FindTheCow()
{
	/** 牛を連れていけるかどうかのフラグが立っていたら処理しない */
	if (m_isCowTakeAwayed) return;

	auto cow = FindGOs<Cow>("cow");
	for (auto c : cow)
	{

		Vector3 diff = c->GetPosition() - m_transform.GetPosition();
		diff.y = 0.0f;

		/** 牛とUFOの距離が600未満だったら牛を連れていく */
		if (diff.LengthSq() < CAPTURE_RANGE_SQ)
		{
			/** 連れていける */
			m_isCowTakeAwayed = true;
			/** 最初の一匹だけ */
			m_targetCow = c;
			m_targetCow->SetTakingUFO(this);

			/** 牛の状態を連れていかれる状態にする */
			m_targetCow->SetIsTakeAwayed(true);
			break;
		}
	}
}

void UFO::Render(RenderContext& rc)
{
	m_ufomodelRender.Draw(rc);
}

