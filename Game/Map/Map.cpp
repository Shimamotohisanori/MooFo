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

	/** �}�W�b�N�i���o�[���� */
	/** マジックナンバー処理 */
	const int COW_NUM = 10;
	const int UFO_NUM = 4;
}

bool Map::Start()
{
	/** �~�j�}�b�v�̔w�i */	
	m_mapSprite.Init("Assets/sprite/MapUI/MapIcon.dds", 400.0f, 400.0f);
	m_mapSprite.SetPosition(MAP_CENTER_POSITION);

	/** �~�j�}�b�v�̒��S(�v���C���[) */	
	m_playerSprite.Init("Assets/sprite/MapUI/PlayerIcon.dds", 50.0f, 50.0f);
	m_playerSprite.SetPosition(MAP_CENTER_POSITION);

	/** �����~�j�}�b�v���ɏo��������B */
	for (int i = 0; i < COW_NUM; i++)
	{
		m_cowSprite[i].Init("Assets/sprite/MapUI/CowIcon.dds", 25.0f, 25.0f);
	}

	/** UFOをミニマップ内に出現させる。 */
	for(int i = 0; i < UFO_NUM; i++)
  {	
		m_ufoSprite[i].Init("Assets/sprite/MapUI/UFOIcon.dds", 30.0f, 30.0f);
	}
	

	/** ���ꂼ��̃|�W�V������������B*/
	m_cows = FindGOs<Cow>("cow");
	m_ufos = FindGOs<UFO>("UFO");
	m_player = FindGO<Player>("player");
	
	return true;
}
void Map::Update()
{
	/** ���ꂼ��̃|�W�V��������������B */	
	Vector3 playerPos = m_player->GetPosition();


	/** �J�������ǂ̕����������Ă��邩�擾����B */
	Vector3 forward = g_camera3D->GetForward();

	/* 
	 *  �J�����̌������p�x�ɕϊ�����B
	 *  atan2��X��Y�̕������ǂ̊p�x����Ԃ��֐� 
	 */
	m_mapAngle = atan2(-forward.x, forward.z);

	/** ���̃A�C�R�� */
	Vector3 forward = g_camera3D->GetForward();

	for (int i = 0; i < m_cows.size(); i++)
	{
		Vector3 cowPos = m_cows[i]->GetPosition();
		Vector3 mapPos;

		/** �}�b�v�ɕ\������͈͂ɋ���UFO�������� */
		if (WorldPositionConvertToMapPosition(playerPos, cowPos, mapPos))
		{
			/** �}�b�v�ɕ\������悤�ɐݒ肷��B */
			m_isCowImage[i] = true;

			/** SpriteRender�ɍ��W��ݒ� */
			m_cowSprite[i].SetPosition(mapPos);
		}

		/** �}�b�v�ɕ\������͈͂ɓG�����Ȃ������� */
		else
		{
			m_isCowImage[i] = false;
		}
	}

	/** UFO�̃A�C�R�� */
	for (int i = 0; i < m_ufos.size(); i++)
	{
		Vector3 ufoPos = m_ufos[i]->GetPosition();
		Vector3 mapPos;

		/** �}�b�v�ɕ\������͈͂ɋ���UFO�������� */
		if (WorldPositionConvertToMapPosition(playerPos, ufoPos, mapPos))
		{
			/** �}�b�v�ɕ\������悤�ɐݒ肷��B */
			m_isUFOImage[i] = true;

			/** SpriteRender�ɍ��W��ݒ� */
			m_ufoSprite[i].SetPosition(mapPos);
		}

		/** �}�b�v�ɕ\������͈͂ɓG�����Ȃ������� */
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
	/** Y���W�̓}�b�v�̍��W�Ƃ͊֌W�Ȃ��̂ŁA0�ɂ���B */
	worldCenterPosition.y = 0.0f;
	cowPosition.y = 0.0f;
	Vector3 cowDiff = cowPosition - worldCenterPosition;
	/** �}�b�v�̒��S�̃v���C���[�Ƃ̋��������ȏ㗣��Ă����� */
	if (cowDiff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE)
	{
		/** �\�����Ȃ��悤�ɂ���B */
		return false;
	}

	/** �x�N�g���̒������擾 */
	float cowLength = cowDiff.Length();

	Quaternion rot;

	
	/** Y������Ƀ}�b�v����]������N�H�[�^�j�I��������Ă���B */
	rot.SetRotationY(m_mapAngle);

	/** �x�N�g���Ɍ������B */
	rot.Apply(cowDiff);

	/** �x�N�g���𐳋K������B */
	cowDiff.Normalize();

	/** �}�b�v�̑傫��/���������ŁB	�x�N�g�����}�b�v���W�n�ɕϊ�����B*/
	cowDiff *= cowLength * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	/** �}�b�v�̒������W�Ə�L�x�N�g�������Z����B */
	rot.SetRotationY(m_mapAngle);
	/** ベクトルに向かう。 */
	rot.Apply(cowDiff);

	/** ベクトルを正規化する。 */
	cowDiff.Normalize();

	/** マップの大きさ/距離制限で。	ベクトルをマップ座標系に変換する。*/
	cowDiff *= cowLength * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	/** マップの中央座標と上記ベクトルを加算する。 */
	mapPosition = Vector3(MAP_CENTER_POSITION.x + cowDiff.x , MAP_CENTER_POSITION.y + cowDiff.z, 0.0f);
	

	return true;
}
void Map::Render(RenderContext& rc)
{
	m_mapSprite.Draw(rc);
	m_playerSprite.Draw(rc);

	/** ����`�悳���� */
	for (int i = 0; i < m_cows.size(); i++)
	{
		/** �����~�j�}�b�v�Ȃ��ɋ���������(true) */
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

