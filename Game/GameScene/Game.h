#pragma once
class Player;
class Stage;
class Cow;
class GameCamera;
class UFO;
class Pause;
class Timer;
class GameClear;
class GameOver;
class CountDown;
class SoundManager;
class Score;
class CowNumberOfRescues;
class CowCaptureController;
class MiniMap;
class Combo;

/** 牛の情報をまとめる構造体 */
struct Cowinfo
{
	/** オブジェクトネーム */
	std::string objectName;

	/** 星の位置を初期化する */
	Vector3 pos = Vector3::Zero;

};

/** 牛の構造体の一覧 */
const Cowinfo COW_INFOMATIONS[] =
{
	{ "cow", Vector3(-300.0f, 0.0f, 0.0f) },
	{ "cow", Vector3(-225.0f, 0.0f, 0.0f) },
	{ "cow", Vector3(-150.0f, 0.0f, 0.0f) },
	{ "cow", Vector3(-75.0f, 0.0f, 0.0f) },
	{ "cow", Vector3(0.0f, 0.0f, 0.0f) },
	{ "cow", Vector3(75.0f, 0.0f, 0.0f) },
	{ "cow", Vector3(150.0f, 0.0f, 0.0f) },
	{ "cow", Vector3(225.0f, 0.0f, 0.0f) },
	{ "cow", Vector3(300.0f, 0.0f, 0.0f) },
	{ "cow", Vector3(375.0f, 0.0f, 0.0f) },
};

/** UFOの情報をまとめる構造体 */
struct UFOinfo
{
	/** オブジェクトネーム */
	std::string objectName;
	/** 星の位置を初期化する */
	Vector3 pos = Vector3::Zero;
};

/** UFOの一覧 */
const UFOinfo UFO_INFOMATIONS[] =
{
	{ "UFO", Vector3(-1400.0f, 70.0f, 0.0f) },
	{ "UFO", Vector3(1400.0f, 70.0f, 0.0f) },
	{ "UFO", Vector3(0.0f, 70.0f, -1400.0f) },
	{ "UFO", Vector3(0.0f, 70.0f, 1400.0f) }
};


class Game : public IGameObject
{
public:
	Game() {};
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	

public:
	/** ゲームクリアで行う処理 */
	void Clear();

	/** ゲームオーバーで行う処理 */
	void Death();

	/** 生きてる牛リスト取得関数 */
	std::vector<Cow*>& GetAliveCows()
	{
		return m_aliveCows;
	}

	/** 牛を生きてる牛リストから消す関数 */
	void ReMoveCow(Cow* cow)
	{
		auto it = std::find(m_aliveCows.begin(), m_aliveCows.end(), cow);
		if (it != m_aliveCows.end())
		{
			m_aliveCows.erase(it);
		}
	}

	bool m_isSound;

	/** UFO立の取得関数 */
	std::vector<UFO*> GetUFOs()
	{
		/** UFOのリストを作成して返す */
		std::vector<UFO*> ufos;

		/** UFOの配列からUFOのリストに追加する */
		for (int i = 0; i < EnUFO_Num; i++)
		{

			/** UFOが存在する場合はリストに追加する */
			if (m_UFO[i] != nullptr)
			{
				ufos.push_back(m_UFO[i]);
			}
		}

		/** UFOのリストを返す */
		return ufos;
	}


private:
	/** 牛が生まれる関数 */
	void SpawnCow();

	/** 空出力関数 */
	void InitSkyCube();


private:
	/** ステージ */
	Stage* m_stage;

	/** プレイヤー */
	Player* m_player;

	/** ゲームカメラ */
	GameCamera* m_gameCamera;

	/** ポーズ */
	Pause* m_pause;

	/** タイマー */
	Timer* m_timer;

	/** ゲームクリア */
	GameClear* m_gameClear;

	/** ゲームオーバー */
	GameOver* m_gameOver;

	/** カウントダウン */
	CountDown* m_countDown;

	/** 牛の救出数 */
	CowNumberOfRescues* m_cowNumberOfRescues;

	/** 牛の捕獲を管理するクラス */
	CowCaptureController* m_cowCaptureController;
	MiniMap* m_map;

	/** サウンドマネージャー */
	SoundManager* m_inGameSound;

	/** ゲーム中のBGM */
	SoundSource* p_inGameBGM;

	/** スコア */
	Score* m_score;

	/** スカイキューブ */
	SkyCube* m_skyCube = nullptr;

	/** コンボ */
	Combo* m_combo;

	/** 牛の配列 */
	enum EnCow
	{
		EnCow_Cow1,
		EnCow_Cow2,
		EnCow_Cow3,
		EnCow_Cow4,
		EnCow_Cow5,
		EnCow_Cow6,
		EnCow_Cow7,
		EnCow_Cow8,
		EnCow_Cow9,
		EnCow_Cow10,
		EnCow_Num
	};

	/** 牛の配列 */
	Cow* m_cow[EnCow_Num];

	/** 生きている牛のリスト */
	std::vector<Cow*> m_aliveCows;

	/** UFOの配列 */
	enum EnUFO
	{
		EnUFO_UFO1,
		EnUFO_UFO2,
		EnUFO_UFO3,
		EnUFO_UFO4,
		EnUFO_Num
	};

	/** UFOの配列 */
	UFO* m_UFO[EnUFO_Num];

	/** スカイキューブのタイプ */
	int m_skyCubeType = enSkyCubeType_Night;

	/** 牛が生まれる時間 */
	float m_spawnTimer = 0.0f;

	/** プレイヤーが死んでいたら */
	bool m_isDead;

};

