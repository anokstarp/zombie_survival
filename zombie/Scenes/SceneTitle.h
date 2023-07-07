#pragma once
#include "Scene.h"
class SceneTitle :
    public Scene
{
protected:

public:
	SceneTitle(SceneId id = SceneId::Title);
	virtual ~SceneTitle();

	virtual void Init() = 0;
	virtual void Release() = 0;

	virtual void Enter();
	virtual void Exit();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);
};

