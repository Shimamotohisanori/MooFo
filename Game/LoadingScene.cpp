#include "stdafx.h"
#include "LoadingScene.h"

LoadingScene::LoadingScene()
{

}


LoadingScene::~LoadingScene()
{

}

bool LoadingScene::Start()
{
	
	return true;
}

void LoadingScene::Update()
{

}


void LoadingScene:: InLoading()
{

}

void LoadingScene::SetNextScene(std::function<void()>next)
{
	m_nextSceneLoading = next;
}
void LoadingScene::Render(RenderContext& rc)
{

}