#pragma once
class LoadingScene : public IGameObject
{
public:
	LoadingScene();
	~LoadingScene();

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/** 次のシーンをセットする関数*/
	void SetNextScene(std::function<void()>next);
	/** ローディング中の処理を行う関数*/
	void InLoading();
private:
	SpriteRender m_loadingSpriteRender[3];
	int m_currentImage = 0;/** 現在表示している画像*/
	float m_timer = 0.0f;/** タイマー*/
	float m_changeTime = 1.5f;/**画像を切り替える時間*/
	//これは、LoadingSceneを呼び出すときに、次のシーンをロードする関数を引数で渡してもらうための変数。
	std::function<void()> m_nextSceneLoading;/** 次のシーンをロードする関数*/

}; 

