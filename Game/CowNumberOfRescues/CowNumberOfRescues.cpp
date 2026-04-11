#include "stdafx.h"
#include "CowNumberOfRescues.h"

namespace
{
    const char* COUNT_NUMBER_FILEPATH = "Assets/sprite/NumberUI/";
    const char* COUNT_NUMBER_FORMAT = ".dds";
    const char* COUNT_NUMBER_FILENAME_LIST[10] =
    {
        "MooFoNumberUI0",
		"MooFoNumberUI1",
		"MooFoNumberUI2",
		"MooFoNumberUI3",
		"MooFoNumberUI4",
		"MooFoNumberUI5",
		"MooFoNumberUI6",
		"MooFoNumberUI7",
		"MooFoNumberUI8",
		"MooFoNumberUI9"
	};

	const char* RESCUE_FILEPATH = "Assets/sprite/CowNumberOfRescuesUI/CowNumberOfRescues.DDS";
	const char* SLASH_FILEPATH = "Assets/sprite/CowNumberOfRescuesUI/slash.DDS";

	const Vector2 NUMBER_SPRITE_SIZE = Vector2(50.0f, 70.0f);
	const Vector2 RESCUE_SPRITE_SIZE = Vector2(250.0f, 220.0f);
	const Vector2 SLASH_SPRITE_SIZE = Vector2(100.0f, 120.0f);

	const Vector3 RESCUE_SPRITE_POSITION = Vector3(-820.0f, -360.0f, 0.0f);
	const Vector3 NUMBER_SPRITE_TENS_POSITION = Vector3(-670.0f, -360.0f, 0.0f);
	const Vector3 NUMBER_SPRITE_ONES_POSITION = Vector3(-630.0f, -360.0f, 0.0f);
	const Vector3 NUMBER_SPRITE_ONES_TENBELOW_POSITION = Vector3(-670.0f, -360.0f, 0.0f);
	const Vector3 SLASH_SPRITE_POSITION = Vector3(-580.0f, -380.0f, 0.0f);
	const Vector3 SLASH_SPRITE_TENBELOW_POSITION = Vector3(-600.0f, -380.0f, 0.0f);
	const Vector3 ONE_SPRITE_POSITION = Vector3(-540.0f, -390.0f, 0.0f);
	const Vector3 FIVE_SPRITE_POSITION = Vector3(-500.0f, -390.0f, 0.0f);
}

bool CowNumberOfRescues::Start()
{
	/** 数のUIを左右全てに読み込む */
	for (int i = 0; i < 10; i++)
    {
		m_filePath[i] = std::string(COUNT_NUMBER_FILEPATH) + COUNT_NUMBER_FILENAME_LIST[i] + COUNT_NUMBER_FORMAT;

		m_tensSprite[i].Init(m_filePath[i].c_str(), NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
		m_onesSprite[i].Init(m_filePath[i].c_str(), NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
    }

	m_rescueSprite.Init(RESCUE_FILEPATH, RESCUE_SPRITE_SIZE.x,RESCUE_SPRITE_SIZE.y);

	m_slashSprite.Init(SLASH_FILEPATH, SLASH_SPRITE_SIZE.x, SLASH_SPRITE_SIZE.y);

	m_oneSprite.Init(m_filePath[1].c_str(), NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
	m_oneSprite.SetPosition(ONE_SPRITE_POSITION);
	m_oneSprite.Update();

	m_fiveSprite.Init(m_filePath[5].c_str(), NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
	m_fiveSprite.SetPosition(FIVE_SPRITE_POSITION);
	m_fiveSprite.Update();

    return true;
}

void CowNumberOfRescues::Update()
{
	tens = m_numberOfRescues / 10;
	ones = m_numberOfRescues % 10;
}

int CowNumberOfRescues::GetNumberOfRescues() const
{
	return m_numberOfRescues;
}

void CowNumberOfRescues::SetResult(bool isResult)
{
	m_isResult = isResult;
}

void CowNumberOfRescues::Render(RenderContext& renderContext)
{
    Vector3 rescuePos;
    Vector3 tensPos;
    Vector3 onesPos;
    Vector3 slashPos;
    Vector3 onePos;
    Vector3 fivePos;

    if (m_isResult)
    {
		if (m_resultType == ResultType::GameClear)
        {
            //ゲームクリア
            rescuePos = Vector3(-520.0f,-10.0f, 0.0f);
            tensPos = Vector3(-420.0f, -10.0f, 0.0f);
            onesPos = Vector3(-370.0f, -10.0f, 0.0f);
            slashPos = Vector3(-320.0f, -10.0f, 0.0f);
            onePos = Vector3(-270.0f, -10.0f, 0.0f);
            fivePos = Vector3(-230.0f, -10.0f, 0.0f);
        }
        else
        {
            //ゲームオーバー
            rescuePos = Vector3(-600.0f, 50.0f, 0.0f);
            tensPos = Vector3(-500.0f, 50.0f, 0.0f);
            onesPos = Vector3(-450.0f, 50.0f, 0.0f);
            slashPos = Vector3(-400.0f, 50.0f, 0.0f);
            onePos = Vector3(-350.0f, 50.0f, 0.0f);
            fivePos = Vector3(-310.0f, 50.0f, 0.0f);
        }
       
    }
    else
    {
        //ゲーム中
        rescuePos = RESCUE_SPRITE_POSITION;
        tensPos = NUMBER_SPRITE_TENS_POSITION;
        onesPos = NUMBER_SPRITE_ONES_POSITION;
        slashPos = SLASH_SPRITE_POSITION;
        onePos = ONE_SPRITE_POSITION;
        fivePos = FIVE_SPRITE_POSITION;
    }

    // ------------------------
    // 救出アイコン
    // ------------------------
    m_rescueSprite.SetPosition(rescuePos);
    m_rescueSprite.Update();
    m_rescueSprite.Draw(renderContext);

    // ------------------------
    // 現在の救出数
    // ------------------------
    if (m_numberOfRescues >= 10)
    {
        // 十の位
        m_tensSprite[tens].SetPosition(tensPos);
        m_tensSprite[tens].Update();
        m_tensSprite[tens].Draw(renderContext);
    }

    // 一の位
    m_onesSprite[ones].SetPosition(onesPos);
    m_onesSprite[ones].Update();
    m_onesSprite[ones].Draw(renderContext);
    //「/」
    m_slashSprite.SetPosition(slashPos);
    m_slashSprite.Update();
    m_slashSprite.Draw(renderContext);

    // 最大値（1 / 5）
    m_oneSprite.SetPosition(onePos);
    m_oneSprite.Update();
    m_oneSprite.Draw(renderContext);

    m_fiveSprite.SetPosition(fivePos);
    m_fiveSprite.Update();
    m_fiveSprite.Draw(renderContext);
}