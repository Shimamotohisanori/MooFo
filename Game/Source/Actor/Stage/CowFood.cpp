#include "stdafx.h"
#include "CowFood.h"
#include "EffectManager/EffectManager.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Rope/Rope.h"
#include "CountDown/CountDown.h"
#include "CowFoodManager.h"
#include"GameScene/Game.h"
#include"GameScene/LoadingScene.h"
#include "SoundManager/SoundManager.h"

namespace
{
	/** 牛の餌のモデルファイルパス */
	const char* COWFOOD_FILE_PATH = "Assets/modelData/Stage/CowFood.tkm";

	/** 牛の餌のUIのファイルパス */
	const char* FOODBUCKET_UI = "Assets/sprite/CowFoodUI/FoodBucket.dds";

	/** 牛の餌のUI(透明状態)のファイルパス */
	const char* FOODBUCKET_BLACK_UI = "Assets/sprite/CowFoodUI/FoodBucketBlack.dds";

	/** 牛の餌のUI(禁止状態)のファイルパス */
	const char* TABOO_COWFOOD_UI = "Assets/sprite/CowFoodUI/TabooCowFood.dds";
	/** 牛の餌の座標 */
	const Vector3 COWFOOD_POS = { -1260.0f,-5.0f,-350.0f };

	/** 牛の餌のUIの座標 */
	const Vector3 FOODUI_POS       = { -850.0f,-250.0f,0.0f };
	const Vector3 FOODUI_SECOND_POS = { -700.0f,-250.0f,0.0f };

	/** 牛の餌のエフェクトの大きさ */
	const Vector3 COWFOOD_EFFECT_SCALE = { 40.0f,40.0f,40.0f };

	/** 牛舎付近に出すエフェクトの大きさ */
	const Vector3 BARNNEAR_EFFECT_SCALE = { 80.0f,80.0f,80.0f };

	/** 牛の餌を置くことができないエリアの座標 */
	const Vector3 COWFOOD_PUT_TABOO_POS = { -1320.0f,0.0f,10.0f };

	/** 牛の餌を置くことができないエリアの半径 */
	constexpr float COWFOOD_PUT_TABOO_RADIUS = 300.0f;
}

CowFood::~CowFood()
{
	/** 牛の餌のエフェクトを削除する。 */
	DeleteGO(m_cowFoodEffect);

	/** Aボタンのエフェクトを削除する */
	DeleteGO(m_AbuttonEffect);

	/** 牛舎付近のエフェクトを削除する */
	DeleteGO(m_barnNearEffect);

	/** 牛の餌を置く音を削除する。 */
	if (m_puthaySE != nullptr)
	{
		DeleteGO(m_puthaySE);
	}

	/** 牛の餌を取る音を削除する。 */
	if (m_takehaySE != nullptr)
	{
		DeleteGO(m_takehaySE);
	}
}

bool CowFood::Start()
{
	/** 牛の餌のモデル */
	m_cowFoodModelRender.Init(COWFOOD_FILE_PATH);
	m_cowFoodModelRender.SetPosition(COWFOOD_POS);
	m_cowFoodModelRender.Update();

	/** 牛の餌のUI(左) */
	m_bucketFood.Init(FOODBUCKET_UI, 100.0f, 100.0f);
	m_bucketFood.SetPosition(FOODUI_POS);
	m_bucketFood.Update();

	/** 牛の餌のUI(右) */
	m_bucketFood_Second.Init(FOODBUCKET_UI, 100.0f, 100.0f);
	m_bucketFood_Second.SetPosition(FOODUI_SECOND_POS);
	m_bucketFood_Second.Update();

	/** 牛の餌のシルエットUI(左) */
	m_bucketFood_Black.Init(FOODBUCKET_BLACK_UI, 100.0f, 100.0f);
	m_bucketFood_Black.SetPosition(FOODUI_POS);
	m_bucketFood_Black.Update();

	/** 牛の餌のシルエットUI(右) */
	m_bucketFood_SecondBlack.Init(FOODBUCKET_BLACK_UI, 100.0f, 100.0f);
	m_bucketFood_SecondBlack.SetPosition(FOODUI_SECOND_POS);
	m_bucketFood_SecondBlack.Update();

	/** 牛の餌の禁止状態UI(左) */
	m_tabooCowFood.Init(TABOO_COWFOOD_UI, 100.0f, 100.0f);
	m_tabooCowFood.SetPosition(FOODUI_POS);
	m_tabooCowFood.Update();

	/** 牛の餌の禁止状態UI(右) */
	m_tabooCowFood_Second.Init(TABOO_COWFOOD_UI, 100.0f, 100.0f);
	m_tabooCowFood_Second.SetPosition(FOODUI_SECOND_POS);
	m_tabooCowFood_Second.Update();

	/** 牛の餌の当たり判定をつける */
	m_FoodObject.CreateFromModel(m_cowFoodModelRender.GetModel(), m_cowFoodModelRender.GetModel().GetWorldMatrix());

	/** 餌のエフェクトを出す */
	m_cowFoodEffect = NewGO<nsK2EngineLow::EffectEmitter>(0);

	/** エフェクトIDを指定して初期化する */
	m_cowFoodEffect->Init((int)EffectID::EffectID_CowFoodEffect);

	/** 牛の餌のエフェクトのポジション */
	Vector3 cowFoodPos = { COWFOOD_POS.x, COWFOOD_POS.y + 30.0f, COWFOOD_POS.z + 10.0f };

	m_cowFoodEffect->SetPosition(cowFoodPos);
	m_cowFoodEffect->SetScale(COWFOOD_EFFECT_SCALE);

	/** AボタンのエフェクトIDを指定して初期化する */
	m_AbuttonEffect = NewGO<nsK2EngineLow::EffectEmitter>(0);
	m_AbuttonEffect->Init((int)EffectID::EffectID_Abutton);

	Vector3 aButtonEffectPos = { COWFOOD_POS.x, COWFOOD_POS.y + 30.0f, COWFOOD_POS.z + 10.0f };

	m_AbuttonEffect->SetPosition(aButtonEffectPos);
	m_AbuttonEffect->SetScale(COWFOOD_EFFECT_SCALE);

	/** 牛舎付近のエフェクトを初期化する */
	m_barnNearEffect = NewGO<nsK2EngineLow::EffectEmitter>(0);
	m_barnNearEffect->Init((int)EffectID::EffectID_Area);

	/** 牛の餌が置けない位置と牛舎付近に出すエフェクトは同じ位置にする */
	m_barnNearEffect->SetPosition(COWFOOD_PUT_TABOO_POS);
	m_barnNearEffect->SetScale(BARNNEAR_EFFECT_SCALE);

	/** ロープのインスタンスを取得する */
	m_rope = FindGO<Rope>("rope");

	/** 牛の餌マネージャーのインスタンスを取得する */
	m_CowFoodManager = FindGO<CowFoodManager>("cowfoodmanager");

	return true;
}

void CowFood::Update()
{
	/** カウントダウンのインスタンスを取得する */
	m_CountDown = FindGO<CountDown>("countdown");

	/** カウントダウン中は処理をスキップする */
	if (m_CountDown && m_CountDown->GetCountDown())
	{
		return;
	}
	/** プレイヤーのクラスを見つける */
	Player* player = FindGO<Player>("player");


	/** ゲームクラスを見つける */
	m_game = FindGO<Game>("game");
  
	/** 餌を置く処理を実行する */
	CowFoodPut();

	/** もしエフェクト再生中じゃないなら */
	if (!m_cowFoodEffect->IsPlay())
	{
		/** エフェクトを再生させる */
		m_cowFoodEffect->Play();
	}

	/** もし牛舎付近のエフェクトが再生中じゃないなら */
	if (m_barnNearEffect && !m_barnNearEffect->IsPlay())
	{
		/** エフェクトを再生させる */
		m_barnNearEffect->Play();
	}

	if (player == nullptr)
	{
		return;
	}

	/** 牛の餌のポジションを変数に代入させる */
	m_position = COWFOOD_POS;
	m_cowFoodModelRender.SetPosition(m_position);

	/** プレイヤーの距離から牛の餌の距離を計算する */
	Vector3 dir = player->GetPosition() - m_position;
	dir.y = 0.0f;

	/** 二つの距離のベクトルの長さ */
	float distance = dir.Length();

	/** もし、100.0f以内なら */
	if (distance <= 100.0f)
	{
		/** ボタンのUIを表示させる。*/
		m_isButtonUI = true;

		/** 餌エフェクトを停止する */
		if (m_cowFoodEffect->IsPlay())
		{
			m_cowFoodEffect->Stop();
		}

		/** Aボタンエフェクトを再生する */
		if (!m_AbuttonEffect->IsPlay())
		{
			m_AbuttonEffect->Play();
		}
    
		/** Aボタンが押されたら餌の数を2にセットして設置フラグを立てる */
		if(g_pad[0]->IsTrigger(enButtonA))
		{
			/** 牛の餌を取る音を再生させる。*/
			SoundManager* soundManager = FindGO<SoundManager>("soundmanager");

			m_takehaySE = soundManager->PlayingSE(SoundSE::enTakehaySE, false);

			m_foodCount = 2;
			m_isPutFood = true;
		}
	}
	else
	{
		/** 距離が離れていたらボタンUIを非表示にする */
		m_isButtonUI = false;

		/** Aボタンエフェクトを停止する */
		if (m_AbuttonEffect->IsPlay())
		{
			m_AbuttonEffect->Stop();
		}
	}
	
}

void CowFood::CowFoodPut()
{
	Player* player = FindGO<Player>("player");

	/** 牛の餌を置くことができないエリアにいるかどうかの判定 */
	float taboodx = player->GetPosition().x - COWFOOD_PUT_TABOO_POS.x;
	float taboodz = player->GetPosition().z - COWFOOD_PUT_TABOO_POS.z;

	/** プレイヤーと牛の餌を置くことができないエリアの距離 */
	float taboorenge = sqrtf(taboodx * taboodx + taboodz * taboodz);

	/** 牛の餌を置くことができないエリアから離れているなら処理を辞める */
	if (taboorenge < COWFOOD_PUT_TABOO_RADIUS)
	{
		/** 牛の餌を置くことができないエリアにいるフラグを立てる */
		m_isInTabooArea = true;
		return;
	}
	else
	{
		m_isInTabooArea = false;
	}

	if (player == nullptr)
	{
		return;
	}

	/** クールタイム中ならタイマーを減らして処理をスキップ */
	if (m_isCoolTime)
	{
		m_coolTime -= g_gameTime->GetFrameDeltaTime();

		if (m_coolTime <= 0.0f)
		{
			m_isCoolTime = false;
		}
		return;
	}

	/** プレイヤーの座標を取得し、Y軸を0にする */
	Vector3 pos = player->GetPosition();
	pos.y = 0.0f;

	/** ロープを投げておらず、牛に当たっておらず、LB2ボタンが押されたら */
	//if (!m_rope->GetIsThrowRope() && !m_rope->GetIsHitCow() && g_pad[0]->IsTrigger(enButtonLB2))
	if (!m_rope->GetIsThrowRope() && !m_rope->GetIsHitCow() && g_pad[0]->IsTrigger(enButtonX))
	{
		/** 餌の残数があり、まだ設置中でなければ */
		if (m_foodCount > 0 && !m_isPutPlayer)
		{
			/** 餌の残数を1減らす */
			/** 牛の餌を置く音を再生させる。*/
			SoundManager* soundManager = FindGO<SoundManager>("soundmanager");
			m_puthaySE = soundManager->PlayingSE(SoundSE::enPuthaySE, false);
			m_foodCount -= 1;
			m_isPutFood = true;
			m_isPutPlayer = true;

			/** クールタイムを開始する */
			m_isCoolTime = true;
			m_coolTime = 1.0f;

			
			/** 牛の餌マネージャーに餌をスポーンさせる */
			if (m_CowFoodManager)
			{
				m_CowFoodManager->SpawnFood(pos);
			}
		}
	}
	else
	{
		/** ボタンが離されたら設置中フラグをリセットする */
		m_isPutPlayer = false;
		return;
	}
}

void CowFood::Render(RenderContext& rc)
{
	/**フェード完了までUIの表示を遅らす*/
	LoadingScene* lodingScene = FindGO<LoadingScene>("loading");
	if (lodingScene != nullptr && !lodingScene->GetLoadingEnd())
	{
		return;
	}
	/** タイムアウト時は餌のUIをでないようにする*/
	if (!m_game || m_game->IsFadeTimeOut())
	{
		return;
	}

	/** 牛の餌のモデルを描画する。*/
	m_cowFoodModelRender.Draw(rc);

	if (m_foodCount == 2)
	{
		/** 牛の餌を置くことができないエリアにいる場合は禁止状態のUIを描画する */
		if (m_isInTabooArea)
		{
			m_tabooCowFood.Draw(rc);
			m_tabooCowFood_Second.Draw(rc);
			return;
		}

		m_bucketFood.Draw(rc);
		m_bucketFood_Second.Draw(rc);
	}
	else if (m_foodCount == 1)
	{
		/** 牛の餌を置くことができないエリアにいる場合は禁止状態のUIを描画する */
		if (m_isInTabooArea)
		{
			m_tabooCowFood.Draw(rc);
			m_bucketFood_SecondBlack.Draw(rc);
			return;
		}

		m_bucketFood.Draw(rc);
		m_bucketFood_SecondBlack.Draw(rc);
	}
	else
	{
		m_bucketFood_Black.Draw(rc);
		m_bucketFood_SecondBlack.Draw(rc);
	}
	
}