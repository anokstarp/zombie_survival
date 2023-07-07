#include "stdafx.h"
#include "SceneTitle.h"
#include "ResourceMgr.h"

SceneTitle::SceneTitle(SceneId id)
{
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Font, "fonts/zombiecontrol.ttf"));
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	Release();

	//AddGo(new TextGo(""))
}

void SceneTitle::Release()
{
}

void SceneTitle::Enter()
{
}

void SceneTitle::Exit()
{
}

void SceneTitle::Update(float dt)
{
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
}
