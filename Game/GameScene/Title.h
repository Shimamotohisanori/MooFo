#pragma once
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

};

