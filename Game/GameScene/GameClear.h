#pragma once
class GameClear :public IGameObject
{
	public:
	GameClear();
	~GameClear();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//ゲームクリアで行う処理
	void InGameClear();
private:
	SpriteRender m_GameClearSpriteRender;//ゲームクリアのスプライトレンダー
	bool m_isDeleteRequst = false;//削除要求フラグ

};

