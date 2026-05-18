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
class DummyCow;
class Combo;
class AddTimerUI;
class EffectManager;

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
	{ "UFO", Vector3(-1400.0f, 70.0f, 0.0f) },
	{ "UFO", Vector3( 1400.0f, 70.0f, 0.0f) },
	{ "UFO", Vector3(    0.0f, 70.0f, -1400.0f) },
	{ "UFO", Vector3(    0.0f, 70.0f, 1400.0f) }
};

/** ゲームクラス */
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
	void ReMoveCow(Cow* cow);

	bool m_isSound;

	/** UFO立の取得関数 */
	std::vector<UFO*> GetUFOs()
	{
		/** UFOのリストを作成して返す */
		std::vector<UFO*> m_ufos;

		/** UFOの配列からUFOのリストに追加する */
		for (int i = 0; i < EnUFO_Num; i++)
		{

			/** UFOが存在する場合はリストに追加する */
			if (m_UFO[i] != nullptr)
			{
				m_ufos.push_back(m_UFO[i]);
			}
		}

		/** UFOのリストを返す */
		return m_ufos;
	}

	/** タイムアウトフラグの取得関数 */
	bool GetIsTimeOut()
	{
		return m_isTimeOut;
	}
	/** ダミーの牛のセッター*/
	void SetDuumyCow(DummyCow* cow)
	{
		m_dummyCow = cow;
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
	/** ダミーの牛*/
	DummyCow* m_dummyCow = nullptr;
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

	/** エフェクトマネージャー */
	EffectManager* m_effectManager;

	/** タイムアウトの画像のスケール */
	Vector3 m_timeOutImageScale = Vector3(0.5f, 0.5f, 1.0f);

	/** 難易度(牛の救出数)により変動する牛生成の範囲 */
	int m_difficultyLevelSpawnRange = 0;

	/** 牛が生まれる時間 */
	float m_spawnTimer = 0.0f;

	/** タイムアウトからの経過時間 */
	float m_timeOutTimer = 0.0f;

	/** タイムアウトフラグ */
	bool m_isTimeOut = false;

};

