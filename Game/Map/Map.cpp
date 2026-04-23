#include "stdafx.h"
#include "Map.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "Source/Actor/Character/UFO/UFO.h"
namespace
{
	Vector3 MAP_CENTER_POSITION = Vector3(704.0f, 284.0f, 0.0f);
	
	float MAP_RADIUS = 240.0f;
	float LIMITED_RANGE_IMAGE = 600.0f;
}

bool Map::Start()
{
	/** ミニマップの背景 */	m_mapSprite.Init("Assets/sprite/MapUI/MapIcon.dds", 512.0f, 512.0f);
	m_mapSprite.SetPosition(MAP_CENTER_POSITION);

	/** ミニマップの中心(プレイヤー) */	m_playerSprite.Init("Assets/sprite/MapUI/PlayerIcon.dds", 50.0f, 50.0f);
	m_playerSprite.SetPosition(MAP_CENTER_POSITION);

	/** 牛をミニマップ内に出現させる。 */	m_cowSprite.Init("Assets/sprite/MapUI/CowIcon.dds", 30.0f, 30.0f);

	/** UFOをミニマップ内に出現させる。 */	m_ufoSprite.Init("Assets/sprite/MapUI/UFOIcon.dds", 30.0f, 30.0f);

	/** それぞれのポジションを見つける。*/	m_player = FindGO<Player>("player");
	m_cow = FindGO<Cow>("cow");
	m_ufo = FindGO<UFO>("ufo");

	return true;
}
void Map::Update()
{
	/** それぞれのポジションを代入させる。 */	Vector3 playerPos = m_player->GetPosition();
	Vector3 cowPos = m_cow->GetPosition();
	Vector3 ufoPos = m_ufo->GetPosition();

	Vector3 mapPos;

	/** マップに表示する範囲に牛やUFOがいたら */if (WorldPositionConvertToMapPosition(playerPos, cowPos, ufoPos, mapPos))
	{
		/** マップに表示するように設定する。 */		m_isImage = true;

		/** SpriteRenderに座標を設定 */		m_cowSprite.SetPosition(mapPos);
		m_ufoSprite.SetPosition(mapPos);
	}
	/** マップに表示する範囲に敵がいなかったら */else {
		m_isImage = false;
	}

	m_mapSprite.Update();
	m_playerSprite.Update();
	m_cowSprite.Update();
	m_ufoSprite.Update();

}
const bool Map::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 cowPosition, Vector3 ufoPosition, Vector3& mapPosition)
{
	/** Y座標はマップの座標とは関係ないので、0にする。 */	worldCenterPosition.y = 0.0f;
	cowPosition.y = 0.0f;
	Vector3 cowDiff = cowPosition - worldCenterPosition;
	Vector3 ufoDiff = ufoPosition - worldCenterPosition;
	/** マップの中心のプレイヤーとの距離が一定以上離れていたら */if (cowDiff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE)
	{
		/** 表示しないようにする。 */return false;
	}

	if (ufoDiff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE)
	{
		/** 表示しないようにする。 */return false;
	}

	/** ベクトルの長さを取得 */float cowLength = cowDiff.Length();
	float ufoLength = ufoDiff.Length();

	/** カメラの前方向ベクトルから、クォータニオンを生成 */	Vector3 forward = g_camera3D->GetForward();

	Quaternion cowrot;
	Quaternion uforot;

	cowrot.SetRotationDegY(atan2(-forward.x, forward.z));
	uforot.SetRotationDegY(atan2(-forward.x, forward.z));

	/** ベクトルに向かう。 */	cowrot.Apply(cowDiff);
	uforot.Apply(ufoDiff);

	/** ベクトルを正規化する。 */	cowrot.Normalize();
	uforot.Normalize();

	/** マップの大きさ/距離制限で。	ベクトルをマップ座標系に変換する。*/	cowDiff *= cowLength * MAP_RADIUS / LIMITED_RANGE_IMAGE;
	ufoDiff *= ufoLength * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	/** マップの中央座標と上記ベクトルを加算する。 */	mapPosition = Vector3(MAP_CENTER_POSITION.x + cowDiff.x, MAP_CENTER_POSITION.y + cowDiff.z, 0.0f);
	mapPosition = Vector3(MAP_CENTER_POSITION.x + ufoDiff.x, MAP_CENTER_POSITION.y + ufoDiff.z, 0.0f);
	return true;
}
void Map::Render(RenderContext& rc)
{
	m_mapSprite.Draw(rc);
	m_playerSprite.Draw(rc);
	if (m_isImage == false)
	{
		return;
	}
	m_cowSprite.Draw(rc);
	m_ufoSprite.Draw(rc);
}

