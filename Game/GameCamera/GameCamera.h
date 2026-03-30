#pragma once
class Player;
class Rope;
class Cow;
class GameCamera : public IGameObject
{

public:
	GameCamera();
	~GameCamera();


public:
	bool Start() override;
	void Update() override;
	void  Follow();

	/** 牛に当たったかどうかのフラグ */
	void SetIsCowCaptured(bool isCowCaptured)
	{
		m_isCowCaptured = isCowCaptured;
	}

	/** カメラの位置取得関数 */
	Vector3 GetCameraPosition()
	{
		return g_camera3D->GetPosition();
	}

	/** カメラの前方向の取得 */
	Vector3 GetCameraForward()
	{
		return g_camera3D->GetForward();
	}


private:
	/** ロープを追いかける関数*/
	void FollowRope();

	/** 牛に当たったかどうかの判定関数*/
	void CheckCameraHitCow();

	/** 牛がロープに当たったら*/
	void HitCow();


private:
	Player* m_player;
	Rope* m_rope;
	Cow* m_cow;

	Vector3 m_cameraPos;

	/** ロープ追従開始前のカメラ位置を保存する変数 */
	Vector3 m_savedCameraPos; 

	/** ロープを投げている間のforwardを固定する */
	Vector3 m_ropeForward;

	/** カメラがロープの事を追従し始めたかどうかのフラグ */
	bool m_isRopeCameraStarted = false;

	/** 牛に当たったかどうかのフラグ */
	bool m_isCowCaptured = false;
};

