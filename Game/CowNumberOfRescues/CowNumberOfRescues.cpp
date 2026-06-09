#include "stdafx.h"
#include "CowNumberOfRescues.h"
#include "GameScene/LoadingScene.h"
namespace
{
	/** 数のUIのファイルパス */
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

	/** 救出数のUIのファイルパス */
	const char* RESCUE_FILEPATH = "Assets/sprite/CowNumberOfRescuesUI/CowNumberOfRescues.DDS";

	/** スラッシュのUIのファイルパス */
	const char* SLASH_FILEPATH = "Assets/sprite/CowNumberOfRescuesUI/slash.DDS";

	/** 数のスプライトのサイズ */
	const Vector2 NUMBER_SPRITE_SIZE = Vector2(40.0f, 60.0f);

	/** 救出数のスプライトのサイズ */
	const Vector2 RESCUE_SPRITE_SIZE = Vector2(270.0f, 240.0f);

	/** スラッシュのスプライトのサイズ */
	const Vector2 SLASH_SPRITE_SIZE = Vector2(120.0f, 100.0f);

    /** インゲームスプライトのスケール */
	const Vector3 INGAME_SPRITE_SCALE = Vector3(1.0f, 1.0f, 1.0f);

    /** インゲームスプライトの座標 */
    const Vector3 INGAME_RESCUE_SPRITE_POSITION = Vector3(-820.0f, -360.0f, 0.0f);
    const Vector3 INGAME_NUMBER_SPRITE_TENS_POSITION = Vector3(-670.0f, -360.0f, 0.0f);
    const Vector3 INGAME_NUMBER_SPRITE_ONES_POSITION = Vector3(-635.0f, -360.0f, 0.0f);
    const Vector3 INGAME_NUMBER_SPRITE_ONES_TENBELOW_POSITION = Vector3(-670.0f, -360.0f, 0.0f);
    const Vector3 INGAME_SLASH_SPRITE_POSITION = Vector3(-590.0f, -370.0f, 0.0f);
    const Vector3 INGAME_SLASH_SPRITE_TENBELOW_POSITION = Vector3(-590.0f, -370.0f, 0.0f);
    const Vector3 INGAME_ONE_SPRITE_POSITION = Vector3(-540.0f, -370.0f, 0.0f);
    const Vector3 INGAME_ZERO_SPRITE_POSITION = Vector3(-500.0f, -370.0f, 0.0f);

    /** ゲームオーバースプライトのスケール */
	const Vector3 GAMEOVER_SPRITE_SCALE = Vector3(1.5f, 1.5f, 1.0f);

    /** ゲームオーバースプライトの座標 */
    const Vector3 GAMEOVER_RESCUE_SPRITE_POSITION = Vector3(-750.0f, 50.0f, 0.0f);
	const Vector3 GAMEOVER_NUMBER_SPRITE_TENS_POSITION = Vector3(-470.0f, 50.0f, 0.0f);
	const Vector3 GAMEOVER_NUMBER_SPRITE_ONES_POSITION = Vector3(-450.0f, 50.0f, 0.0f);
	const Vector3 GAMEOVER_SLASH_SPRITE_POSITION = Vector3(-370.0f, 50.0f, 0.0f);
	const Vector3 GAMEOVER_ONE_SPRITE_POSITION = Vector3(-310.0f, 50.0f, 0.0f);
	const Vector3 GAMEOVER_ZERO_SPRITE_POSITION = Vector3(-260.0f, 50.0f, 0.0f);

	/** ゲームクリアスプライトのスケール */
	const Vector3 GAMECLEAR_SPRITE_SCALE = Vector3(1.5f, 1.5f, 1.0f);

    /** ゲームクリアスプライトの座標 */
	const Vector3 GAMECLEAR_RESCUE_SPRITE_POSITION = Vector3(-750.0f, 50.0f, 0.0f);
	const Vector3 GAMECLEAR_NUMBER_SPRITE_TENS_POSITION = Vector3(-520.0f, 50.0f, 0.0f);
	const Vector3 GAMECLEAR_NUMBER_SPRITE_ONES_POSITION = Vector3(-450.0f, 50.0f, 0.0f);
	const Vector3 GAMECLEAR_SLASH_SPRITE_POSITION = Vector3(-370.0f, 50.0f, 0.0f);
	const Vector3 GAMECLEAR_ONE_SPRITE_POSITION = Vector3(-310.0f, 50.0f, 0.0f);
	const Vector3 GAMECLEAR_ZERO_SPRITE_POSITION = Vector3(-260.0f, 50.0f, 0.0f);

    
}

bool CowNumberOfRescues::Start()
{
    m_numberOfRescues = 0;
    tens = 0;
    ones = 0;
    m_isResult = false;

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
	m_oneSprite.SetPosition(INGAME_ONE_SPRITE_POSITION);
	m_oneSprite.Update();

	m_zeroSprite.Init(m_filePath[0].c_str(), NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
	m_zeroSprite.SetPosition(INGAME_ZERO_SPRITE_POSITION);
	m_zeroSprite.Update();

    return true;
}

void CowNumberOfRescues::Update()
{
    InitLayout();

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

void CowNumberOfRescues::InitLayout()
{
    /** ゲーム中 */
    rescuePos[InGame] = INGAME_RESCUE_SPRITE_POSITION;
    tensPos[InGame] = INGAME_NUMBER_SPRITE_TENS_POSITION;
    onesPos[InGame] = INGAME_NUMBER_SPRITE_ONES_POSITION;
    slashPos[InGame] = INGAME_SLASH_SPRITE_POSITION;
    onePos[InGame] = INGAME_ONE_SPRITE_POSITION;
    zeroPos[InGame] = INGAME_ZERO_SPRITE_POSITION;

    rescueScale[InGame] = INGAME_SPRITE_SCALE;
	tensScale[InGame] = INGAME_SPRITE_SCALE;
	onesScale[InGame] = INGAME_SPRITE_SCALE;
    slashScale[InGame] = INGAME_SPRITE_SCALE;
	oneScale[InGame] = INGAME_SPRITE_SCALE;
	zeroScale[InGame] = INGAME_SPRITE_SCALE;
    
    /** クリア */
    rescuePos[GameClear] = GAMECLEAR_RESCUE_SPRITE_POSITION;
    tensPos[GameClear] = GAMECLEAR_NUMBER_SPRITE_TENS_POSITION;
    onesPos[GameClear] = GAMECLEAR_NUMBER_SPRITE_ONES_POSITION;
    slashPos[GameClear] = GAMECLEAR_SLASH_SPRITE_POSITION;
    onePos[GameClear] = GAMECLEAR_ONE_SPRITE_POSITION;
    zeroPos[GameClear] = GAMECLEAR_ZERO_SPRITE_POSITION;

	rescueScale[GameClear] = GAMECLEAR_SPRITE_SCALE;
	tensScale[GameClear] = GAMECLEAR_SPRITE_SCALE;
    onesScale[GameClear] = GAMECLEAR_SPRITE_SCALE;
	slashScale[GameClear] = GAMECLEAR_SPRITE_SCALE;
    oneScale[GameClear] = GAMECLEAR_SPRITE_SCALE;
	zeroScale[GameClear] = GAMECLEAR_SPRITE_SCALE;
    
    /** オーバー */
    rescuePos[GameOver] = GAMEOVER_RESCUE_SPRITE_POSITION;
    tensPos[GameOver] = GAMEOVER_NUMBER_SPRITE_TENS_POSITION;
    onesPos[GameOver] = GAMEOVER_NUMBER_SPRITE_ONES_POSITION;
    slashPos[GameOver] = GAMEOVER_SLASH_SPRITE_POSITION;
    onePos[GameOver] = GAMEOVER_ONE_SPRITE_POSITION;
    zeroPos[GameOver] = GAMEOVER_ZERO_SPRITE_POSITION;

	rescueScale[GameOver] = GAMEOVER_SPRITE_SCALE;
	tensScale[GameOver] = GAMEOVER_SPRITE_SCALE;
    onesScale[GameOver] = GAMEOVER_SPRITE_SCALE;
	slashScale[GameOver] = GAMEOVER_SPRITE_SCALE;
    oneScale[GameOver] = GAMEOVER_SPRITE_SCALE;
	zeroScale[GameOver] = GAMEOVER_SPRITE_SCALE;
}

void CowNumberOfRescues::Render(RenderContext& renderContext)
{
    /**フェード完了までUIの表示を遅らす*/
    LoadingScene* lodingScene = FindGO<LoadingScene>("loading");
    if (lodingScene != nullptr && !lodingScene->GetLoadingEnd())
    {
        return;
    }
    /** レイアウトの種類 */
	LayoutType layoutType;

	/** もしリザルトに移動したら */
    if (m_isResult)
    {
        /** リザルト画面の場合
         * レイアウトをクリアに
         * そうじゃなければゲームオーバーに設定 */
		layoutType = (m_resultType == ResultType::GameClear) ? LayoutType::GameClear : LayoutType::GameOver;
    }

	/** そうじゃなければゲーム中のレイアウトに設定 */
    else
    {
		layoutType = LayoutType::InGame;
    }

    /** 救出画像の表示 */
    m_rescueSprite.SetPosition(rescuePos[layoutType]);
	m_rescueSprite.SetScale(rescueScale[layoutType]);
    m_rescueSprite.Update();
    m_rescueSprite.Draw(renderContext);

    /** 現在の救出数が10以上なら */
    if (m_numberOfRescues >= 10)
    {
        /** 十の位用のスプライトの表示 */
        m_tensSprite[tens].SetPosition(tensPos[layoutType]);
        m_tensSprite[tens].SetScale(tensScale[layoutType]);
        m_tensSprite[tens].Update();
        m_tensSprite[tens].Draw(renderContext);
    }

    /** 一の位用のスプライトの表示 */
    m_onesSprite[ones].SetPosition(onesPos[layoutType]);
    m_onesSprite[ones].SetScale(onesScale[layoutType]);
    m_onesSprite[ones].Update();
    m_onesSprite[ones].Draw(renderContext);

    /** スラッシュの表示 */
    m_slashSprite.SetPosition(slashPos[layoutType]);
    m_slashSprite.SetScale(slashScale[layoutType]);
    m_slashSprite.Update();
    m_slashSprite.Draw(renderContext);

    /** 最大値（1 / 10）の表示 */
    m_oneSprite.SetPosition(onePos[layoutType]);
    m_oneSprite.SetScale(oneScale[layoutType]);
    m_oneSprite.Update();
    m_oneSprite.Draw(renderContext);

    m_zeroSprite.SetPosition(zeroPos[layoutType]);
    m_zeroSprite.SetScale(zeroScale[layoutType]);
    m_zeroSprite.Update();
    m_zeroSprite.Draw(renderContext);
}