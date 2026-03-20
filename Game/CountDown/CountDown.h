#pragma once
class Player;
class CountDown : public IGameObject
{
public:
	CountDown() {};
	~CountDown() {};

	bool  Start();
	void Update();
	void Render(RenderContext& rc);
	//カウントダウン処理
	void InCountDown();

	///*プレイヤーの操作ができるかできないかを決める関数*/
	//void SetControlEnabled(bool enabled) { m_isControlEnabled = enabled; }
	///*プレイヤーの操作ができるかできないかを決める関数*/
	//bool GetControlEnabled()const  { return m_isControlEnabled;}
	/*カウントダウンの変数を取りたいときに使う関数*/
	bool GetCountDown()const { return m_isCountDown;};

private:
	FontRender m_CountDownFontRender;
	//カウントダウン(3秒) + START(1秒)の文字の出力を行う
	float m_CountDownTime = 4.0f;
	//trueの間だけカウントダウンが進む
	bool m_isCountDown = true;

};

