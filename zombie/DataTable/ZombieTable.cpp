#include "stdafx.h"
#include "ZombieTable.h"
#include "3rd/rapidcsv.h"

const ZombieInfo& ZombieTable::Get(const int id) const
{
	auto& table = tables[0];	
	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "ERROR : ZOMBIE_TABLE" << std::endl;
		//exit(1);
	}
	return find->second;
}

bool ZombieTable::Load()
{
	std::vector<std::string> filenames;
	filenames.push_back("tables/ZombieTable.csv");

	for (int i = 0; i < filenames.size(); i++)
	{
		rapidcsv::Document doc(filenames[i]);
		std::vector<int> ids = doc.GetColumn<int>(0);
		std::vector<std::string> textureId = doc.GetColumn<std::string>(1);
		std::vector<float> speedStats = doc.GetColumn<float>(2);
		std::vector<int> hpStats = doc.GetColumn<int>(3);
		std::vector<int> damageStats = doc.GetColumn<int>(4);
		std::vector<float> attackRateStats = doc.GetColumn<float>(5);
		for (int j = 0; j < ids.size(); j++)
		{
			ZombieInfo zombieInfo;
			zombieInfo.textureId = textureId[j];
			zombieInfo.speed = speedStats[j];
			zombieInfo.maxHp = hpStats[j];
			zombieInfo.damage = damageStats[j];
			zombieInfo.attackRate = attackRateStats[j];
			tables[i].insert({ ids[j], zombieInfo});
		}
	}
	return true;
}

void ZombieTable::Release()
{
	for (auto table : tables)
	{
		table.clear();
	}
}
