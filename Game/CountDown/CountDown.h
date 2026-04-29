#pragma once
class Player;
class CountDown : public IGameObject
{
public:
	CountDown() {};
	~CountDown();

	bool  Start();
	void Update();
	void Render(RenderContext& rc);
	//カウントダウン処理
	void InCountDown();
	/*カウントダウンの変数を取りたいときに使う関数*/
	bool GetCountDown()const { return m_isCountDown; };

private:
	SpriteRender m_countDown3;
	SpriteRender m_countDown2;
	SpriteRender m_countDown1;
	SpriteRender m_countDownStart;

	/** カウントダウンの音 */
	SoundSource* m_countDownSE = nullptr;

	//カウントダウン(3秒) + START(1秒)の文字の出力を行う
	float m_countDownTime = 4.0f;

	/** カウントダウンのα値*/
	float m_countDownAlpha = 1.0f;

	/** カウントダウンのスケール値 */
	float m_countDownScale = 0.5f;

	//trueの間だけカウントダウンが進む
	bool m_isCountDown = true;

	/** カウントダウンの音を出したかどうかのフラグ */
	bool m_isPlayCountDownSE = false;

	//カウントダウン中に出す数字のUIを決めるフラグ
	bool m_Show1 = false;
	bool m_Show2 = false;
	bool m_Show3 = false;
	bool m_ShowStart = false;

};

