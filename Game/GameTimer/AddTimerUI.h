#pragma once
/** タイムを追加する際のUIクラス */
class AddTimerUI : public IGameObject
{
public:
	AddTimerUI();
	~AddTimerUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);


public:
	/** タイムを追加する際の処理 */
	void Play();

	/** タイムを追加する際のフェードアウト処理 */
	void FadeOut();


private:
	/** タイムを追加する際に出す[+]のスプライトレンダー */
	SpriteRender m_addTimerPlusSpriteRender;

	/** タイムを追加する際に出す[3]のスプライトレンダー */
	SpriteRender m_addTimerThreeSpriteRender;

	/** タイムを追加してからの経過時間 */
	float m_timer = 0.0f;

	/** タイムを追加する際に出す[+]と[3]のアルファ値 */
	float m_alpha = 1.0f;

	/** タイムを追加したかどうかのフラグ */
	bool m_isAddTimer = false;

	/** タイムを追加する際に出す[+]の基準位置 */
	Vector3 m_basePosition = Vector3::Zero;

	/** タイムを追加する際に出す[3]の基準位置 */
	Vector3 m_baseThreePosition = Vector3::Zero;
};

