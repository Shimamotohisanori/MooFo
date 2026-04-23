#include "stdafx.h"
#include "Map.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "Source/Actor/Character/UFO/UFO.h"
namespace
{
	Vector3 MAP_CENTER_POSITION = Vector3(704.0f, -300.0f, 0.0f);
	
	float MAP_RADIUS = 190.0f;
	float LIMITED_RANGE_IMAGE = 410.0f;


	/** マジックナンバー処理 */
	const int COW_NUM = 10;
	const int UFO_NUM = 4;
}

bool Map::Start()
{
	/** ミニマップの背景 */	
	m_mapSprite.Init("Assets/sprite/MapUI/MapIcon.dds", 400.0f, 400.0f);
	m_mapSprite.SetPosition(MAP_CENTER_POSITION);

	/** ミニマップの中心(プレイヤー) */	
	m_playerSprite.Init("Assets/sprite/MapUI/PlayerIcon.dds", 50.0f, 50.0f);
	m_playerSprite.SetPosition(MAP_CENTER_POSITION);

	/** 牛をミニマップ内に出現させる。 */
	for (int i = 0; i < COW_NUM; i++)
	{
		m_cowSprite[i].Init("Assets/sprite/MapUI/CowIcon.dds", 25.0f, 25.0f);
	}

	/** UFOをミニマップ内に出現させる。 */
	for(int i = 0; i < UFO_NUM; i++)
	{
		m_ufoSprite[i].Init("Assets/sprite/MapUI/UFOIcon.dds", 30.0f, 30.0f);
	}
	

	/** それぞれのポジションを見つける。*/
	m_cows = FindGOs<Cow>("cow");
	m_ufos = FindGOs<UFO>("UFO");
	m_player = FindGO<Player>("player");
	
	return true;
}
void Map::Update()
{
	/** それぞれのポジションを代入させる。 */	
	Vector3 playerPos = m_player->GetPosition();

	for (int i = 0; i < m_cows.size(); i++)
	{
		Vector3 cowPos = m_cows[i]->GetPosition();
		Vector3 mapPos;

		/** マップに表示する範囲に牛やUFOがいたら */
		if (WorldPositionConvertToMapPosition(playerPos, cowPos, mapPos))
		{
			/** マップに表示するように設定する。 */
			m_isCowImage[i] = true;

			/** SpriteRenderに座標を設定 */
			m_cowSprite[i].SetPosition(mapPos);
		}

		/** マップに表示する範囲に敵がいなかったら */
		else
		{
			m_isCowImage[i] = false;
		}
	}

	for (int i = 0; i < m_ufos.size(); i++)
	{
		Vector3 ufoPos = m_ufos[i]->GetPosition();
		Vector3 mapPos;

		/** マップに表示する範囲に牛やUFOがいたら */
		if (WorldPositionConvertToMapPosition(playerPos, ufoPos, mapPos))
		{
			/** マップに表示するように設定する。 */
			m_isUFOImage[i] = true;

			/** SpriteRenderに座標を設定 */
			m_ufoSprite[i].SetPosition(mapPos);
		}

		/** マップに表示する範囲に敵がいなかったら */
		else
		{
			m_isUFOImage[i] = false;
		}
	}

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

	/** カメラの前方向ベクトルから、クォータニオンを生成 */	
	Vector3 forward = g_camera3D->GetForward();

	Quaternion cowrot;

	cowrot.SetRotationDegY(atan2(-forward.x, forward.z));

	/** ベクトルに向かう。 */	
	cowrot.Apply(cowDiff);

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
	m_mapSprite.Draw(rc);
	m_playerSprite.Draw(rc);

	/** 牛を描画させる */
	for (int i = 0; i < m_cows.size(); i++)
	{
		/** もしミニマップないに牛がいたら(true) */
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
}

