#include "stdafx.h"
#include "Map.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "Source/Actor/Character/UFO/UFO.h"
#include "Pause/Pause.h"
namespace
{
	/** ミニマップ画像のファイルパス */
  const char* MAP_SPRITE_PATH = "Assets/sprite/MapUI/MapIcon.dds";

	/** プレイヤーアイコンのファイルパス */
  const char* PLAYER_ICON_PATH = "Assets/sprite/MapUI/PlayerIcon.dds";

	/** 牛アイコンのファイルパス */
  const char* COW_ICON_PATH = "Assets/sprite/MapUI/CowIcon.dds";

	/** UFOアイコンのファイルパス */
  const char* UFO_ICON_PATH = "Assets/sprite/MapUI/UFOIcon.dds";

	/** 危険アイコンのファイルパス */
  const char* DANGER_ICON_PATH = "Assets/sprite/MapUI/Danger.dds";

	/** アウトラインアイコンのファイルパス */
  const char* OUTLINE_ICON_PATH = "Assets/sprite/MapUI/OutLine.dds";

	/** ミニマップの中心位置 */
  Vector3 MAP_CENTER_POSITION = Vector3(704.0f, -300.0f, 0.0f);

	/** ミニマップの外枠の位置 */
  Vector3 MAP_OUTLINE_POSITION = Vector3(704.0f, -302.5f, 0.0f);

	/** ミニマップの半径 */
  constexpr float MAP_RADIUS = 180.0f;

	/** ミニマップに表示される範囲 */
  constexpr float LIMITED_RANGE_IMAGE = 400.0f;

	/** 牛とUFOの数 */
  constexpr int COW_NUM = 10;
	constexpr int UFO_NUM = 4;
}
bool Map::Start()
{
	m_pause = FindGO<Pause>("pause");

		/** ミニマップ画像の初期化 */
  　m_mapSprite.Init(MAP_SPRITE_PATH, 400.0f, 400.0f);
		m_mapSprite.SetPosition(MAP_CENTER_POSITION);

		/** プレイヤーアイコンの初期化 */
  　m_playerSprite.Init(PLAYER_ICON_PATH, 50.0f, 50.0f);
		m_playerSprite.SetPosition(MAP_CENTER_POSITION);

		/** アウトラインアイコンの初期化 */
  　m_outLineSprite.Init(OUTLINE_ICON_PATH, 532.0f, 532.0f);
		m_outLineSprite.SetPosition(MAP_OUTLINE_POSITION);

		/** 牛アイコンの初期化 */
  　for (int i = 0; i < COW_NUM; i++)
		{
			m_cowSprite[i].Init(COW_ICON_PATH, 25.0f, 25.0f);
		}

		/** UFOアイコンの初期化 */
  　for (int i = 0; i < UFO_NUM; i++)
		{
			m_ufoSprite[i].Init(UFO_ICON_PATH, 50.0f, 50.0f);
		}

		/** 危険アイコンの初期化 */
  　for (int i = 0; i < UFO_NUM; i++)
		{
			m_dangerSprite[i].Init(DANGER_ICON_PATH, 30.0f, 30.0f);
		}
		for (int i = 0; i < UFO_NUM; i++)
		{
			m_dangerSprite[i].Init(DANGER_ICON_PATH, 30.0f, 30.0f);
		}

	/** ゲームオブジェクトの取得 */
  m_cows = FindGOs<Cow>("cow");
	m_ufos = FindGOs<UFO>("UFO");
	m_player = FindGO<Player>("player");

	return true;
　}
void Map::Update()
{
	/** プレイヤーの位置を取得 */
  Vector3 playerPos = m_player->GetPosition();

	/** カメラの前方向を取得 */
  Vector3 forward = g_camera3D->GetForward();

　/*	 
  *  カメラの前方向に基づいてマップの回転角度を計算
  *  atan2関数を使用してX軸とZ軸の角度を取得	 
  */
  m_mapAngle = atan2(-forward.x, forward.z);

	/** 牛の位置を更新 */
  for (int i = 0; i < m_cows.size(); i++)
	{
		if (m_cows[i]->GetIsTakeAwayed())
		{
			m_isCowImage[i] = false;
			continue;
		}
		Vector3 cowPos = m_cows[i]->GetPosition();
		Vector3 mapPos;

		/** 牛の位置をマップ座標に変換 */
    if (WorldPositionConvertToMapPosition(playerPos, cowPos, mapPos))
		{
			/** 牛アイコンを表示 */
      m_isCowImage[i] = true;

			/** SpriteRenderに位置を設定 */
      m_cowSprite[i].SetPosition(mapPos);
		}

		/** 牛の位置をマップ座標に変換できなかった場合 */
    else
    {
			m_isCowImage[i] = false;
		}
	}

	/** UFOの位置を更新 */
  for (int i = 0; i < m_ufos.size(); i++)
	{
		/** UFOが牛を連れ去った場合 */
    if (m_ufos[i]->GetIsCowTakeAwayed())
		{
			/** UFOの位置を非表示にする */
      m_isUFOImage[i] = false;

			/** 危険アイコンの位置を更新 */
      Vector3 Pos = m_ufos[i]->GetPosition();
			Vector3 mapPos;

			/** 危険アイコンの位置をマップ座標に変換 */
      if (WorldPositionConvertToMapPosition(playerPos, Pos, mapPos))
			{
				m_dangerSprite[i].SetPosition(mapPos);
				/** 危険アイコンを表示 */
        m_isdanger[i] = true;
			}

			else			{
				m_isdanger[i] = false;
			}

			continue;
		}

		/** UFOが牛を連れ去っていない場合 */
    Vector3 ufoPos = m_ufos[i]->GetPosition();
		Vector3 mapPos;

		/** UFOの位置をマップ座標に変換 */
    if (WorldPositionConvertToMapPosition(playerPos, ufoPos, mapPos))
		{
			/** UFOアイコンを表示 */
      m_isUFOImage[i] = true;
			m_ufoSprite[i].SetPosition(mapPos);
		}
		else
    {
			/** UFOアイコンを非表示にする */
      m_isUFOImage[i] = false;
		}

		m_isdanger[i] = false;
	}

	/** スプライトの更新 */
  m_mapSprite.Update();
	m_playerSprite.Update();
	for (int i = 0; i < COW_NUM; i++)
	{
		m_cowSprite[i].Update();
	}

	for (int i = 0; i < UFO_NUM; i++)
	{
		m_ufoSprite[i].Update();
	}

	for (int i = 0; i < UFO_NUM; i++)
	{
		m_dangerSprite[i].Update();
	}

	m_outLineSprite.Update();
}
bool Map::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 cowPosition, Vector3& mapPosition)
{
	/** Y座標を0に設定 */
  worldCenterPosition.y = 0.0f;
	cowPosition.y = 0.0f;
	Vector3 cowDiff = cowPosition - worldCenterPosition;
	/** 牛の位置がマップの範囲外かどうかを判定 */
  if (cowDiff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE)
	{
		/** 範囲外の場合はfalseを返す */
    return false;
	}

	/** 牛の距離を取得 */
  float cowLength = cowDiff.Length();

	Quaternion rot;

	/** Y軸回転のクォータニオンを作成 */
  rot.SetRotationY(m_mapAngle);

	/** 牛の位置に回転を適用 */
  rot.Apply(cowDiff);

	/** 牛の位置を正規化 */
  cowDiff.Normalize();

	/** 牛の距離をマップ座標に変換 */
  cowDiff *= cowLength * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	/** 牛の位置をマップ座標に設定 */
  mapPosition = Vector3(MAP_CENTER_POSITION.x + cowDiff.x, MAP_CENTER_POSITION.y + cowDiff.z, 0.0f);

	return true;
}
void Map::Render(RenderContext& rc)
{
	if (m_pause->GetIsPause() == false)
	{
		m_mapSprite.SetMulColor(Vector4{ 1.0f,1.0f,1.0f,0.7f });
		m_mapSprite.Draw(rc);
		m_playerSprite.Draw(rc);

		/** 牛アイコンの描画 */
    for (int i = 0; i < m_cows.size(); i++)
		{
			/** 牛アイコンを表示する場合 */
      if (m_isCowImage[i])
			{
				m_cowSprite[i].Draw(rc);
			}
		}

		for (int i = 0; i < m_ufos.size(); i++)
		{
			if (m_isUFOImage[i])
			{
				m_ufoSprite[i].Draw(rc);
			}
		}

		for (int i = 0; i < m_ufos.size(); i++)
		{
			if (m_isdanger[i])
			{
				m_dangerSprite[i].Draw(rc);
			}
		}

		m_outLineSprite.Draw(rc);
	}
}
 
 