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
#include "Source/Actor/Character/UFO/CowCaptureController.h"
#include "Pause/Pause.h"
#include "Combo/Combo.h"
namespace
{
	const char* GAMECLEAR_FILEPATH = "Assets/modelData/UFO/UFO.tkm"; //enModelUpAxis = enModelUpAxisZ;

	/** UFOの大きさ*/
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
	constexpr float CAPTURE_SPEED = 0.2f;

	/** 牛を見つけるときの距離 */
	constexpr float CAPTURE_RANGE = 30.0f;

	/**移動可能エリア（柵の内側）*/
	constexpr float AREA_MIN_X = -1300.0f;
	constexpr float AREA_MAX_X = 1300.0f;
	constexpr float AREA_MIN_Z = -600.0f;
	constexpr float AREA_MAX_Z = 500.0f;

	/** UFO同士の回避処理のための距離の二乗と力 */
	constexpr float UFO_AVOID_RANGE_SQ = 1500.0f;
	constexpr float UFO_AVOID_FORCE = 1.5f;

	/** UFO同士の最低距離*/
	constexpr float MIN_DIST = 300.0f; 
}

UFO::UFO()
{

}

UFO::~UFO()
{
	DeleteGO(m_cowCaptureController);
}


bool UFO::Start()
{
	m_cowCaptureController =NewGO<CowCaptureController>(0,"cowcapturecontroller");
	/** それぞれのUFOに自分自身を設定 */
	m_cowCaptureController->SetUFO(this);
	m_ufomodelRender.SetScale(UFO_SCALE,UFO_SCALE,UFO_SCALE);
	m_ufomodelRender.Init(GAMECLEAR_FILEPATH);
	m_ufomodelRender.SetPosition(m_transform.GetPosition());
	m_ufomodelRender.Update();
	
	return true;
}

void UFO::Update()
{
	m_game = FindGO<Game>("game");
	m_countdown = FindGO<CountDown>("countdown");
	m_pause = FindGO<Pause>("pause");
	m_score = FindGO<Score>("score");

	if(m_pause == nullptr || m_countdown == nullptr || m_game == nullptr)
	{
		return;
	}

	/** タイムアウトしているときはUFOを動かさない */
	if (m_game->GetIsTimeOut())
	{
		return;
	}

	/** ポーズ中はUFOを動かさない */
	if (m_pause->GetIsPause())
	{
		return;
	}

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
	/**　光が出ているときだけ探す*/
	if (IsLightEmitting())
	{
		/** 牛を見つける関数 */
		FindTheCow();
	}
	

	/** 牛を連れていく関数 */
	TakeAwayTheCow();

	m_transform.SetPosition(Vector3{ m_transform.GetPosition().x,70.0f,m_transform.GetPosition().z });

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

	/** 追尾中なら近くの牛の方向に少しずつ進む。 */
	if (m_isChasing)
	{
		/** UFOの位置をposに入れる */
		Vector3 pos = m_transform.GetPosition();

		/** 現在の位置に少しだけ移動量を足している。 */
		pos += m_moveDir * m_moveSpeed * g_gameTime->GetFrameDeltaTime();

		/** 計算した新しい位置を、実際のUFOに反映する。 */
		m_transform.SetPosition(pos);
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
	
	Game* game = FindGO<Game>("game");
	/** UFO同士の反発処理 */
	if (game)
	{
		/** UFOのリストを取得 */
		auto ufos = game->GetUFOs();
		
		/** UFO同士の最低距離の二乗 */
		float MIN_DIST_SQ = MIN_DIST * MIN_DIST;

		/** UFO同士の距離が近すぎたら反発する */
		for (auto u : ufos)
		{
			/** 自分自身はスキップ */
			if (u == this) continue;

			/** 自分と他のUFOの距離を計算 */
			Vector3 otherPos = u->m_transform.GetPosition();
			
			/**y軸は考慮しない */
			Vector3 diff = pos - otherPos;
			diff.y = 0.0f;

			/** UFO同士の距離の二乗 */
			float distSq = diff.LengthSq();

			/** 近すぎたら反発する */
			if (distSq < MIN_DIST_SQ)
			{
				/* 反発する方向を計算 */
				float dist = sqrtf(distSq);
				
				/** 方向ベクトルを作る。 */
				if (dist == 0.0f)
				{
				/**
				  * 永遠に重ならないようにランダム関数でどっちか逃がす
				　* 方向ベクトルが0の時Normalizeにできないため使用する
				  **/
					diff = Vector3(rand() % 3 - 1, 0, rand() % 3 - 1);
					diff.Normalize();
				}
				else
				{
					/** 正規化 */
					diff /= dist;
				}
				

				/** 反発する力を計算 */
				//float pushBack = (MIN_DIST - dist) * 0.5f;
				float pushBack = (MIN_DIST - dist);

				pos += diff * pushBack;

				/** 進行方向も変更する。 */
				m_moveDir += diff * 0.5f;
				m_moveDir.Normalize();

			}
		}
	}

	
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
		
		/** Gameに通知してaliveCowsから外す */
		Game* game = FindGO<Game>("game");
		Combo* combo = FindGO<Combo>("combo");
		if (game && combo)
		{
			combo->ResetCombo();
			game->ReMoveCow(m_targetCow);
		}

		/** 牛を削除 */
		DeleteGO(m_targetCow);

		m_cowCaptureController->EndCapture();
		/** 状態をリセットする */
		m_targetCow = nullptr;
		m_isCowTakeAwayed = false;
		m_cowCaptureController->SetCapturing(false);
		return;
	}

	m_targetCow->SetPosition(cowPos);

}

CowCaptureController* UFO::GetCowCaptureController()
{
	return m_cowCaptureController;
}

void UFO::FindTheCow()
{
	/**光が出ていないときは探さないようにする*/
	if (!m_cowCaptureController->GetIsEmitting()) return;
	/** 牛を連れていけるかどうかのフラグが立っていたら処理しない */
	if (m_isCowTakeAwayed) return;
	auto cow = FindGOs<Cow>("cow");

	Cow* nearestCow = nullptr;

	/** 最も近い牛を見つけるためにFLT_MAXを利用して一度最大数にしておく */
	float nearestDistSq = FLT_MAX;

	/** 牛を一匹ずつ見る。 */
	for (auto c : cow)
	{
		/** すでに近くの牛を捕まえようとしているUFOがいた場合無視する。 */
		if (c->GetIsTakeAwayed()) continue;

		/* 
		 * 距離を計算する。
		 * UFOから牛の方向を取得する。
		 */
		Vector3 diff = c->GetPosition() - m_transform.GetPosition();
		diff.y = 0.0f;

		/** 距離の二乗 */
		float distSq = diff.LengthSq();

		/** 一番近い牛を更新 */
		if (distSq < nearestDistSq)
		{
			nearestDistSq = distSq;
			nearestCow = c;
		}

	}

	/** 近くに牛が見つかっているかどうか */
	if (nearestCow)
	{
		/**  追尾中だったらtrue */
		m_isChasing = true;

		/** 牛の方向を作る。 */
		Vector3 dir = nearestCow->GetPosition() - m_transform.GetPosition();
		dir.Normalize();

		/** dirの方向に進むようにセットする。 */
		m_moveDir = dir;


		/** もし牛を牛を見つけるときの距離のほうが大きかったら */
		if (sqrtf(nearestDistSq) < CAPTURE_RANGE)
		{
			/** この牛を捕まえる(true) */
			m_isCowTakeAwayed = true;
			m_targetCow = nearestCow;

			/** 捕獲処理が作動する。 */
			m_cowCaptureController->StartCapture();

			/** いま追っている牛をtrueにして他のUFOは追尾しないようにする。 */
			m_targetCow->SetTakingUFO(this);
			m_targetCow->SetIsTakeAwayed(true);
		}
	}
	/** それいがいは追尾しない。 */
	else
	{
		m_isChasing = false;
	}

}


void UFO::Render(RenderContext& rc)
{
	m_ufomodelRender.Draw(rc);
}

