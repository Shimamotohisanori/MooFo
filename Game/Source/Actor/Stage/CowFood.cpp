#include "stdafx.h"
#include "CowFood.h"
#include "EffectManager/EffectManager.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Rope/Rope.h"
#include "CountDown/CountDown.h"
namespace
{
	/** 牛の餌のモデルファイルパス */
	const char* COWFOOD_FILE_PATH = "Assets/modelData/Stage/CowFood.tkm";

	/** ボタンのUIのファイルパス */
	const char* BUTTON_UI = "Assets/sprite/CowFoodUI/test.dds";

	/** 牛の餌のUIのファイルパス */
	const char* FOODBUCKET_UI = "Assets/sprite/CowFoodUI/FoodBucket.dds";

	/** 牛の餌のUI(透明状態)のファイルパス */
	const char* FOODBUCKET_BLACK_UI = "Assets/sprite/CowFoodUI/FoodBucketBlack.dds";

	/** 牛の餌の座標 */
	const Vector3 COWFOOD_POS = { -1260.0f,-5.0f,-350.0f };

	/** 牛の餌のUIの座標 */
	const Vector3 FOODUI_POS       = { -850.0f,-250.0f,0.0f };
	const Vector3 FOODUI_SECOND_POS = { -700.0f,-250.0f,0.0f };

	/** 牛の餌のエフェクトの大きさ */
	const Vector3 COWFOOD_EFFECT_SCALE = { 40.0f,40.0f,40.0f };

}

CowFood::~CowFood()
{
	/** 牛の餌のエフェクトを削除する。 */
	DeleteGO(m_cowFoodEffect);
}

bool CowFood::Start()
{
	/** 牛の餌のモデル */
	m_cowFoodModelRender.Init(COWFOOD_FILE_PATH);
	m_cowFoodModelRender.SetPosition(COWFOOD_POS);
	m_cowFoodModelRender.Update();

	/** AボタンのUI */
	m_Abutton.Init(BUTTON_UI,280.0f,108.0f);

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

	/** 牛の餌の当たり判定をつける */
	m_FoodObject.CreateFromModel(m_cowFoodModelRender.GetModel(), m_cowFoodModelRender.GetModel().GetWorldMatrix());

	/** 餌のエフェクトを出す */
	m_cowFoodEffect = NewGO<nsK2EngineLow::EffectEmitter>(0);

	m_cowFoodEffect->Init((int)EffectID::EffectID_CowFoodEffect);

	/** 牛の餌のエフェクトのポジション */
	Vector3 cowFoodPos = { COWFOOD_POS.x, COWFOOD_POS.y + 30.0f, COWFOOD_POS.z + 10.0f };

	m_cowFoodEffect->SetPosition(cowFoodPos);
	m_cowFoodEffect->SetScale(COWFOOD_EFFECT_SCALE);
	
	m_rope = FindGO<Rope>("rope");

	m_CountDown = FindGO<CountDown>("countdown");

	return true;
}

void CowFood::Update()
{
	if (m_CountDown && m_CountDown->GetIsCountDown())
	{
		return;
	}
	/** プレイヤーのクラスを見つける */
	Player* player = FindGO<Player>("player");

	CowFoodPut();

	/** もしエフェクト再生中じゃないなら */
	if (!m_cowFoodEffect->IsPlay())
	{
		/** エフェクトを再生させる */
		m_cowFoodEffect->Play();
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
		m_Abutton.Update();

		if(g_pad[0]->IsTrigger(enButtonA))
		{
			m_foodCount = 2;
			m_isPutFood = true;
		}
	}
	else
	{
		m_isButtonUI = false;
	}
	
}

void CowFood::CowFoodPut()
{


	if (!m_rope->GetIsThrowRope() && !m_rope->GetIsHitCow() && g_pad[0]->IsTrigger(enButtonLB2))
	{
		if (m_foodCount > 0 && !m_isPutPlayer)
		{
			m_foodCount -= 1;
			m_isPutFood = true;
			m_isPutPlayer = true;
		}
	}
	else
	{
		m_isPutPlayer = false;
		return;
	}
}

void CowFood::Render(RenderContext& rc)
{
	/** 牛の餌のモデルを描画する。*/
	m_cowFoodModelRender.Draw(rc);

	/** フラグがtrueならボタンのUIを描画させる。*/
	if (m_isButtonUI)
	{
		m_Abutton.Draw(rc);
	}

	if (m_foodCount == 2)
	{
		m_bucketFood.Draw(rc);
		m_bucketFood_Second.Draw(rc);
	}
	else if (m_foodCount == 1)
	{
		m_bucketFood.Draw(rc);
		m_bucketFood_SecondBlack.Draw(rc);
	}
	else
	{
		m_bucketFood_Black.Draw(rc);
		m_bucketFood_SecondBlack.Draw(rc);
	}
	
}