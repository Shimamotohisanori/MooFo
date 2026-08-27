#pragma once
class UIPanels : public IGameObject
{
public:
	UIPanels();
	~UIPanels();

	bool Start();
	void Update();
	void Render(RenderContext& renderContext);

	/** 外部から表示/非表示を明示的に切り替える関数*/
	void SetVisible(bool isVisiblle)
	{
		m_isVisible = isVisiblle;
	}
private:
	/** 画面左下のUIの後ろに配置する黒いパネル */
	SpriteRender m_lowerLeftPanelSpriteRender;

	/** タイマーの後ろに配置する黒いパネル */
	SpriteRender m_timerPanelSpriteRender;

	/** 画面左上のUIの後ろに配置する黒いパネル */
	SpriteRender m_upperLeftPanelSpriteRender;

	/** 画面右下のUIの後ろに配置する黒いパネル */
	SpriteRender m_lowerRightPanelSpriteRender;


	/** 表示してよいかどうかを明示的に管理するフラグ
	 * ロード完了(ActivateGameBGM)まではfalseにしておく */
	bool m_isVisible = false;
};

