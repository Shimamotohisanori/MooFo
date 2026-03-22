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
	
	const Vector2 NUMBER_SPRITE_SIZE = Vector2(100.0f, 120.0f);
	const Vector2 RESCUE_SPRITE_SIZE = Vector2(350.0f, 320.0f);

	const Vector3 RESCUE_SPRITE_POSITION = Vector3(-780.0f, -400.0f, 0.0f);
	const Vector3 NUMBER_SPRITE_TENS_POSITION = Vector3(-560.0f, -400.0f, 0.0f);
	const Vector3 NUMBER_SPRITE_ONES_POSITION = Vector3(-480.0f, -400.0f, 0.0f);
	const Vector3 NUMBER_SPRITE_ONES_BELOW_POSITION = Vector3(-560.0f, -400.0f, 0.0f);
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
    return true;
}

void CowNumberOfRescues::Update()
{
	tens = m_numberOfRescues / 10;
	ones = m_numberOfRescues % 10;
}

void CowNumberOfRescues::Render(RenderContext& renderContext)
{
	/** 救出画像の描画 */
	m_rescueSprite.SetPosition(RESCUE_SPRITE_POSITION);
	m_rescueSprite.Update();
	m_rescueSprite.Draw(renderContext);

	/** 救出数が10を超えていたら */
	if (m_numberOfRescues >= 10)
	{
		/** 10の位の描画 */	
		m_tensSprite[tens].SetPosition(NUMBER_SPRITE_TENS_POSITION);
		m_tensSprite[tens].Update();
		m_tensSprite[tens].Draw(renderContext);

		/** 1の位の描画 */
		m_onesSprite[ones].SetPosition(NUMBER_SPRITE_ONES_POSITION);
		m_onesSprite[ones].Update();
		m_onesSprite[ones].Draw(renderContext);
	}

	else
	{
		/** 1の位の描画 */
		m_onesSprite[ones].SetPosition(NUMBER_SPRITE_ONES_BELOW_POSITION);
		m_onesSprite[ones].Update();
		m_onesSprite[ones].Draw(renderContext);
	}
	
}
