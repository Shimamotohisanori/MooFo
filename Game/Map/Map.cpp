#include "stdafx.h"
#include "Map.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Cow/Cow.h"
#include "Source/Actor/Character/UFO/UFO.h"
#include "Pause/Pause.h"
namespace
{
	/** �~�j�}�b�v�̃X�v���C�g�̃p�X */
	const char* MAP_SPRITE_PATH = "Assets/sprite/MapUI/MapIcon.dds";

	/** �v���C���[�̃A�C�R���̃t�@�C���p�X */
	const char* PLAYER_ICON_PATH = "Assets/sprite/MapUI/PlayerIcon.dds";

	/** ���̃A�C�R���̃t�@�C���p�X */
	const char* COW_ICON_PATH = "Assets/sprite/MapUI/CowIcon.dds";

	/** UFO�̃A�C�R���̃t�@�C���p�X */
	const char* UFO_ICON_PATH = "Assets/sprite/MapUI/UFOIcon.dds";

	/** �r�b�N���}�[�N�̃t�@�C���p�X */
	const char* DANGER_ICON_PATH = "Assets/sprite/MapUI/Danger.dds";

	/** �~�j�}�b�v�̊O�g�̃t�@�C���p�X */
	const char* OUTLINE_ICON_PATH = "Assets/sprite/MapUI/OutLine.dds";

	Vector3 MAP_CENTER_POSITION = Vector3(704.0f, -300.0f, 0.0f);
  
	Vector3 MAP_OUTLINE_POSITION = Vector3(704.0f, -302.5f, 0.0f);


	constexpr float MAP_RADIUS = 180.0f;
	constexpr float LIMITED_RANGE_IMAGE = 400.0f;

	/** �}�W�b�N�i���o�[���� */
	constexpr int COW_NUM = 10;
	constexpr int UFO_NUM = 4;
}

bool Map::Start()
{
	m_pause = FindGO<Pause>("pause");

		/** �~�j�}�b�v�̔w�i */
		m_mapSprite.Init(MAP_SPRITE_PATH, 400.0f, 400.0f);
		m_mapSprite.SetPosition(MAP_CENTER_POSITION);

		/** �~�j�}�b�v�̒��S(�v���C���[) */
		m_playerSprite.Init(PLAYER_ICON_PATH, 50.0f, 50.0f);
		m_playerSprite.SetPosition(MAP_CENTER_POSITION);

		/** �~�j�}�b�v�̊O�g */
		m_outLineSprite.Init(OUTLINE_ICON_PATH, 532.0f, 532.0f);
		m_outLineSprite.SetPosition(MAP_OUTLINE_POSITION);

		/** ����~�j�}�b�v��ɏo��������B */
		for (int i = 0; i < COW_NUM; i++)
		{
			m_cowSprite[i].Init(COW_ICON_PATH, 25.0f, 25.0f);
		}

		/** UFO��~�j�}�b�v��ɏo��������B */
		for (int i = 0; i < UFO_NUM; i++)
		{
			m_ufoSprite[i].Init(UFO_ICON_PATH, 50.0f, 50.0f);
		}

		/* �r�b�N���}�[�N��UFO������߂܂����Ƃ��ɕ\��������B
		 * UFO���N�_�ƂȂ邽��UFO_NUM��g�p����
		 */
		for (int i = 0; i < UFO_NUM; i++)
		{
			m_dangerSprite[i].Init(DANGER_ICON_PATH, 30.0f, 30.0f);
		}


	/** ���ꂼ��̃|�W�V�����������B*/
	m_cows = FindGOs<Cow>("cow");
	m_ufos = FindGOs<UFO>("UFO");
	m_player = FindGO<Player>("player");

	return true;
}
void Map::Update()
{
	/** ���ꂼ��̃|�W�V��������������B */
	Vector3 playerPos = m_player->GetPosition();

	/** �J�������ǂ̕���������Ă��邩�擾����B */
	Vector3 forward = g_camera3D->GetForward();

	/*
	 *  �J�����̌�����p�x�ɕϊ�����B
	 *  atan2��X��Y�̕������ǂ̊p�x����Ԃ��֐�
	 */
	m_mapAngle = atan2(-forward.x, forward.z);

	/** ���̃A�C�R�� */
	for (int i = 0; i < m_cows.size(); i++)
	{
		if (m_cows[i]->GetIsTakeAwayed())
		{
			m_isCowImage[i] = false;
			continue;
		}
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
		/** UFO������z�����񂾂� */
		if (m_ufos[i]->GetIsCowTakeAwayed())
		{
			/** UFO�̕`������ */
			m_isUFOImage[i] = false;

			/** ����Ƀr�b�N���}�[�N��`�悳���� */
			Vector3 Pos = m_ufos[i]->GetPosition();
			Vector3 mapPos;

			/** �}�b�v�ɕ\������͈͂ɋ���UFO�������� */
			if (WorldPositionConvertToMapPosition(playerPos, Pos, mapPos))
			{
				m_dangerSprite[i].SetPosition(mapPos);
				/** �}�b�v�ɕ\������悤�ɐݒ肷��B */
				m_isdanger[i] = true;
			}

			else
			{
				m_isdanger[i] = false;
			}

			continue;
		}
		
		/** ����z������ł��Ȃ��ꍇ�ʏ��UFO��`�悳����B */
		Vector3 ufoPos = m_ufos[i]->GetPosition();
		Vector3 mapPos;

		/** �~�j�}�b�v���UFO�������� */
		if (WorldPositionConvertToMapPosition(playerPos, ufoPos, mapPos))
		{
			/** �~�j�}�b�v���UFO�ƍ��W��Z�b�g����B */
			m_isUFOImage[i] = true;
			m_ufoSprite[i].SetPosition(mapPos);
		}
		else
		{
			/** ��������Ȃ�������`�悵�Ȃ��B */
			m_isUFOImage[i] = false;
		}

		m_isdanger[i] = false;
	}

	/** �`��X�V���� */
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

	/** �x�N�g���̒�����擾 */
	float cowLength = cowDiff.Length();

	Quaternion rot;

	/** Y������Ƀ}�b�v���]������N�H�[�^�j�I�������Ă���B */
	rot.SetRotationY(m_mapAngle);

	/** �x�N�g���Ɍ������B */
	rot.Apply(cowDiff);

	/** �x�N�g���𐳋K������B */
	cowDiff.Normalize();

	/** �}�b�v�̑傫��/���������ŁB	�x�N�g����}�b�v���W�n�ɕϊ�����B*/
	cowDiff *= cowLength * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	/** �}�b�v�̒������W�Ə�L�x�N�g������Z����B */
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

		/** ����`�悳���� */
		for (int i = 0; i < m_cows.size(); i++)
		{
			/** ����~�j�}�b�v�Ȃ��ɋ���������(true) */
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

