#include "stdafx.h"
#include "SceneTitle.h"
#include "ResourceMgr.h"
#include "TextGo.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "SpriteGo.h"
#include "Framework.h"

SceneTitle::SceneTitle(SceneId id)
	: Scene(SceneId::Title)
{
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background_resize.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Font, "fonts/zombiecontrol.ttf"));
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	Release();

	AddGo(new SpriteGo("graphics/background_resize.png", "TitleBackground"));
	AddGo(new TextGo("Title", "fonts/zombiecontrol.ttf"));

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneTitle::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneTitle::Enter()
{
	Scene::Enter();

	SpriteGo* titleBackground = (SpriteGo*)FindGo("TitleBackground");
	TextGo* titleText = (TextGo*)FindGo("Title");

	// 배경
	titleBackground->SetOrigin(Origins::MC);
	titleBackground->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);

	// 텍스트
	titleText->text.setFont(*RESOURCE_MGR.GetFont("fonts/zombiecontrol.ttf"));
	titleText->text.setString("PRESS ENTER TO PLAY");
	titleText->text.setCharacterSize(75);
	titleText->text.setFillColor(sf::Color::White);
	titleText->SetOrigin(Origins::MC);
	titleText->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MGR.ChangeScene(SceneId::Dev1);
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
