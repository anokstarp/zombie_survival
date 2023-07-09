#include "stdafx.h"
#include "SpriteItem.h"
#include "Player.h"
const std::string SpriteItem::TextureIds[2] =
{
	"graphics/ammo_icon.png",
	"graphics/health_pickup.png",
};

const SpriteItem::EffectFunction SpriteItem::Effects[2] =
{
	[](Player* player, int amount) {player->AddAmmo(amount); },
	[](Player* player, int amount) {player->AddHealth(amount); },
};
int SpriteItem::Amounts[2] = {25, 120};

SpriteItem::SpriteItem(const std::string& n)
	:SpriteGo("", n)
{
}

SpriteItem::~SpriteItem()
{
}

void SpriteItem::Reset()
{
	int ammo = player->GetAmmo();
	int hp = player->GetHealth();
	std::cout << "아이템 리셋" << std::endl;
	if (ammo < 50)
	{
		SetType(Types::Ammo);
	}
	else if(hp < 20)
	{
		SetType(Types::MediKit);
	}
	else if(ammo < Utils::RandomRange(50, 1000))
	{
		SetType(Types::Ammo);
	}
	else
	{
		SetType(Types::MediKit);
	}
	SpriteGo::Reset();
}

void SpriteItem::SetType(Types type)
{
	std::cout << "아이템 셋 타입" << std::endl;
	itemType = type;
	int index = (int)itemType;
	textureId = TextureIds[index];
	amount = Amounts[index];
}

void SpriteItem::AddAmount(Types type, int amount)
{
	SpriteItem::Amounts[(int)type] += amount;
}

void SpriteItem::IsCollidedWithItem()
{
	Effects[(int)itemType](player, amount);
}

void SpriteItem::SetPlayer(Player* player)
{
	this->player = player;
}

void SpriteItem::ResetAmount()
{
	SpriteItem::Amounts[0] = 25;
	SpriteItem::Amounts[1] = 120;
}
