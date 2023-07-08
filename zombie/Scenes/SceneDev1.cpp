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
// 김민지, 230708
#include "TextGo.h"
#include "RectGo.h"
////////////////

SceneDev1::SceneDev1()
	: Scene(SceneId::Dev1), player(nullptr)
{
	// 김민지, 230708
	resources.push_back(std::make_tuple(ResourceTypes::Font, "fonts/zombiecontrol.ttf"));
	resources.push_back(std::make_tuple(ResourceTypes::Texture, "graphics/ammo_icon.png"));
	////////////////
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

	// 김민지, 230708, ui AddGo
	AddGo(new TextGo("score", "fonts/zombiecontrol.ttf"));
	AddGo(new TextGo("hiScore", "fonts/zombiecontrol.ttf"));
	AddGo(new TextGo("leftBullets", "fonts/zombiecontrol.ttf"));
	AddGo(new TextGo("wave", "fonts/zombiecontrol.ttf"));
	AddGo(new TextGo("leftZombies", "fonts/zombiecontrol.ttf"));
	AddGo(new SpriteGo("graphics/ammo_icon.png", "bulletImg"));
	AddGo(new RectGo("hpBar"));

	AddGo(new TextGo("fps", "fonts/zombiecontrol.ttf"));
	////////////////////////

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

	// 김민지, 230708, ui 세팅
	TextGo* score = (TextGo*)FindGo("score");
	TextGo* hiScore = (TextGo*)FindGo("hiScore");
	TextGo* leftBullets = (TextGo*)FindGo("leftBullets");
	TextGo* wave = (TextGo*)FindGo("wave");
	TextGo* leftZombies = (TextGo*)FindGo("leftZombies");
	SpriteGo* bulletImg = (SpriteGo*)FindGo("bulletImg");
	RectGo* hpBar = (RectGo*)FindGo("hpBar");
	TextGo* fps = (TextGo*)FindGo("fps");

	sf::Vector2f screenSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = screenSize * 0.5f;

	this->score = 0;
	std::stringstream ss;
	ss << "SCORE:" << this->score;
	score->text.setString(ss.str());
	score->text.setCharacterSize(50);
	score->text.setFillColor(sf::Color::White);
	score->SetOrigin(Origins::TL);
	score->SetPosition(10.f, 10.f);
	score->sortLayer = 100;

	std::stringstream ss2;
	ss2 << "HI SCORE:" << this->hiScore;
	hiScore->text.setString(ss2.str());
	hiScore->text.setCharacterSize(50);
	hiScore->text.setFillColor(sf::Color::White);
	hiScore->SetOrigin(Origins::TL);
	hiScore->SetPosition(screenSize.x - 300.f, 10.f);
	hiScore->sortLayer = 100;

	std::stringstream ss3;
	ss3 << this->leftBullets;
	leftBullets->text.setString(ss3.str());
	leftBullets->text.setCharacterSize(50);
	leftBullets->text.setFillColor(sf::Color::White);
	leftBullets->SetOrigin(Origins::BL);
	leftBullets->SetPosition(100.f, screenSize.y - 10.f);
	leftBullets->sortLayer = 100;

	std::stringstream ss4;
	ss4 << "WAVE:" << this->wave;
	wave->text.setString(ss4.str());
	wave->text.setCharacterSize(50);
	wave->text.setFillColor(sf::Color::White);
	wave->SetOrigin(Origins::BL);
	wave->SetPosition(centerPos.x + 100.f, screenSize.y - 10.f);
	wave->sortLayer = 100;

	std::stringstream ss5;
	ss5 << "ZOMBIES:" << this->leftZombies;
	leftZombies->text.setString(ss5.str());
	leftZombies->text.setCharacterSize(50);
	leftZombies->text.setFillColor(sf::Color::White);
	leftZombies->SetOrigin(Origins::BL);
	leftZombies->SetPosition(screenSize.x - 300.f, screenSize.y - 10.f);
	leftZombies->sortLayer = 100;

	bulletImg->SetOrigin(Origins::BL);
	bulletImg->SetPosition(40.f, screenSize.y);
	bulletImg->sortLayer = 100;

	sf::RectangleShape& hpRect = hpBar->GetRect();
	hpRect.setFillColor(sf::Color::Red);
	hpRect.setSize(sf::Vector2f( 350.f, 40.f ));
	hpBar->SetOrigin(Origins::BL);
	hpBar->SetPosition(250.f, screenSize.y - 10.f);
	hpBar->sortLayer = 100;

	fps->text.setString("FPS:0");
	fps->text.setCharacterSize(50);
	fps->text.setFillColor(sf::Color::White);
	fps->SetOrigin(Origins::TC);
	fps->SetPosition(centerPos.x, 10.f);
	fps->sortLayer = 100;

	/////////////////////////
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
	// 김민지, 230708
	// fps 값 세팅, 출력
	frame++;
	dtTotal += dt;
	if (dtTotal >= 1.f)
	{
		dtTotal -= 1.f;
		std::stringstream ss;
		ss << "FPS:" << frame;
		TextGo* fps = (TextGo*)FindGo("fps");
		fps->text.setString(ss.str());
		frame = 0;
	}
	// fps on/off
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num0))
	{
		TextGo* fps = (TextGo*)FindGo("fps");
		if (fps->GetActive())
		{
			fps->SetActive(false);
		}
		else
		{
			fps->SetActive(true);
		}
	}
	// hpBar 길이 세팅, 출력
	RectGo* hpBar = (RectGo*)FindGo("hpBar");
	sf::RectangleShape& hpRect = hpBar->GetRect();
	float hpBarWidth = (float)player->GetHp() / 100.f;
	hpRect.setSize(sf::Vector2f(350.f * hpBarWidth, 40.f));
	// ui값 세팅 함수
	SetUiData();
	/////////////////////////

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
			SCENE_MGR.ChangeScene(SceneId::Title);
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
	// 김민지, 230708, score 세팅
	score++;
	///////////////////////////

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

void SceneDev1::SetUiData()
{
	TextGo* score = (TextGo*)FindGo("score");
	TextGo* hiScore = (TextGo*)FindGo("hiScore");
	TextGo* leftBullets = (TextGo*)FindGo("leftBullets");
	TextGo* wave = (TextGo*)FindGo("wave");
	TextGo* leftZombies = (TextGo*)FindGo("leftZombies");

	std::stringstream ss;
	ss << "SCORE:" << this->score;
	score->text.setString(ss.str());

	if (this->hiScore < this->score)
	{
		this->hiScore = this->score;
		std::stringstream ss2;
		ss2 << "HI SCORE:" << this->hiScore;
		hiScore->text.setString(ss2.str());
	}

	// leftBullets => 탄약 구현 후 추가
	// wave => 스테이지 구현 후 
}
