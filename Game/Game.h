#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class Stage;

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;
	Vector3 m_pos;

	Stage* m_stage;
};

