#pragma once
#include <vector>
/**
* Ropeクラス
*/
class Player;
class Cow;
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

	/** 縄が牛に当たったかどうかのフラグを取得する関数 */
	void SetIsHitCow(bool isHitCow)
	{
		m_isHitCow = isHitCow;
		if (!isHitCow)
		{
			m_hitCow = nullptr;
		}
	}

	/** 縄が牛に当たったかどうかのフラグを取得する関数 */
	bool GetIsHitCow()
	{
		return m_isHitCow;
	}

	/** 牛に当たったときの処理関数 */
	void OnHitCow(Cow* cow);

	/** 当たった牛をセットする関数 */
	void SetHitCow(Cow* cow) 
	{
		m_hitCow = cow;
	}

	/** 当たった牛を取得する関数 */
	Cow* GetHitCow()
	{ 
		return m_hitCow;
	}

	/** 牛リストをセットする関数 */
	void SetCowList(const std::vector<Cow*>& cows)
	{
		m_cowList = cows;
	}

	/** 牛を捕まえた時のロープモデルの位置設定 */
	void SetRopeModelPos(Vector3 pos)
	{
		m_ropeCapturedCowModelRender.SetPosition(pos);
	}

	/** 牛を捕まえた時のロープモデルの回転設定 */
	void SetRopeModelRot(Quaternion rot)
	{
		m_ropeCapturedCowModelRender.SetRotation(rot);
	}

private:
	/** プレイヤーが縄を投げた時の縄の処理関数 */
	void PlayerThrowsRope();

	/** プレイヤーの右手の位置のロープが常にある関数 */
	void FollowRightHand();

	/** ロープの伸び縮みに関する関数 */
	void StretchRope();

	/** 伸び縮みするロープの回転に関する関数 */
	void RotateStretchRope();

	/** ロープの回転に関する関数 */
	void RotateRope();

private:
	/** プレイヤー */
	Player* m_player;

	/** 牛 */
	Cow* m_hitCow = nullptr;

	/** 牛リスト */
	std::vector<Cow* > m_cowList;

	/** ロープモデルレンダー */
	ModelRender m_ropeModelRender;

	/** 牛を捕まえた時のロープモデルレンダー */
	ModelRender m_ropeCapturedCowModelRender;

	/** ロープの位置*/
	Vector3 m_ropePos;

	/** ロープの回転*/
	Quaternion m_ropeRot;

	/** ロープのスケール*/
	Vector3 m_ropeScale;

	/** ロープを投げたかどうかのフラグ */
	bool m_isThrowRope = false;

	/** ロープアニメーションが始まったかどうかのフラグ */
	bool m_isStartRopeAnimation = false;

	/** 縄が牛に当たったかどうかのフラグ*/
	bool m_isHitCow = false;

	/** ロープアニメーション時間 */
	float m_ropeAnimationTime = 0.0f;
};

