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
class UFOLightUI;
class UFOLightManager;
class CowFood;
class CowFoodManager;
class CowLuring;
class FadeManager;

/** UFOの情報をまとめる構造体 */
struct UFOinfo
{
	/** オブジェクトネーム */
	std::string objectName;
	/** UFOの位置を初期化する */
	Vector3 pos = Vector3::Zero;
};
/** UFOの再出現リクエストをまとめる構造体 
　　どのUFOを何秒後に再出現させるかを管理する */
struct UFORespawnRequest
{
	/** 初期値を-1にする理由は、UFOの配列のインデックスとして
	有効な値ではないことを示すため */
	int SlotIndex = -1; // UFOの配列のインデックス
	float RespawnTimer = 0.0f; // UFOが再出現するまでの時間
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
	/** UFOが消えた際にリクエストをする関数*/
	void RequestUFORespawn(int slotIndex);

	/** UFO達の取得関数 */
	std::vector<UFO*> GetUFOs()const
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

	/** UFOの配列をセットする関数 */
	void SetUFOList(const std::vector<UFO*>& ufos);

	/** タイムアウトフラグの取得関数 */
	bool GetIsTimeOut()const 
	{
		return m_isTimeOut;
	}
	
	/** ダミーの牛のセッター*/
	void SetDuumyCow(DummyCow* cow)
	{
		m_dummyCow = cow;
	}
	/** タイムアウトのフェード処理でUIを消すためのゲッター関数*/
	bool IsFadeTimeOut()const
	{
		return m_isfadeStart;
	}

	/** 全ての牛を削除する関数 */
	void KillAllCows();


private:
	/** 牛が生まれる関数 */
	void SpawnCow();

	/** タイムアウト処理 */
	void TimeOut();
	/** 消えたUFOを新しく生成させる関数 */
	void UpdateUFORespawn();

private:
	/** ステージ */
	Stage* m_stage = nullptr;

	/** プレイヤー */
	Player* m_player = nullptr;

	/** ゲームカメラ */
	GameCamera* m_gameCamera = nullptr;

	/** ポーズ */
	Pause* m_pause = nullptr;

	/** タイマー */
	Timer* m_timer  = nullptr;

	/** ゲームクリア */
	GameClear* m_gameClear = nullptr;

	/** ゲームオーバー */
	GameOver* m_gameOver = nullptr;

	/** カウントダウン */
	CountDown* m_countDown = nullptr;

	/** 牛の救出数 */
	CowNumberOfRescues* m_cowNumberOfRescues = nullptr;

	/** ミニマップ */
	Map* m_map = nullptr;

	/** サウンドマネージャー */
	SoundManager* m_inGameSound = nullptr;

	/** ゲーム中のBGM */
	SoundSource* m_inGameBGM = nullptr;

	/** タイムアウト時のSE */
	SoundSource* m_timeOutSE = nullptr;

	SoundSource* m_pauseSE = nullptr;

	/** スコア */
	Score* m_score = nullptr;

	/** スカイキューブ */
	SkyCube* m_skyCube = nullptr;
	/** コンボ */
	Combo* m_combo = nullptr;

	/** タイマー追加UI */
	AddTimerUI* m_addTimerUI = nullptr;
	
	/** ダミーの牛 */
	DummyCow* m_dummyCow = nullptr;
	
	/** 牛のライトUI */
	UFOLightUI* m_ufoLightUI = nullptr;

	/** UFOのライトマネージャー */
	UFOLightManager* m_ufoLightManager = nullptr;

	/** 牛の餌 */
	CowFood* m_cowFood = nullptr;

	/** 牛の餌の管理 */
	CowFoodManager* m_cowFoodManager = nullptr;

	/** 牛の餌のモデル */
	CowLuring* m_cowLuring = nullptr;

	/** 終了画像 */
	SpriteRender m_timeOutImage;

	/** フェードマネージャーのポインタ*/
	FadeManager* m_fadeManager = nullptr;

	/** 生きている牛のリスト */
	std::vector<Cow*> m_aliveCows;
	/** フェード処理を開始させるフラグ*/
	bool m_isfadeStart = false;

	bool m_isFadeOut = false;
	/** UFOの再出現リクエストのリスト */
	std::vector<UFORespawnRequest> m_ufoRespawnRequests;
	/** リスポーン時間*/
	static constexpr float UFO_RESPAWN_TIME = 10.0f;
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

	/** 難易度(牛の救出数)により変動する牛生成の範囲 */
	int m_difficultyLevelSpawnRange = 0;

	/** 牛が生まれる時間 */
	float m_spawnTimer = 0.0f;

	/** タイムアウトからの経過時間 */
	float m_timeOutTimer = 0.0f;

	/** タイムアウトフラグ */
	bool m_isTimeOut = false;

};

