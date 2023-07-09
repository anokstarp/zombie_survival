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

	float speed = 300.f;
	float addSpeed = 0.f;
	int maxHp = 100;
	int hp = 0;
	int addHp = 0;

	// ±è¹ÎÁö, 230709, »ç¿îµå Ãß°¡
	sf::Sound sound;
	///////////////////////////

	int projectile = 1;

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

	void OnHitted(int damage);
	void OnDie();

	// ±è¹ÎÁö, 230708, hpBar ±¸Çö¿ë get ÇÔ¼ö
	int GetHp();
	//////////////////////////////////////

	//2023-07-09 ÀÌ³²¼®
	void IncreaseHealth(int hp);
	void IncreaseSpeed(float speed);
	void IncreaseProjectile(int num);

};

