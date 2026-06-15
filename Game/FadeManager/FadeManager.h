#pragma once
class FadeManager : public IGameObject
{
public:
	FadeManager() {};
	~FadeManager() {};

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** フェードイン(暗転)を設定する関数*/
	void SetFadeIn() { m_FadeAlpha = 0.0f; m_isFadeIn = true; }
	/** フェードアウト(明転)を設定する関数*/
	void SetFadeOut() { m_FadeAlpha = 1.0f; m_isFadeOut = true; }
	/** フェードインが完了したかどうかのフラグを取得するための関数*/
	bool IsFadeInComplete()const
	{
		return m_FadeAlpha >= 1.0f && !m_isFadeIn;
	}

	/** フェードアウトが完了したかどうかのフラグを取得するための関数*/
	bool IsFadeOutComplete()const
	{
		return m_FadeAlpha <= 0.0f && !m_isFadeOut;
	}

	
private:
	/** フェードインする処理*/
	void FadeIn();
	/** フェードアウトする処理*/
	void FadeOut();
private:
	SpriteRender m_BlackSpriteRender;
	/**α値*/
	float m_FadeAlpha = 0.0f;
	/** フェード速度*/
	float m_FadeSpeed = 0.5f;
	/** フェードインしているかどうかのフラグ*/
	bool m_isFadeIn = false;
	/** フェードアウトしているかどうかのフラグ*/
	bool m_isFadeOut = false;
};

