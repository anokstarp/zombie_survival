#include "stdafx.h"
#include "SceneTitle.h"
#include "ResourceMgr.h"
#include "TextGo.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "SpriteGo.h"
#include "Framework.h"
// ±ËπŒ¡ˆ, 230709
#include "SceneDev1.h"
#include "DataTableManager.h"
#include "StringTable.h"
////////////////

SceneTitle::SceneTitle(SceneId id)
	: Scene(SceneId::Title)
{
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background_resize.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Font, "fonts/DOSPilgi.ttf"));
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	Release();

	AddGo(new SpriteGo("graphics/background_resize.png", "TitleBackground"));
	AddGo(new TextGo("Title", "fonts/DOSPilgi.ttf"));
	AddGo(new TextGo("hiScore", "fonts/zombiecontrol.ttf"));

	for (auto go : gameObjects)
	{
		go->Init();
	}

	// ≈∏¿Ã∆≤ ∫‰
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;
	titleView.setSize(windowSize);
	titleView.setCenter(centerPos);
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
	auto stringTable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);

	SpriteGo* titleBackground = (SpriteGo*)FindGo("TitleBackground");
	TextGo* titleText = (TextGo*)FindGo("Title");
	TextGo* hiScore = (TextGo*)FindGo("hiScore");

	// πË∞Ê
	titleBackground->SetOrigin(Origins::MC);
	titleBackground->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);

	// ≈ÿΩ∫∆Æ

	titleText->text.setString(stringTable->Get("TITLE"));
	titleText->text.setCharacterSize(75);
	titleText->text.setFillColor(sf::Color::White);
	titleText->SetOrigin(Origins::MC);
	titleText->SetPosition(FRAMEWORK.GetWindowSize() / 2.f);

	// hiScore
	Scene* scene = SCENE_MGR.GetGameScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	if (sceneDev1 != nullptr)
	{
		int hiScoreNum = sceneDev1->GetHiScore();
		std::stringstream ss;
		ss << "HI SCORE:" << hiScoreNum;
		hiScore->text.setString(ss.str());
	}
	hiScore->text.setCharacterSize(50);
	hiScore->text.setFillColor(sf::Color::White);
	hiScore->SetOrigin(Origins::TL);
	hiScore->SetPosition(FRAMEWORK.GetWindowSize().x - 300.f, 10.f);
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
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Numpad1))
	{
		ChangeLang(Languages::KOR);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Numpad2))
	{
		ChangeLang(Languages::ENG);
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	window.setView(titleView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer >= 100)
			continue;

		if (go->GetActive())
		{
			go->Draw(window);
		}
	}
}

void SceneTitle::ChangeLang(Languages lang)
{
	auto stringData = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	TextGo* titleText = (TextGo*)FindGo("Title");

	titleText->text.setString(stringData->Get("TITLE", lang));
}
