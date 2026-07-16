#pragma once
class DecreaseTimerUI :public IGameObject 
{
public:
	DecreaseTimerUI();
	~DecreaseTimerUI();
	bool Start();
	void Update();
	void Render(RenderContext & rc);

public:
	/** タイマーを減少させる際の処理*/
	void Play();

	/** タイマーを減少させる際のフェードアウト処理*/
	void FadeOut();

private:
	/** タイマーを減少する際に出す「－」のスプライトレンダー*/
	SpriteRender m_decreaseSpriteRender;

	/** タイマーが減少した際に出す「2」のスプライトレンダー*/
	SpriteRender m_decreaseTwoSpriteRender;

	/** タイムを追加してからの経過時間 */
	float m_timer = 0.0f;

	/** タイムを追加する際に出す[-]と[2]のアルファ値 */
	float m_alpha = 1.0f;

	/** タイムが減少したかどうかのフラグ */
	bool m_isDecreaseTimer = false;

	/** タイムが減少する際に出す[-]の基準位置 */
	Vector3 m_basePosition = Vector3::Zero;

	/** タイムが減少する際に出す[2]の基準位置 */
	Vector3 m_baseTwoPosition = Vector3::Zero;

};

