#pragma once
#include"Source/Actor/Character/Character.h"
class UFO : public Character
{
	public:
	UFO();
	~UFO();
	public:
		bool Start() override;
		void Update() override;
		void Render(RenderContext& rc) override;
		void Move()override;

private:
	ModelRender m_UFOmodelRender;


};

