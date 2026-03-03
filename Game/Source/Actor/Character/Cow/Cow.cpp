#include "stdafx.h"
#include "Cow.h"
namespace
{
	const char* FILEPATH = "Assets/modelData/Cow/Model/Cow.tkm"; //enModelUpAxis = enModelUpAxisZ;
}
Cow::Cow()
{

}

Cow::~Cow()
{

}

bool Cow::Start()
{
	m_CowmodelRender.Init(FILEPATH);
	return true;
}

void Cow::Update()
{
	m_CowmodelRender.Update();
}


void Cow::Move()
{

}

void Cow::Render(RenderContext& rc)
{
	m_CowmodelRender.Draw(rc);
}
