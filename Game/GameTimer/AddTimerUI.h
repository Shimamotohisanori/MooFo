#pragma once
class AddTimerUI : public IGameObject
{
public:
	AddTimerUI();
	~AddTimerUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:

	void Play();

	void FadeOut();

private:
	SpriteRender m_addTimerPlusSpriteRender;/** タイムを追加する際に出す[+]のスプライトレンダー*/
	SpriteRender m_addTimerThreeSpriteRender;/** タイムを追加する際に出す[3]のスプライトレンダー*/
	float m_timer = 0.0f;/** タイムを追加してからの経過時間*/
	float m_alpha = 1.0f;/** タイムを追加する際に出す[+]と[3]のアルファ値*/
	bool m_isAddTimer = false;/** タイムを追加したかどうかのフラグ*/

	Vector3 m_basePosition;/** タイムを追加する際に出す[+]の基準位置 */
	Vector3 m_baseThreePosition;/** タイムを追加する際に出す[3]の基準位置 */
};

