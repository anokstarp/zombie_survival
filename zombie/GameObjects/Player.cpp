#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "SceneDev1.h"
#include "Utils.h"

Player::Player(const std::string& textureId = "", const std::string& n = "")
	: SpriteGo(textureId, n)
{
}

void Player::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);

	poolBullets.OnCreate = [this](Bullet* bullet) {
		bullet->textureId = "graphics/bullet.png";
		bullet->pool = &poolBullets;
	};

	poolBullets.Init();
}

void Player::Release()
{
	SpriteGo::Release();

	poolBullets.Release();
}

void Player::Reset()
{
	SpriteGo::Reset();

	isAlive = true;
	hp = maxHp;
	
	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}
	poolBullets.Clear();

	//230707장다훈
	//탄약 옵션 초기화
	magCapacity = 30;
	remainAmmo = 210;
	reloadTimer = 0.f;
	reloadingSpeed = 0.3f;
	curAmmo = magCapacity;
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	//230707장다훈
	//장전중일때 총안나오게 reloadTimer 0 이하로 안내려가게
	if(reloadTimer >= 0)
		reloadTimer -= dt;

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
	sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(position);

	// 회전
	look = Utils::Normalize(mousePos - playerScreenPos);
	sprite.setRotation(Utils::Angle(look));

	// 이동
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxis(Axis::Vertical);
	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}

	position += direction * speed * dt;
	if (!wallBounds.contains(position))
	{
		position = Utils::Clamp(position, wallBoundsLT, wallBoundsRB);
	}
	sprite.setPosition(position);
	/*
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
	{
		Bullet* bullet = poolBullets.Get();
		bullet->Fire(GetPosition(), look, 1000.f);

		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
		if (sceneDev1 != nullptr)
		{
			bullet->SetZombieList(sceneDev1->GetZombieList());
			sceneDev1->AddGo(bullet);
		}
	}
	*/
	//230707 장다훈
	//원문 위 주석에 있음, 마우스 클릭, 탄창 내 탄 수, 재장전중 점검
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left) && reloadTimer < 0 && curAmmo > 0)
	{		
		Bullet* bullet = poolBullets.Get();
		bullet->Fire(GetPosition(), look, 1000.f);
		curAmmo--;

		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
		if (sceneDev1 != nullptr)
		{
			bullet->SetZombieList(sceneDev1->GetZombieList());
			sceneDev1->AddGo(bullet);
		}		
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::R))
	{
		ReloadMagazine();
	}
	//test//std::cout<< curAmmo << "/" << remainAmmo << "/" << reloadTimer <<std::endl;
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Player::SetWallBounds(const sf::FloatRect& bounds)
{
	wallBounds = bounds;

	wallBoundsLT = { wallBounds.left, wallBounds.top };
	wallBoundsRB = { wallBounds.left + wallBounds.width, wallBounds.top + wallBounds.height };
}

void Player::ReloadMagazine()
{
	reloadTimer = reloadingSpeed;

	if (remainAmmo + curAmmo == 0)
		return;
	int tempAmmo = remainAmmo + curAmmo - magCapacity;// 연산을 위한 임시객체
	remainAmmo = std::max(tempAmmo, 0); // 총 남은 탄 수가 0보다 적다면 remainAmmo를 0으로 설정
	curAmmo = remainAmmo == 0 ? magCapacity + tempAmmo : magCapacity; // remainAmmo이 0 이면 총 남은 탄 수를 curAmmo로 설정 아니면 magCapacity를 curAmmo로 설정

}

void Player::AddHealth(int amount)
{
	hp += amount;
	if (hp > maxHp)
		hp = maxHp;	
}

void Player::OnHitted(int damage)
{
	if (!isAlive)
		return;

	hp = std::max(hp - damage, 0);
	if (hp == 0)
	{
		OnDie();
	}
}

void Player::OnDie()
{
	isAlive = false;

	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	if (sceneDev1 != nullptr)
	{
		sceneDev1->OnDiePlayer();
	}
}
