#pragma once
#include "DataTable.h"
#include "Zombie.h"

struct ZombieInfo
{
    Zombie::Types zombieTypes;
    std::string textureId;
    float speed = 0.f;
    int maxHp = 0;
    int damage = 0;
    float attackRate = 0.f;
    /*
    ZombieInfo(Zombie::Types zombieTypes, std::string textureId, float speed, int maxHp, int damage, float attackRate)
        :zombieTypes(zombieTypes), textureId(textureId), speed(speed), maxHp(maxHp), damage(damage), attackRate(attackRate) {}
    */
};

class ZombieTable :
    public DataTable
{
protected:
    std::vector<std::map<std::string, ZombieInfo>> tables;
public:
    ZombieTable() : DataTable(Ids::Zombie) {}
    virtual ~ZombieTable() override = default;

    const ZombieInfo& Get(const std::string id) const;

    // DataTable을(를) 통해 상속됨
    virtual bool Load() override;
    virtual void Release() override;
};

