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
};

