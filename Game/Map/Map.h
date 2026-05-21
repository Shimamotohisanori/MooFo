#pragma once
class Player;
class Cow;
class UFO;
class Pause;
class Game;
class Map : public IGameObject
{
public:
	Map() {}
	~Map() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/** 牛を削除する関数 */
	/*void RemoveCow(Cow* cow)
	{
		auto iter = std::find(m_cows.begin(), m_cows.end(), cow);
		if (iter != m_cows.end())
		{
			m_cows.erase(iter);
		}
	}*/

private:
	/** ワールド座標系からマップ座標系に変換 */
	bool WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 cowPosition, Vector3& mapPosition);

	/** vector型で牛とUFOの数値を入れている。 */
	//std::vector<Cow*> m_cows;
	Game* m_game;

	std::vector<UFO*> m_ufos;

	/** マップの画像 */
	SpriteRender m_mapSprite;

	/** プレイヤーアイコンの画像 */
	SpriteRender m_playerSprite;

	/** 牛アイコンの画像 */
	SpriteRender m_cowSprite[10];

	/** UFOアイコンの画像 */
	SpriteRender m_ufoSprite[4];

	/** 危険アイコンの画像 */
	SpriteRender m_dangerSprite[4];

	/** アウトラインアイコンの画像 */
	SpriteRender m_outLineSprite;

	/** プレイヤーのポインタ */
	Player* m_player = nullptr;

	/** ポーズのポインタ */
	Pause* m_pause = nullptr;
	/** マップの回転角度 */
	float m_mapAngle = 0.0f;

	float m_flashTImer = 0.0f;

	/** マップ内に表示されているUIがいるかどうかのフラグ */
	bool m_isCowImage[10];
	bool m_isUFOImage[4];
	bool m_isdanger[4];

};