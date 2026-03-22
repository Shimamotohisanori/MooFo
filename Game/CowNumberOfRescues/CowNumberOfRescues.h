#pragma once
/**
 * 牛の救出数クラス
 */
class CowNumberOfRescues : public IGameObject
{
	public:
    CowNumberOfRescues() {};
	~CowNumberOfRescues() {};
	bool Start();
	void Update();
	void Render(RenderContext& renderContext);


private:
	/** 救出数 */
	int m_numberOfRescues = 0;

	/**	十の位 */
	int tens = 0;

	/** 一の位 */
	int ones = 0;

	/** 数画像 */
	SpriteRender m_numberSprite[10];

	/** 救出画像 */
	SpriteRender m_rescueSprite;
};

