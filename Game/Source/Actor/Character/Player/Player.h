#pragma once
#include"Source/Actor/Character/Character.h"
/**
*Playerクラス
*/
class Player : public Character
{
public:
	Player();
	~Player();

public:
    bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Move()override;

	Vector3 GetPosition()
	{
		return m_transform.GetPosition();
	};

private:
	ModelRender m_playerModelRender;

	Vector3 m_moveSpeed;//移動速度
	CharacterController m_characterController;//キャラコン

	Vector3 m_playerPos;//プレイヤーの座標
};

