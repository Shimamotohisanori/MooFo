#include "stdafx.h"
#include "Map.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "GameScene/Game.h"
#include "Source/Actor/Character/UFO/UFO.h"
#include "Pause/Pause.h"
#include"GameScene/LoadingScene.h"
namespace
{
	/** ミニマップのスプライトのパス */
	const char* MAP_SPRITE_PATH = "Assets/sprite/MapUI/MapIcon.dds";

	/** プレイヤーのアイコンのファイルパス */
	const char* PLAYER_ICON_PATH = "Assets/sprite/MapUI/PlayerIcon.dds";

	/** 牛のアイコンのファイルパス */
	const char* COW_ICON_PATH = "Assets/sprite/MapUI/CowIcon.dds";

	/** UFOのアイコンのファイルパス */
	const char* UFO_ICON_PATH = "Assets/sprite/MapUI/UFOIcon.dds";

	/** ビックリマークのファイルパス */
	const char* DANGER_ICON_PATH = "Assets/sprite/MapUI/Danger.dds";

	/** ミニマップの外枠のファイルパス */
	const char* OUTLINE_ICON_PATH = "Assets/sprite/MapUI/OutLine.dds";

	Vector3 MAP_CENTER_POSITION = Vector3(704.0f, -300.0f, 0.0f);

	Vector3 MAP_OUTLINE_POSITION = Vector3(704.0f, -302.5f, 0.0f);


	constexpr float MAP_RADIUS = 180.0f;
	constexpr float LIMITED_RANGE_IMAGE = 400.0f;

	/** マジックナンバー処理 */
	constexpr int COW_NUM = 10;
	constexpr int UFO_NUM = 4;
}

bool Map::Start()
{
	m_pause = FindGO<Pause>("pause");

	/** ミニマップの背景 */
	m_mapSprite.Init(MAP_SPRITE_PATH, 400.0f, 400.0f);
	m_mapSprite.SetPosition(MAP_CENTER_POSITION);

	/** ミニマップの中心(プレイヤー) */
	m_playerSprite.Init(PLAYER_ICON_PATH, 50.0f, 50.0f);
	m_playerSprite.SetPosition(MAP_CENTER_POSITION);

	/** ミニマップの外枠 */
	m_outLineSprite.Init(OUTLINE_ICON_PATH, 532.0f, 532.0f);
	m_outLineSprite.SetPosition(MAP_OUTLINE_POSITION);

	/** 牛をミニマップ内に出現させる。 */
	for (int i = 0; i < COW_NUM; i++)
	{
		m_cowSprite[i].Init(COW_ICON_PATH, 25.0f, 25.0f);
	}

	/** UFOをミニマップ内に出現させる。 */
	for (int i = 0; i < UFO_NUM; i++)
	{
		m_ufoSprite[i].Init(UFO_ICON_PATH, 50.0f, 50.0f);
	}

	/* ビックリマークをUFOが牛を捕まえたときに表示させる。
	 * UFOが起点となるためUFO_NUMを使用する
	 */
	for (int i = 0; i < UFO_NUM; i++)
	{
		m_dangerSprite[i].Init(DANGER_ICON_PATH, 30.0f, 30.0f);
	}


	/** それぞれのポジションを見つける。*/
	//m_cows = FindGOs<Cow>("cow");
	m_ufos = FindGOs<UFO>("UFO");
	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");
	return true;
}
void Map::Update()
{
	/** ゲームオブジェクトが見つからなかったら処理を行わない。 */
	if (m_game == nullptr || m_player == nullptr) return;
	/** 毎フレームUFOのリストを更新する */
	m_ufos = m_game->GetUFOs();
	/** フラッシュタイマーの更新 */
	m_flashTImer += g_gameTime->GetFrameDeltaTime();

	/** 生きている牛のリストを取得する。 */
	auto& cows = m_game->GetAliveCows();

	/** プレイヤーのワールド座標を取得する。 */
	Vector3 playerPos = m_player->GetPosition();

	/** カメラがどの方向を向いているか取得する。 */
	Vector3 forward = g_camera3D->GetForward();

	/*
	 *  カメラの向きを角度に変換する。
	 *  atan2はXとYの方向がどの角度かを返す関数
	 */
	m_mapAngle = atan2(-forward.x, forward.z);

	/** 全フラグをリセット */
	for (int i = 0; i < COW_NUM; i++)
	{
		m_isCowImage[i] = false;
	}

	/** 牛のアイコン */
	/** 牛の数が牛のアイコンの数より多い場合
	牛のアイコンの数までしか処理を行わない。 */
	int cowCount = min((int)cows.size(), COW_NUM);
	for (int i = 0; i < cowCount; i++)
	{
		/** 死んでいる牛とUFOに連れて行かれた牛はマップに表示させない。 */
		if (cows[i]->IsDead()) continue;
		if (cows[i]->GetIsTakeAwayed()) continue;

		/** 牛のワールド座標を取得する。 */
		Vector3 cowPos = cows[i]->GetPosition();
		Vector3 mapPos;

		/** マップに表示する範囲に牛やUFOがいたら */
		if (WorldPositionConvertToMapPosition(playerPos, cowPos, mapPos))
		{
			/** マップに表示するように設定する。 */
			m_isCowImage[i] = true;

			/** SpriteRenderに座標を設定 */
			m_cowSprite[i].SetPosition(mapPos);
		}
	}

	/** UFOのアイコン */
	for (int i = 0; i < (int)m_ufos.size(); i++)
	{
		/** UFOが牛を吸い込んだら */
		if (m_ufos[i]->GetIsCowTakeAwayed())
		{
			/** UFOの描画を消す */
			m_isUFOImage[i] = false;

			/** 代わりにビックリマークを描画させる */
			Vector3 pos = m_ufos[i]->GetPosition();
			Vector3 mapPos;

			/** マップに表示する範囲に牛やUFOがいたら */
			if (WorldPositionConvertToMapPosition(playerPos, pos, mapPos))
			{
				/** マップに表示するように設定する。 */
				m_dangerSprite[i].SetPosition(mapPos);

				/** ビックリマークを描画させる。 */
				m_isdanger[i] = true;
			}
			else
			{
				/** そうじゃなかったら描画しない。 */
				m_isdanger[i] = false;
			}
			continue;
		}

		/** UFOが牛を吸い込んでいない場合通常のUFOを描画させる。 */
		Vector3 ufoPos = m_ufos[i]->GetPosition();
		Vector3 mapPos;

		/** ミニマップ内にUFOがいたら */
		if (WorldPositionConvertToMapPosition(playerPos, ufoPos, mapPos))
		{
			/** ミニマップ内にUFOと座標をセットする。 */
			m_isUFOImage[i] = true;
			m_ufoSprite[i].SetPosition(mapPos);
		}
		else
		{
			/** そうじゃなかったら描画しない。 */
			m_isUFOImage[i] = false;
		}

		/* ビックリマークは描画しない。 */
		m_isdanger[i] = false;
	}

	/** 描画更新処理 */
	m_mapSprite.Update();
	m_playerSprite.Update();

	for (int i = 0; i < COW_NUM; i++)
	{
		m_cowSprite[i].Update();
	}
	for (int i = 0; i < UFO_NUM; i++)
	{
		m_ufoSprite[i].Update();
		m_dangerSprite[i].Update();
	}

	m_outLineSprite.Update();


}
bool Map::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 cowPosition, Vector3& mapPosition)
{
	/** Y座標はマップの座標とは関係ないので、0にする。 */
	worldCenterPosition.y = 0.0f;
	cowPosition.y = 0.0f;
	Vector3 cowDiff = cowPosition - worldCenterPosition;
	/** マップの中心のプレイヤーとの距離が一定以上離れていたら */
	if (cowDiff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE)
	{
		/** 表示しないようにする。 */
		return false;
	}

	/** ベクトルの長さを取得 */
	float cowLength = cowDiff.Length();

	Quaternion rot;

	/** Y軸周りにマップを回転させるクォータニオンを作っている。 */
	rot.SetRotationY(m_mapAngle);

	/** ベクトルに向かう。 */

	rot.Apply(cowDiff);

	/** ベクトルを正規化する。 */
	cowDiff.Normalize();

	/** マップの大きさ/距離制限で。	ベクトルをマップ座標系に変換する。*/
	cowDiff *= cowLength * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	/** マップの中央座標と上記ベクトルを加算する。 */
	mapPosition = Vector3(MAP_CENTER_POSITION.x + cowDiff.x, MAP_CENTER_POSITION.y + cowDiff.z, 0.0f);


	return true;
}
void Map::Render(RenderContext& rc)
{
	/**フェード完了までUIの表示を遅らす*/
	LoadingScene* lodingScene = FindGO<LoadingScene>("loading");
	if (lodingScene != nullptr && !lodingScene->GetLoadingEnd())
	{
		return;
	}
	/** タイムアウト時のフェード処理中は描画を止める*/
	if (m_game->IsFadeTimeOut())
	{
		return;
	}

	if (m_pause == nullptr) return;
	if (m_game == nullptr) return;
	if (m_pause->GetIsPause()) return;

	auto& cows = m_game->GetAliveCows();

	m_mapSprite.SetMulColor(Vector4{ 1.0f, 1.0f, 1.0f, 0.7f });
	m_mapSprite.Draw(rc);
	m_playerSprite.Draw(rc);

	/** 牛アイコンの描画 */
	int cowCount = min((int)cows.size(), COW_NUM);
	for (int i = 0; i < cowCount; i++)
	{
		if (m_isCowImage[i])
		{
			m_cowSprite[i].Draw(rc);
		}
	}

	/** UFOアイコンの描画 */
	for (int i = 0; i < (int)m_ufos.size(); i++)
	{
		if (m_isUFOImage[i])
		{
			m_ufoSprite[i].Draw(rc);
		}
	}

	/** ビックリマークの描画 */
	float flash = (sinf(m_flashTImer * 4.0f) + 1.0f) * 0.5f;
	for (int i = 0; i < (int)m_ufos.size(); i++)
	{
		if (m_isdanger[i])
		{
			m_dangerSprite[i].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, flash));
			m_dangerSprite[i].Draw(rc);
		}
	}

	m_outLineSprite.Draw(rc);
}