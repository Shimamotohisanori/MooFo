#pragma once
class LoadingScene;
class Title : public IGameObject
{
public:
	Title() ;
	~Title();
	bool Start();
	void Update();
	//タイトルで行う処理
	void InTitle();
	void Render(RenderContext& rc);

private:
	SpriteRender m_TitleSpriteRender;//タイトルのスプライトレンダー

	SoundManager* m_soundManager;
	SoundSource* p_titleBGM;
	SoundSource* p_titleSE;
	LoadingScene* m_loadingScene;

};

