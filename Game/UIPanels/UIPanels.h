#pragma once
class UIPanels : public IGameObject
{
public:
	UIPanels();
	~UIPanels();

	bool Start();
	void Update();
	void Render(RenderContext& renderContext);


private:
	/** 画面左下のUIの後ろに配置する黒いパネル */
	SpriteRender m_lowerLeftPanelSpriteRender;

	/** タイマーの後ろに配置する黒いパネル */
	SpriteRender m_timerPanelSpriteRender;

	/** 画面左上のUIの後ろに配置する黒いパネル */
	SpriteRender m_upperLeftPanelSpriteRender;

	/** 画面右下のUIの後ろに配置する黒いパネル */
	SpriteRender m_lowerRightPanelSpriteRender;
};

