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
	//void Render(RenderContext& rc) override;

	void  Follow();

private:
	/** ロープを追いかける関数*/
	void FollowRope();

	/** 牛がロープに当たったら*/
	void HitCow();
private:
	Player* m_player;
	Rope* m_rope;
	Cow* m_cow;
	Vector3 m_CameraPos;
};

