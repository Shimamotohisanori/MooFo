#pragma once
class CowFood;
class Player;
class CowLuring : public IGameObject
{
public:
	CowLuring(){}
	~CowLuring(){}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetPosition(const Vector3& pos);

	const Vector3& GetPosition() const
	{
		return m_position;
	}
private:
	void PutHay();

	Vector3 m_position;

	ModelRender m_hayModelRender;

	CowFood* m_cowfood;

	Player* m_player;


};

