#pragma once
#include "TextGo.h"
#include "SpriteGo.h"

class UiGo :
    public GameObject
{
protected:
	TextGo textUi;
	SpriteGo spriteUi;

public:
	UiGo(const std::string n = "", const std::string rId = "");
	virtual ~UiGo() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

