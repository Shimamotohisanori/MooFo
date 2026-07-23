#pragma once
/** 照準クラス */
class Pause;
class Cow;
class CountDown;
class GameCamera;
class Rope;
class CowLivesUI;
class Aiming : public IGameObject
{
public:
	Aiming();
	~Aiming();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

private:
	/** 照準の画像 */
	SpriteRender m_aimingSpriteRender;

	/** ポーズクラスのポインタ */
	Pause* m_pause = nullptr;

	/** カウントダウンクラスのポインタ */
	CountDown* m_countdown = nullptr;

	/** ゲームカメラのポインタ */
	GameCamera* m_gameCamera = nullptr;

	/** ロープクラスのポインタ */
	Rope* m_rope = nullptr;
	/** プレイヤーの残機ポインタ*/
	CowLivesUI* m_cowLivesUI = nullptr;

	/** 牛が捕まっているかどうかのフラグ */
	bool m_isAnyCowCaptured = false;
};

