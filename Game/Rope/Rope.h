#pragma once
/**
* Ropeクラス
*/
class Player;
class Rope : public IGameObject
{
public:
	Rope();
	~Rope();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/** ロープを投げたかどうかのフラグを設定する関数 */
	void SetIsThrowRope(bool isThrowRope)
	{
		m_isThrowRope = isThrowRope;
	}

	/** ロープを投げたかどうかのフラグを取得する関数 */
	bool GetIsThrowRope()
	{
		return m_isThrowRope;
	}


private:
	/** プレイヤーが縄を投げた時の縄の処理関数*/
	void PlayerThrowsRope();

private:
	/** プレイヤー */
	Player* m_player;

	/** ロープモデルレンダー */
	ModelRender m_ropeModelRender;

	/** ロープの位置*/
	Vector3 m_ropePos;

	/** ロープを投げたかどうかのフラグ */
	bool m_isThrowRope = false;

	/** ロープアニメーションが始まったかどうかのフラグ*/
	bool m_isStartRopeAnimation = false;

	/** ロープアニメーション時間*/
	float m_ropeAnimationTime = 0.0f;

	/** ループのアニメーション */
	enum EnRopeAnimation
	{
		EnRopeAnimation_Throw,
		EnRopeAnimation_Num,
	};
	AnimationClip m_ropeAnimationClips[EnRopeAnimation_Num];
};

