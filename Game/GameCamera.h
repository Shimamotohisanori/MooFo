#pragma once

class Player;
class Rope;
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


private:
	Player* m_player;
	Rope* m_rope;
	Vector3 m_CameraPos;
};

