#include "stdafx.h"
#include "StringTable.h"
#include "3rd/rapidcsv.h"

bool StringTable::Load()
{
	std::vector<std::string> fileNames;
	fileNames.push_back("tables/stringTable_Kor.csv");
	fileNames.push_back("tables/stringTable_Eng.csv");
	fileNames.push_back("tables/stringTable_Jp.csv");

	for (int i = 0; i < fileNames.size(); i++)
	{
		rapidcsv::Document doc(fileNames[i]);
		std::vector<std::string> ids = doc.GetColumn<std::string>(0);
		std::vector<std::string> values = doc.GetColumn<std::string>(1);

		for (int j = 0; j < ids.size(); j++)
		{
			//std::cout << ids[i] << " / " << values[i] << std::endl;
			tables[i].insert({ ids[j], values[j] });
		}
	}

	return true;
}

void StringTable::Release()
{
	for (auto table : tables)
	{
		table.clear();
	}
	//tables.clear();
}

const std::string& StringTable::Get(const std::string& id, Languages lang) const
{
	auto& table = tables[(int)lang];
	auto find = table.find(id);
	if (find == table.end())
	{
		return "ERR: UndeFind";
	}
	return find->second;
}
