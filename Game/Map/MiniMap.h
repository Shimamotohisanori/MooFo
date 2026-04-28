#pragma once
class Player;
class Cow;
class UFO;
class Pause;
class MiniMap : public IGameObject
{
public:
	MiniMap() {}
	~MiniMap() {}
	bool Start();
	void Update();
void Render(RenderContext& rc);private:
	/** ワールド座標系からマップ座標系に変換 */
	bool WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 cowPosition, Vector3& mapPosition);

	/** vector型で牛とUFOの数値を入れている。 */
	std::vector<Cow*> m_cows;
	std::vector<UFO*> m_ufos;

	SpriteRender m_mapSprite;
	SpriteRender m_playerSprite;
	SpriteRender m_cowSprite[10];
	SpriteRender m_ufoSprite[4];
	SpriteRender m_dangerSprite[4];

	Player* m_player = nullptr;
	Pause* m_pause = nullptr;

	float m_mapAngle = 0.0f;

	/** マップ内に表示されているUIがいるかどうかのフラグ */
	bool m_isCowImage[10];
	bool m_isUFOImage[4];
	bool m_isdanger[4];

};


