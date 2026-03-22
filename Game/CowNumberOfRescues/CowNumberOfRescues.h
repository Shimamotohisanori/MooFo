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

	/** 救出数を加算する関数 */
	void AddRescue()
	{
		m_numberOfRescues++;
	}


private:
	/** 救出数 */
	int m_numberOfRescues = 0;

	/**	十の位 */
	int tens = 0;

	/** 一の位 */
	int ones = 0;

	/** ファイルパス */
	std::string m_filePath[10];

	/** 十の位専用数字画像 */
	SpriteRender m_tensSprite[10];

	/** 一の位専用数字画像 */
	SpriteRender m_onesSprite[10];

	/** 救出画像 */
	SpriteRender m_rescueSprite;

	/**「/」のUI画像 */
	SpriteRender m_slashSprite;

	/** 1の画像 */
	SpriteRender m_oneSprite;

	/** 5の画像 */
	SpriteRender m_fiveSprite;
};

