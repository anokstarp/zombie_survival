#include "stdafx.h"
#include "SceneDev1.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Player.h"
#include "VertexArrayGo.h"
#include "Framework.h"
#include "Zombie.h"
#include "SpriteEffect.h"

SceneDev1::SceneDev1()
	: Scene(SceneId::Dev1), player(nullptr)
{
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/player.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/background_sheet.png"));

	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/bloater.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/chaser.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/crawler.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/bullet.png"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/blood.png"));
}

SceneDev1::~SceneDev1()
{
	Release();
}

void SceneDev1::Init()
{
	Release();

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	sf::Vector2f tileWorldSize = { 50.f, 50.f };
	sf::Vector2f tileTexSize = { 50.f, 50.f };

	player = (Player*)AddGo(new Player("graphics/player.png", "Player"));
	VertexArrayGo* background = CreateBackground({ 50, 50 }, tileWorldSize, tileTexSize, "graphics/background_sheet.png");
	AddGo(background);
	
	for (auto go : gameObjects)
	{
		go->Init();
	}

	background->sortLayer = -1;
	background->SetOrigin(Origins::MC);
	background->SetPosition(0.f, 0.f);

	wallBounds = background->vertexArray.getBounds();
	wallBounds.width -= tileWorldSize.x * 2.f;
	wallBounds.height -= tileWorldSize.y * 2.f;
	wallBounds.left += tileWorldSize.x;
	wallBounds.top += tileWorldSize.y;

	player->SetWallBounds(wallBounds);

	zombiePool.OnCreate = [this](Zombie* zombie) {
		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes);
		zombie->SetType(zombieType);
		zombie->SetPlayer(player);
	};
	zombiePool.Init();

	bloodEffectPool.OnCreate = [this](SpriteEffect* effect)
	{
		effect->textureId = "graphics/blood.png";
		effect->SetDuration(3.f);
		effect->SetPool(&bloodEffectPool);
	};
	bloodEffectPool.Init();
}

void SceneDev1::Release()
{
	zombiePool.Release();
	bloodEffectPool.Release();

	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneDev1::Enter()
{
	Scene::Enter();

	isGameOver = false;
	player->SetPosition(0.f, 0.f);
}

void SceneDev1::Exit()
{
	//ClearZombies();
	ClearObjectPool(zombiePool);
	ClearObjectPool(bloodEffectPool);

	player->Reset();

	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	//2023-07-07 이남석
	//Scene::Update 위치 수정, 사망시 키 입력 받아서 초기화
	if (isGameOver)
	{
		std::cout << "죽음" << std::endl;
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Y))
		{
			SCENE_MGR.ChangeScene(sceneId);
			return;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::N))
		{
			SCENE_MGR.ChangeScene(sceneId);
			return;
		}
		
		//SCENE_MGR.ChangeScene(sceneId);
		return;
	}

	Scene::Update(dt);
	////////////////////////////////////////////////////

	worldView.setCenter(player->GetPosition());

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(sceneId);
		return;
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnZombies(100, player->GetPosition(), 500.f);
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		//ClearZombies();
		ClearObjectPool(zombiePool);
	}
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

VertexArrayGo* SceneDev1::CreateBackground(sf::Vector2i size, sf::Vector2f tileSize, sf::Vector2f texSize, std::string textureId)
{
	VertexArrayGo* background = new VertexArrayGo(textureId, "Background");

	background->vertexArray.setPrimitiveType(sf::Quads);
	background->vertexArray.resize(size.x * size.y * 4);

	sf::Vector2f startPos = { 0, 0 };
	sf::Vector2f offsets[4] =
	{
		{ 0.f, 0.f },
		{ tileSize.x, 0.f },
		{ tileSize.x, tileSize.y },
		{ 0.f, tileSize.y }
	};

	sf::Vector2f texOffsets[4] =
	{
		{ 0.f, 0.f },
		{ texSize.x, 0.f },
		{ texSize.x, texSize.y },
		{ 0.f, texSize.y }
	};

	sf::Vector2f currPos = startPos;
	for (int i = 0; i < size.y; ++i)
	{
		for (int j = 0; j < size.x; ++j)
		{
			int texIndex = 3;
			if (i != 0 && i != size.y - 1 && j != 0 && j != size.x - 1)
			{
				texIndex = Utils::RandomRange(0, 3);
			}

			int tileIndex = size.x * i + j;
			for (int k = 0; k < 4; ++k)
			{
				int vertexIndex = tileIndex * 4 + k;
				background->vertexArray[vertexIndex].position = currPos + offsets[k];
				background->vertexArray[vertexIndex].texCoords = texOffsets[k];
				background->vertexArray[vertexIndex].texCoords.y += texSize.y * texIndex;
			}
			currPos.x += tileSize.x;
		}
		currPos.x = startPos.x;
		currPos.y += tileSize.y;
	}

	return background;
}

void SceneDev1::SpawnZombies(int count, sf::Vector2f center, float radius)
{
	for (int i = 0; i < count; ++i)
	{
		Zombie* zombie = zombiePool.Get();
		sf::Vector2f pos;
		do
		{
			pos = center + Utils::RandomInCircle(radius);
		}
		while (Utils::Distance(center, pos) < 100.f && radius > 100.f);

		zombie->SetPosition(pos);
		zombie->sortLayer = 1;
		AddGo(zombie);
	}
}

void SceneDev1::ClearZombies()
{
	for (auto zombie : zombiePool.GetUseList())
	{
		RemoveGo(zombie);
	}
	zombiePool.Clear();
}

void SceneDev1::OnDieZombie(Zombie* zombie)
{
	SpriteEffect* blood = bloodEffectPool.Get();
	blood->SetPosition(zombie->GetPosition());
	blood->sortLayer = 0;
	AddGo(blood);

	RemoveGo(zombie);
	zombiePool.Return(zombie);
	//핏자국 추가
}

void SceneDev1::OnDiePlayer()
{
	isGameOver = true;
	
	
	//SCENE_MGR.ChangeScene(sceneId);
}

const std::list<Zombie*>* SceneDev1::GetZombieList() const
{
	return &zombiePool.GetUseList();
}
