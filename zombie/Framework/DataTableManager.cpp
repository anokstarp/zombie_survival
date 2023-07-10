#include "stdafx.h"
#include "DataTableManager.h"
#include "StringTable.h"
#include "ZombieTable.h"

void DataTableManager::LoadAll()
{
	//StringTable
	tables.insert({ DataTable::Ids::String, new StringTable() });
	tables.insert({ DataTable::Ids::Zombie, new ZombieTable() });

	for (auto pair : tables)
	{
		if (!pair.second->Load())
		{
			std::cout << "ERROR : LoadALL" << std::endl;
		}
	}
}

void DataTableManager::ReleaseAll()
{
	for (auto pair : tables)
	{
		delete pair.second;
	}
	tables.clear();
}
