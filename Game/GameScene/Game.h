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

	bool m_isSound;

private:
	ModelRender m_modelRender;
	Vector3 m_pos;

	bool m_isDead;
	
private:
	Stage* m_stage;
	Player* m_player;
	Cow* m_cow;
	//回転している牛
	Cow* m_spincow;
	GameCamera* m_gameCamera;
	UFO* m_UFO;
	Pause* m_pause;
	//棒立ちのUFO
	UFO* m_IdleUFO;
	Timer* m_timer;
	GameClear* m_gameClear;
	GameOver* m_gameOver;
	CountDown* m_countDown;

	SoundManager* m_inGameSound;
	SoundSource* p_inGameBGM;
};

