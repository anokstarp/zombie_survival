#pragma once
#include "stdafx.h"
#include "SpriteGo.h"

class Player;

class SpriteItem : public SpriteGo
{
protected:	

	Player* player =nullptr;
public:
	SpriteItem(const std::string& textureId, const std::string& n);
	virtual ~SpriteItem() override;

};