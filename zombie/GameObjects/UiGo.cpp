#include "stdafx.h"
#include "UiGo.h"
#include "ResourceMgr.h"

UiGo::UiGo(const std::string n, const std::string rId)
	: GameObject(n)
{
	sortLayer = 100;
}

UiGo::~UiGo()
{
}

void UiGo::Init()
{
	GameObject::Init();

}

void UiGo::Release()
{
	GameObject::Release();
}

void UiGo::Reset()
{
	GameObject::Reset();
}

void UiGo::Update(float dt)
{
	GameObject::Update(dt);
}

void UiGo::Draw(sf::RenderWindow& window)
{
	GameObject::Draw(window);
}
