#include "stdafx.h"
#include "Source/Actor/Character/UFO/CowCaptureController.h"
#include "UFO.h"
#include "CountDown/CountDown.h"
#include "Pause/Pause.h"
#include "GameScene/Game.h"
#include "EffectManager/EffectManager.h"
namespace
{
	const char* LIGHT_FILEPATH = "Assets/modelData/UFO/UFOLight.tkm";
	/** 光が出ているときに表示する数字のスプライト*/
	const char* LIGHT_APPEAR_NUMBER_FILEPATH[5] = 
	{
		"Assets/sprite/UFOLightFontUI/Number1.dds",
		"Assets/sprite/UFOLightFontUI/Number2.dds",
		"Assets/sprite/UFOLightFontUI/Number3.dds",
		"Assets/sprite/UFOLightFontUI/Number4.dds",
		"Assets/sprite/UFOLightFontUI/Number5.dds"
	};

	/** 光が出ているときに表示する「光の発射まで」のスプライト*/
	const char* LIGHT_APPEAR_SPRITE_FILEPATH = "Assets/sprite/UFOLightFontUI/NextLightFring.dds";
	/** 光が出ているときに発射する「秒」のスプライト*/
	const char* LIGHT_APPEAR_SECONDS_SPRITE_FILEPATH = "Assets/sprite/UFOLightFontUI/Seconds.dds";
	/** UFOの大きさ*/
	const Vector3 UFO_SCALE = Vector3(3.0f, 8.0f,3.0f);
	const Vector3 FONT_SCALE = Vector3(2.0f, 2.0f, 2.0f);
	/** 数字の大きさ*/
	const float NUMBERFONT_WIDTH = 100.0f;
	const float NUMBERFONT_HEIGHT = 100.0f;

	const float FONT_WIDTH = 200.0f;
	const float FONT_HEIGHT = 200.0f;

	const float SECOMDS_WIDTH = 100.0f;
	const float SECOMDS_HEIGHT = 100.0f;
}

CowCaptureController::CowCaptureController()
{

}

CowCaptureController::~CowCaptureController()
{

}


bool CowCaptureController::Start()
{
	/** スポットライト初期化*/
	m_ufoSpotLight.Init();
	/** 色の設定*/
	m_ufoSpotLight.SetColor(0.6f, 0.9f, 1.0f);
	/** スポットライト範囲*/
	m_ufoSpotLight.SetRange(200.0f);
	/** 射出方向*/
	m_ufoSpotLight.SetDirection(0.0f, 5.0f, 0.0f);
	/** 射出角度*/
	m_ufoSpotLight.SetAngle(100.0f);
	/** 影響率に累乗する値の設定*/
	m_ufoSpotLight.SetAngleAffectPowParam(0.6f);
	/** 影響率の累乗数の設定*/
	m_ufoSpotLight.SetRangeAffectPowParam(0.5f);

	/*m_ufocontrollermodelRender.Init(LIGHT_FILEPATH);
	m_ufocontrollermodelRender.SetScale(UFO_SCALE);*/
	for (int i = 0; i < 5; i++)
	{
			/** 光の数字スプライト初期化*/
		m_LightApperNumberSpriteRender[i].Init(LIGHT_APPEAR_NUMBER_FILEPATH[i], NUMBERFONT_WIDTH, NUMBERFONT_HEIGHT);
		m_LightApperNumberSpriteRender[i].Update();
	}
	/** 光の発射までのスプライト初期化*/
	m_LightApperSpriteRender.Init(LIGHT_APPEAR_SPRITE_FILEPATH, FONT_WIDTH, FONT_HEIGHT);
	m_LightApperSpriteRender.SetPosition(Vector3(740.0f,500.0f,0.0f));
	m_LightApperSpriteRender.SetScale(FONT_SCALE);
	m_LightApperSpriteRender.Update();
	/** 秒のスプライト初期化*/
	m_secondsSpriteRender.Init(LIGHT_APPEAR_SECONDS_SPRITE_FILEPATH, SECOMDS_WIDTH, SECOMDS_HEIGHT);
	m_secondsSpriteRender.SetPosition(Vector3(940.0f, 510.0f, 0.0f));
	m_secondsSpriteRender.SetScale(FONT_SCALE);
	m_secondsSpriteRender.Update();
	/**　最初は光が出ていない状態にする*/
	m_timer = m_waitTimer;
	
	return true;
}


void CowCaptureController::Update()
{
	m_game = FindGO<Game>("game");
	m_countdown = FindGO<CountDown>("countdown");
	m_pause = FindGO<Pause>("pause");
	if (m_pause == nullptr || m_countdown == nullptr || m_game == nullptr)
	{
		return;
	}

	/** タイムアウトしているときは処理を止める*/
	if (m_game->GetIsTimeOut())
	{
		return;
	}

	/** カウントダウン中は処理を止める*/
	if (m_countdown->GetCountDown())
	{
		return;
	}

	/** Pause中は処理を止める*/
	if (m_pause->GetIsPause())
	{
		return;
	}
	/** UFOが存在しない場合は処理を止める*/
	if (m_ufo == nullptr)
	{
		return;
	}
	/** タイマーのカウント処理*/
	CountTimer();
	/** スプライトのカウント処理*/
	CountSpriteUI();
	/** UFOの追従処理*/
	FollowTheCow();
	/** UFOのモデル更新*/
	//m_ufoSpotLight.Update();
	/** 数字スプライトの更新*/
	if (m_currentCount >= 0)
	{
		m_LightApperNumberSpriteRender[m_currentCount].Update();
	}
}


void CowCaptureController::FollowTheCow()
{
	if (!m_ufo)
	{
		return;
	}

	Vector3 pos = m_ufo->GetPosition();

	/** UFOの光をエフェクト*/

		//Vector3 pos = m_ufo->GetPosition();
		////pos.y -= 50.0f;
		//m_ufoSpotLight.SetPosition(pos);

		///** 光が出ているときだけ描画する*/
		//if (m_isEmitting ||m_ufo->GetIsCowTakeAwayed())
		//{
		//	m_ufoSpotLight.SetColor(0.6, 0.9f, 1.0f);
		//}
		//else
		//{
		//	m_ufoSpotLight.SetColor(0.0f, 0.0f, 0.0f);
		//}
		//m_ufoSpotLight.Update();
}




void CowCaptureController::CountTimer()
{
	switch (m_state)
	{
	case Wait:
		m_timer -= g_gameTime->GetFrameDeltaTime();
		if (m_timer <= 0.0f)
		{
			m_state = Emit;
			m_timer = m_emitTimer;
		}
		break;
	case Emit:
		m_timer -= g_gameTime->GetFrameDeltaTime();
		if (m_timer <= 0.0f)
		{
			m_state = Wait;
			m_timer = m_waitTimer;
		}
		break;
	case Capture:
		break;
	}
	// 状態に応じてフラグ更新
	m_isEmitting = (m_state == Emit || m_state == Capture);

	/** Emitに入った瞬間にエフェクトを再生する*/
	if (!m_prevIsEmitting && m_isEmitting)
	{
		PlayLightEffect();
	}

	m_prevIsEmitting = m_isEmitting;

}

void CowCaptureController::CountSpriteUI()
{
	/** 光が出ているときは表示しない*/
	if (m_isEmitting)
	{
		/** 光が出ているときはフォントを表示しない状態にする*/
		m_currentCount = -1;
		return;
	}
	/** 残り秒数を計算(5→1)*/
	int seconds = static_cast<int>(std::ceilf(m_timer));

	/** 残り秒数が変わったらスプライトを更新*/
	if (seconds <= 0 || seconds > 5)
	{
		m_currentCount = -1;
		return;
	}
	/**画像配列用に変換*/
	m_currentCount = seconds - 1;

	/** 表示位置*/
	Vector3 pos;
	pos.x = 900.0f;
	pos.y = 513.0f;
	pos.z = 0.0f;
	m_LightApperNumberSpriteRender[m_currentCount].SetPosition(pos);
}

void CowCaptureController::PlayLightEffect()
{
	if (!m_ufo) return;

	auto e = NewGO<nsK2EngineLow::EffectEmitter>(0);
	e->Init((int)EffectID::EffectID_UFOLight);

	/**  UFO の真下に出す */
	Vector3 pos = m_ufo->GetPosition();
	pos.y -= 50.0f;
	e->SetPosition(pos);

	/** UFO のライトは大きい方が見える */
	e->SetScale({ 5.0f, 5.0f, 5.0f });

	e->Play();
}

void CowCaptureController::Render(RenderContext& rc)
{
	if (m_pause == nullptr || m_countdown == nullptr)
	{
		return;
	}

	if (m_countdown->GetCountDown())
	{
		return;
	}
	/** Pause中は描画を止める*/
	if (m_pause->GetIsPause())
	{
		return;
	}

	if (m_ufo == nullptr)
	{
		return;
	}
	/** 光が出ていないときだけカウント*/
	if (!m_isEmitting)
	{
		/** 光の発射までのスプライト描画*/
		m_LightApperSpriteRender.Draw(rc);
		/** 秒のスプライト描画*/
		m_secondsSpriteRender.Draw(rc);
	}
	
	

	///** 光が出ているときもしくは、UFOが牛を捕まえているときのみ描画*/
	if (m_isEmitting || m_ufo->GetIsCowTakeAwayed())
	{
		//m_ufocontrollermodelRender.Draw(rc);
	}
	/** 数字スプライトの描画*/
	if (m_currentCount >= 0 && !m_isEmitting)
	{
		m_LightApperNumberSpriteRender[m_currentCount].Draw(rc);
	}
	
}
	
