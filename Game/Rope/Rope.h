#pragma once
#include <vector>
/**
* Ropeクラス
*/
class Player;
class Cow;
class Timer;
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
	bool GetIsThrowRope()const
	{
		return m_isThrowRope;
	}

	/** 縄が牛に当たったかどうかのフラグを設定する関数 */
	void SetIsHitCow(bool isHitCow)
	{
		m_isHitCow = isHitCow;
		
		/** 縄が牛に当たっていない場合は
		 * 当たった牛の情報をリセットする */
		if (!isHitCow)
		{
			m_hitCow = nullptr;
		}
	}

	/** 縄が牛に当たったかどうかのフラグを取得する関数 */
	bool GetIsHitCow()const
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
	Cow* GetHitCow()const
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
		m_ropeModelRender.SetPosition(pos);
	}

	/** 牛を捕まえた時のロープモデルの回転設定 */
	void SetRopeModelRot(Quaternion rot)
	{
		m_ropeModelRender.SetRotation(rot);
	}

	/** ロープが縮んでいるかどうかを取得する関数 */
	bool GetIsEndRopeAnimation()const
	{
		return m_isEndRopeAnimation;
	}


private:
	/** プレイヤーが縄を投げた時の縄の処理関数 */
	void PlayerThrowsRope();

	/** プレイヤーの右手の位置のロープが常にある関数 */
	void FollowRightHand();

	/** ロープの回転に関する関数 */
	void RotateRope();

	/** セグメントの位置の計算関数 */
	void CalcCatenarySegments();

	/** セグメントの更新 */
	void UpdateSegments();

	/** ロープのアニメーション関数 */
	void CalcThrowSegments();

	/** 投げている最中のゴーストの更新 */
	void UpdateThrowGhost();

	/** ゴーストが連れ去られている牛に当たったか判定 */
	void CheckHitCowByGhost();

	/** ゴーストが牛に当たった時の処理 */
	void UpdateHandPosition();

private:
	/** プレイヤー */
	Player* m_player = nullptr;

	/** 牛 */
	Cow* m_hitCow = nullptr;

	/** タイマー */
	Timer* m_timer = nullptr;

	/** 牛リスト */
	std::vector<Cow* > m_cowList;

	/** セグメント数 */
	static const int ROPE_SEGMENT_COUNT = 10;

	/** セグメントごとのモデルレンダー */
	ModelRender m_ropeSegments[ROPE_SEGMENT_COUNT];

	/** ロープモデルレンダー */
	ModelRender m_ropeModelRender;

	/** セグメントごとの位置 */
	Vector3 m_ropeSegmentPositions[ROPE_SEGMENT_COUNT + 1];

	/** ロープの位置*/
	Vector3 m_ropePos = Vector3::Zero;

	/** ロープの回転 */
	Quaternion m_ropeRot = Quaternion::Identity;

	/** ロープのスケール */
	Vector3 m_ropeScale = Vector3::Zero;

	/** ゴーストの現在位置 */
	Vector3 m_ropeGhostPosition = Vector3::Zero;

	/** ゴーストの方向 */
	Vector3 m_throwDirection = Vector3::AxisZ;

	/** ロープが飛んでいく起点(投げた瞬間の位置で固定、ゴーストの計算基準) */
	Vector3 m_ropeThrowOrigin = Vector3::Zero;

	/** ロープのゴーストオブジェクト */
	PhysicsGhostObject m_ropeGhostObject;

	/** ゴーストが作成されたかどうかのフラグ */
	bool m_isGhostCreated = false;

	/** ロープを投げたかどうかのフラグ */
	bool m_isThrowRope = false;

	/** ロープアニメーションが始まったかどうかのフラグ */
	bool m_isStartRopeAnimation = false;

	/** ロープアニメーションが終わったかどうかのフラグ */
	bool m_isEndRopeAnimation = false;

	/** 縄が牛に当たったかどうかのフラグ*/
	bool m_isHitCow = false;

	/** ゴーストが進んだ距離 */
	float m_ropeThrowDistance = 0.0f;

	/** ロープアニメーション時間 */
	float m_ropeAnimationTime = 0.0f;

	/** ロープの垂れ下がり度 */
	float m_ropeSlack = 50.0f;

	/** ロープのたるみアニメーション時間 */
	float m_ropeSlackTime = 0.0f;

	/** ロープの縮む時間 */
	float m_ropeShrinkTime = 0.0f;

	/** RB2を押している間のロープのスピン時間 */
	float m_ropeSpinTime = 0.0f;
};

