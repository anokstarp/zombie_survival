#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "Bullet.h"

class Player : public SpriteGo
{
protected:
	sf::Vector2f look;
	sf::Vector2f direction;

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

	float speed = 500.f;
	int maxHp = 100;
	int hp = 0;

	//230707 장다훈
	//탄 관련 옵션
	int curAmmo; 
	int magCapacity;
	int remainAmmo;
	float reloadingSpeed;
	float reloadTimer;

	ObjectPool<Bullet> poolBullets;

public:
	Player(const std::string& textureId, const std::string& n);

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetWallBounds(const sf::FloatRect& bounds);

	bool isAlive = false;
	//230707 장다훈
	//재장전 구현
	void ReloadMagazine();
	//230710 장다훈
	//아이템을 이용한 값 수정 및 획득
	void AddAmmo(int amount) { remainAmmo += amount; }
	void AddHealth(int amount);
	int GetAmmo() { return remainAmmo + curAmmo; }
	int GetHealth() { return hp; }

	void OnHitted(int damage);
	void OnDie();
};

