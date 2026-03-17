#pragma once
#include "sound/SoundSource.h"
class Pause;
class SoundManager : public IGameObject
{
public:
	SoundManager(){}
	~SoundManager(){}
	bool Start();
	void Update();
	void Barbgm();
	void Barse();
	void Render(RenderContext& rc);

	void SetCount(int count)
	{
		m_Count = count;
	}

	int GetCount()
	{
		return m_Count;
	}


private:
	int m_Count = 0;

	SpriteRender m_backGroundSprite;
	SpriteRender m_settingSprite;
	SpriteRender m_volumeSprite;
	SpriteRender m_notVolumeSprite;
	SpriteRender m_bgmSprite;
	SpriteRender m_seSprite;
	SpriteRender m_seVolumeSprite;
	SpriteRender m_seNotVolumeSprite;
	SpriteRender m_buttonReturnSprite;
	SpriteRender m_soundIconSprite;
	SpriteRender m_seIconSprite;
	SpriteRender m_blownBarSprite;
	SpriteRender m_seBlownBarSprite;

private:
	Pause* m_pause;

	Vector3 m_bgmVolume;
	Vector3 m_seVolume;
};

