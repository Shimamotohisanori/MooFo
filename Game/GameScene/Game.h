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
class Map;
class Combo;
class AddTimerUI;

/** UFOの情報をまとめる構造体 */
struct UFOinfo
{
	/** オブジェクトネーム */
	std::string objectName;
	/** UFOの位置を初期化する */
	Vector3 pos = Vector3::Zero;
};

/** UFOの一覧 */
const UFOinfo UFO_INFOMATIONS[] =
{
	{ "UFO1", Vector3(-1400.0f, 70.0f, 0.0f) },
	{ "UFO2", Vector3(1400.0f, 70.0f, 0.0f) },
	{ "UFO3", Vector3(0.0f, 70.0f, -1400.0f) },
	{ "UFO4", Vector3(0.0f, 70.0f, 1400.0f) }
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

	/** タイムアウトフラグの取得関数 */
	bool GetIsTimeOut()
	{
		return m_isTimeOut;
	}


private:
	/** 牛が生まれる関数 */
	void SpawnCow();

	/** タイムアウト処理 */
	void TimeOut();


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

	/** ミニマップ */
	Map* m_map;

	/** サウンドマネージャー */
	SoundManager* m_inGameSound;

	/** ゲーム中のBGM */
	SoundSource* m_inGameBGM;

	/** タイムアウト時のSE */
	SoundSource* m_timeOutSE;

	/** スコア */
	Score* m_score;

	/** スカイキューブ */
	SkyCube* m_skyCube = nullptr;

	/** コンボ */
	Combo* m_combo;

	/** タイマー追加UI */
	AddTimerUI* m_addTimerUI;

	/** 終了画像 */
	SpriteRender m_timeOutImage;

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

	/** タイムアウトの画像のスケール */
	Vector3 m_timeOutImageScale = Vector3(0.5f, 0.5f, 1.0f);

	/** 牛が生まれる時間 */
	float m_spawnTimer = 0.0f;

	/** タイムアウトからの経過時間 */
	float m_timeOutTimer = 0.0f;

	/** タイムアウトフラグ */
	bool m_isTimeOut = false;

};

