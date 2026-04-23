#pragma once
class Player;
class Cow;
class Map : public IGameObject
{
 public:
	Map() {}
	~Map() {}
	bool Start();
	void Update();
    void Render(RenderContext& rc);private:
	/** ワールド座標系からマップ座標系に変換 */
	bool WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 cowPosition, Vector3& mapPosition);

	std::vector<Cow*> m_cows;

	SpriteRender m_mapSprite;
	SpriteRender m_playerSprite;
	SpriteRender m_cowSprite[10];

	Player* m_player = nullptr;
	Cow* m_cow = nullptr;

	bool m_isImage[10];

};


