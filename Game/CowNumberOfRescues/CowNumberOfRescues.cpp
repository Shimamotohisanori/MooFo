#include "stdafx.h"
#include "CowNumberOfRescues.h"
#include "GameScene/Game.h"
#include "GameScene/LoadingScene.h"
#include"CowLivesUI/CowLivesUI.h"
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

    /** 赤い数のUIのファイルパス */
    const char* RED_COUNT_NUMBER_FILEPATH = "Assets/sprite/NumberUI/";
    const char* RED_COUNT_NUMBER_FORMAT = ".dds";
    const char* RED_COUNT_NUMBER_FILENAME_LIST[10] =
    {
        "Red_MooFoNumberUI0",
        "Red_MooFoNumberUI1",
        "Red_MooFoNumberUI2",
        "Red_MooFoNumberUI3",
        "Red_MooFoNumberUI4",
        "Red_MooFoNumberUI5",
        "Red_MooFoNumberUI6",
        "Red_MooFoNumberUI7",
        "Red_MooFoNumberUI8",
        "Red_MooFoNumberUI9"
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
    const Vector3 INGAME_NUMBER_SPRITE_TENS_POSITION = Vector3(-680.0f, -360.0f, 0.0f);
    const Vector3 INGAME_NUMBER_SPRITE_ONES_POSITION = Vector3(-630.0f, -360.0f, 0.0f);
    const Vector3 INGAME_NUMBER_SPRITE_ONES_TENBELOW_POSITION = Vector3(-670.0f, -360.0f, 0.0f);
    const Vector3 INGAME_SLASH_SPRITE_POSITION = Vector3(-590.0f, -365.0f, 0.0f);
    const Vector3 INGAME_SLASH_SPRITE_TENBELOW_POSITION = Vector3(-590.0f, -365.0f, 0.0f);
    const Vector3 INGAME_ONE_SPRITE_POSITION = Vector3(-540.0f, -360.0f, 0.0f);
    const Vector3 INGAME_ZERO_SPRITE_POSITION = Vector3(-500.0f, -360.0f, 0.0f);

    /** ゲームオーバースプライトのスケール */
    const Vector3 GAMEOVER_SPRITE_SCALE = Vector3(1.5f, 1.5f, 1.0f);

    /** ゲームオーバースプライトの座標 */
    const Vector3 GAMEOVER_RESCUE_SPRITE_POSITION = Vector3(-750.0f, 50.0f, 0.0f);
    const Vector3 GAMEOVER_NUMBER_SPRITE_TENS_POSITION = Vector3(-520.0f, 50.0f, 0.0f);
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

    /** 赤い数のUIを読み込む */
    for (int i = 0; i < 10; i++)
    {
        m_redFilePath[i] = std::string(RED_COUNT_NUMBER_FILEPATH) + RED_COUNT_NUMBER_FILENAME_LIST[i] + COUNT_NUMBER_FORMAT;

        m_redTensSprite[i].Init(m_redFilePath[i].c_str(), NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
        m_redOnesSprite[i].Init(m_redFilePath[i].c_str(), NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
    }

    m_rescueSprite.Init(RESCUE_FILEPATH, RESCUE_SPRITE_SIZE.x, RESCUE_SPRITE_SIZE.y);

    m_slashSprite.Init(SLASH_FILEPATH, SLASH_SPRITE_SIZE.x, SLASH_SPRITE_SIZE.y);

    m_oneSprite.Init(m_filePath[1].c_str(), NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
    m_oneSprite.SetPosition(INGAME_ONE_SPRITE_POSITION);
    m_oneSprite.Update();

    m_zeroSprite.Init(m_filePath[0].c_str(), NUMBER_SPRITE_SIZE.x, NUMBER_SPRITE_SIZE.y);
    m_zeroSprite.SetPosition(INGAME_ZERO_SPRITE_POSITION);
    m_zeroSprite.Update();

	m_game = FindGO<Game>("game");

    return true;
}

void CowNumberOfRescues::Update()
{
	/** ゲーム中の牛の残機UIのポインタを取得 */
    if(!m_cowLivesUI)
    {
        m_cowLivesUI = FindGO<CowLivesUI>("cowlivesui");
        return;
    }

    tens = m_numberOfRescues / 10;
    ones = m_numberOfRescues % 10;

    InitLayout();

    BounceAnimation();
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
    m_rescuePos[InGame] = INGAME_RESCUE_SPRITE_POSITION;
    m_tensPos[InGame] = INGAME_NUMBER_SPRITE_TENS_POSITION;
    m_onesPos[InGame] = INGAME_NUMBER_SPRITE_ONES_POSITION;
    m_slashPos[InGame] = INGAME_SLASH_SPRITE_POSITION;
    m_onePos[InGame] = INGAME_ONE_SPRITE_POSITION;
    m_zeroPos[InGame] = INGAME_ZERO_SPRITE_POSITION;

    m_rescueScale[InGame] = INGAME_SPRITE_SCALE;
    m_tensScale[InGame] = INGAME_SPRITE_SCALE;
    m_onesScale[InGame] = INGAME_SPRITE_SCALE;
    m_slashScale[InGame] = INGAME_SPRITE_SCALE;
    m_oneScale[InGame] = INGAME_SPRITE_SCALE;
    m_zeroScale[InGame] = INGAME_SPRITE_SCALE;

    /** クリア */
    m_rescuePos[GameClear] = GAMECLEAR_RESCUE_SPRITE_POSITION;
    m_tensPos[GameClear] = GAMECLEAR_NUMBER_SPRITE_TENS_POSITION;
    m_onesPos[GameClear] = GAMECLEAR_NUMBER_SPRITE_ONES_POSITION;
    m_slashPos[GameClear] = GAMECLEAR_SLASH_SPRITE_POSITION;
    m_onePos[GameClear] = GAMECLEAR_ONE_SPRITE_POSITION;
    m_zeroPos[GameClear] = GAMECLEAR_ZERO_SPRITE_POSITION;

    m_rescueScale[GameClear] = GAMECLEAR_SPRITE_SCALE;
    m_tensScale[GameClear] = GAMECLEAR_SPRITE_SCALE;
    m_onesScale[GameClear] = GAMECLEAR_SPRITE_SCALE;
    m_slashScale[GameClear] = GAMECLEAR_SPRITE_SCALE;
    m_oneScale[GameClear] = GAMECLEAR_SPRITE_SCALE;
    m_zeroScale[GameClear] = GAMECLEAR_SPRITE_SCALE;

    /** オーバー */
    m_rescuePos[GameOver] = GAMEOVER_RESCUE_SPRITE_POSITION;
    m_tensPos[GameOver] = GAMEOVER_NUMBER_SPRITE_TENS_POSITION;
    m_onesPos[GameOver] = GAMEOVER_NUMBER_SPRITE_ONES_POSITION;
    m_slashPos[GameOver] = GAMEOVER_SLASH_SPRITE_POSITION;
    m_onePos[GameOver] = GAMEOVER_ONE_SPRITE_POSITION;
    m_zeroPos[GameOver] = GAMEOVER_ZERO_SPRITE_POSITION;

    m_rescueScale[GameOver] = GAMEOVER_SPRITE_SCALE;
    m_tensScale[GameOver] = GAMEOVER_SPRITE_SCALE;
    m_onesScale[GameOver] = GAMEOVER_SPRITE_SCALE;
    m_slashScale[GameOver] = GAMEOVER_SPRITE_SCALE;
    m_oneScale[GameOver] = GAMEOVER_SPRITE_SCALE;
    m_zeroScale[GameOver] = GAMEOVER_SPRITE_SCALE;
}

void CowNumberOfRescues::BounceAnimation()
{
    /** 救出数が変化した場合 */
    if (m_oldNumberOfRescues != m_numberOfRescues)
    {
        /** バウンドの値を設定 */
        m_bounceValue.y = 30.0f;

        /** 古い救出数を更新 */
        m_oldNumberOfRescues = m_numberOfRescues;

        /** 数字がバウンドしたフラグを立てる */
        m_numberBound = true;
    }

    /** 数字がバウンドしている場合 */
    if (m_numberBound)
    {
        m_bounceValue.Lerp(0.25f, m_bounceValue, Vector3::Zero);
        /** 救出数のUIの座標にバウンドの値を加算 */
        for (int i = 0; i < LayoutTypeNum; i++)
        {
            m_tensPos[i].y += m_bounceValue.y;
            m_onesPos[i].y += m_bounceValue.y;
        }
        /** バウンドの値が0.1以下になったらバウンド終了 */
        if (m_bounceValue.y <= 0.1f)
        {
            m_bounceValue = Vector3::Zero;
            m_numberBound = false;
        }
    }
}

void CowNumberOfRescues::Render(RenderContext& renderContext)
{
    /**フェード完了までUIの表示を遅らす*/
    LoadingScene* lodingScene = FindGO<LoadingScene>("loading");
    if (lodingScene != nullptr && !lodingScene->GetLoadingEnd())
    {
        return;
    }

    /** タイムアウトしている場合は更新しない */
    if (!m_isResult)
    {
        /** ゲームオブジェクトマネージャーからゲームオブジェクトを取得 */
        if (m_game == nullptr || m_game->IsFadeTimeOut())
        {
            return;
        }
    }
    /** フェード処理中は描画しない*/
    if (m_cowLivesUI && m_cowLivesUI->IsFadeInComplete())
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
    m_rescueSprite.SetPosition(m_rescuePos[layoutType]);
    m_rescueSprite.SetScale(m_rescueScale[layoutType]);
    m_rescueSprite.Update();
    m_rescueSprite.Draw(renderContext);

    /** 現在の救出数が10より大きいなら */
    if (m_numberOfRescues > 10)
    {
        /** 赤い十の位用のスプライトの表示 */
        /** 位置は通常の十の位の位置 */
        m_redTensSprite[tens].SetPosition(m_tensPos[layoutType]);
        m_redTensSprite[tens].SetScale(m_tensScale[layoutType]);
        m_redTensSprite[tens].Update();
        m_redTensSprite[tens].Draw(renderContext);
    }

    if (m_numberOfRescues == 10)
    {
        /** 十の位用のスプライトの表示 */
        m_tensSprite[tens].SetPosition(m_tensPos[layoutType]);
        m_tensSprite[tens].SetScale(m_tensScale[layoutType]);
        m_tensSprite[tens].SetMulColor(
            Vector4(
                1.0f,
                1.0f,
                1.0f,
                1.0f)
        );
        m_tensSprite[tens].Update();
        m_tensSprite[tens].Draw(renderContext);
    }

    if (m_numberOfRescues > 10)
    {
        /** 赤い一の位用のスプライトの表示 */
        /** 位置は通常の一の位の位置 */
        m_redOnesSprite[ones].SetPosition(m_onesPos[layoutType]);
        m_redOnesSprite[ones].SetScale(m_onesScale[layoutType]);
        m_redOnesSprite[ones].Update();
        m_redOnesSprite[ones].Draw(renderContext);
    }

    else
    {
        /** 一の位用のスプライトの表示 */
        m_onesSprite[ones].SetPosition(m_onesPos[layoutType]);
        m_onesSprite[ones].SetScale(m_onesScale[layoutType]);
        m_onesSprite[ones].SetMulColor(
            Vector4(
                1.0f,
                1.0f,
                1.0f,
                1.0f)
        );
        m_onesSprite[ones].Update();
        m_onesSprite[ones].Draw(renderContext);
    }

    /** 10以下の場合はスラッシュと最大値を表示 */
    if (m_numberOfRescues <= 10)
    {
        /** スラッシュの表示 */
        m_slashSprite.SetPosition(m_slashPos[layoutType]);
        m_slashSprite.SetScale(m_slashScale[layoutType]);
        m_slashSprite.Update();
        m_slashSprite.Draw(renderContext);

        /** 最大値（1 / 10）の表示 */
        m_oneSprite.SetPosition(m_onePos[layoutType]);
        m_oneSprite.SetScale(m_oneScale[layoutType]);
        m_oneSprite.Update();
        m_oneSprite.Draw(renderContext);

        m_zeroSprite.SetPosition(m_zeroPos[layoutType]);
        m_zeroSprite.SetScale(m_zeroScale[layoutType]);
        m_zeroSprite.Update();
        m_zeroSprite.Draw(renderContext);
    }

}