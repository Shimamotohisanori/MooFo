#pragma once
#include"Source/Actor/Character/Character.h"
class Cow : public Character
{
public:
	Cow();
	~Cow();

	public:
		bool Start() override;
		void Update() override;
		void Render(RenderContext& rc) override;

		void Move()override;
private:
	ModelRender m_CowmodelRender;
};

