#pragma once
class Player;
class Rope;
class Game;
class VoiceManager;
class CowLivesUI;
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

	/** 牛に当たったかどうかのフラグ取得関数 */
	bool GetIsCowCaptured()const 
	{
		return m_isCowCaptured;
	}

	/** 照準が牛を狙っているかどうかのフラグ取得関数 */
	bool GetIsAimingCow() const
	{
		return m_isAimingCow;
	}

	/** カメラの位置取得関数 */
	Vector3 GetCameraPosition() const
	{
		return g_camera3D->GetPosition();
	}

	/** カメラの前方向の取得 */
	Vector3 GetCameraForward() const
	{
		return g_camera3D->GetForward();
	}

	/** 牛を捕まえた瞬間、牛捕獲用カメラの基準位置を初期化する関数 */
	void InitHitCowCameraPos()
	{
		/** 現在のカメラの位置と注視点の差分を基準に、牛捕獲用のオフセットを設定する */
		Vector3 currentPos = g_camera3D->GetPosition();
		Vector3 currentTarget = g_camera3D->GetTarget();

		Vector3 offset = currentPos - currentTarget;

		/** 差分がゼロに近い場合は、デフォルトの後方・上方オフセットを使う */
		if (offset.LengthSq() < 0.0001f)
		{
			offset = Vector3(0.0f, 80.0f, -200.0f);
		}

		m_hitCowCameraPos = offset;
	}

	/** 照準が狙っている牛座標を取得する関数 */
	Vector3 GetAimTargetPosition() const;

private:
	/** 牛がロープに当たったら*/
	void HitCow();

	/** 照準が牛を狙っているかどうかの確認関数 */
	void CheckAimingCow();


private:
	/** プレイヤー */
	Player* m_player = nullptr;

	/** ロープ */
	Rope* m_rope = nullptr;

	/** ゲーム */
	Game* m_game = nullptr;

	/** ロープを投げている間のカメラ位置を保存する変数 */
	Vector3 m_cameraPos = Vector3::Zero;

	/** ロープ追従開始前のカメラ位置を保存する変数 */
	Vector3 m_savedCameraPos = Vector3::Zero; 

	/** ロープを投げている間のforwardを固定する */
	Vector3 m_ropeForward = Vector3::Zero;

	/** 牛捕獲時専用カメラ位置 */
	Vector3 m_hitCowCameraPos = Vector3::Zero;

	/** 牛を捕まえた時の音 */
	SoundSource* m_cowCatchSE = nullptr;

	/** ボイスマネージャー */
	VoiceManager* m_voiceManager = nullptr;

	/** プレイヤーの残機UI*/
	CowLivesUI* m_cowLivesUI = nullptr;

	/** カメラがロープの事を追従し始めたかどうかのフラグ */
	bool m_isRopeCameraStarted = false;

	/** 牛に当たったかどうかのフラグ */
	bool m_isCowCaptured = false;

	/** 照準が牛を狙っているかどうかのフラグ */
	bool m_isAimingCow = false;
};

