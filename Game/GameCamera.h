#pragma once

class Player;
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
	Player* m_player;
	Vector3 m_CameraPos;
};

