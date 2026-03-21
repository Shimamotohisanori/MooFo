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
	{ "UFO", Vector3(-300.0f, 70.0f, 0.0f) },
	{ "UFO", Vector3(300.0f, 70.0f, 0.0f) },
	{ "UFO", Vector3(600.0f, 70.0f, 0.0f) },
	{ "UFO", Vector3(900.0f, 70.0f, 0.0f) }
};

class Game : public IGameObject
{
public:
	Game() {};
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc) {};
	
public:
	//ゲームクリアで行う処理
	void Clear();
	void Death();

	/** 生きてる牛リスト取得関数 */
	std::vector<Cow*>& GetAliveCows()
	{
		return m_aliveCows;
	}

	bool m_isSound;
	

private:
	//牛が生まれる関数
	void SpawnCow();

private:
	Stage* m_stage;
	Player* m_player;
	GameCamera* m_gameCamera;
	Pause* m_pause;
	Timer* m_timer;
	GameClear* m_gameClear;
	GameOver* m_gameOver;
	CountDown* m_countDown;

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
	Cow* m_cow[EnCow_Num];

	/** 生きている牛のリスト */
	std::vector<Cow*> m_aliveCows;

	enum EnUFO
	{
		EnUFO_UFO1,
		EnUFO_UFO2,
		EnUFO_UFO3,
		EnUFO_UFO4,
		EnUFO_Num
	};
	UFO* m_UFO[EnUFO_Num];

	SoundManager* m_inGameSound;
	SoundSource* p_inGameBGM;

	/** 牛が生まれる時間 */
	float m_spawnTimer = 0.0f;

	bool m_isDead;

};

