#pragma once
class Player;
class Cow;
class UFO;
class Map : public IGameObject {
public:
	Map() {}
	~Map() {}
	bool Start();
	void Update();
    void Render(RenderContext& rc);private:
	/** ワールド座標系からマップ座標系に変換 */
	const bool WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 cowPosition, Vector3 ufoPosition, Vector3& mapPosition);

	SpriteRender m_mapSprite;
	SpriteRender m_playerSprite;
	SpriteRender m_cowSprite;
	SpriteRender m_ufoSprite;

	Player* m_player = nullptr;
	Cow* m_cow = nullptr;
	UFO* m_ufo = nullptr;

	bool m_isImage = false;

};


