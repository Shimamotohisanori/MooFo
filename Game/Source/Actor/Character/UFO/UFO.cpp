#include "stdafx.h"
#include "UFO.h"

namespace
{
	const char* FILEPATH = "Assets/modelData/UFO/UFO2.tkm"; //enModelUpAxis = enModelUpAxisZ;
}
UFO::UFO()
{

}

UFO::~UFO()
{
}


bool UFO::Start()
{
	m_UFOmodelRender.Init(FILEPATH);
	return true;
}

void UFO::Update()
{
	m_UFOmodelRender.Update();
}
		

void UFO::Move()
{

}
	


void UFO::Render(RenderContext& rc)
{
	m_UFOmodelRender.Draw(rc);
}

