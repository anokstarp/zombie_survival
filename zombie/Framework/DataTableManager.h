#pragma once
#include "DataTable/DataTable.h"
#include "Singleton.h"

class DataTableManager :public Singleton<DataTableManager>
{
protected:
	std::unordered_map<DataTable::Ids, DataTable*> tables;

	DataTableManager(const DataTableManager& other) = delete;
	bool operator==(const DataTableManager& other) = delete;
public:
	DataTableManager() = default;
	~DataTableManager() {ReleaseAll();};

	void LoadAll();
	void ReleaseAll();

	template <typename T>
	T* Get(DataTable::Ids id) const;
};

template<typename T>
inline T* DataTableManager::Get(DataTable::Ids id) const
{
	auto find = tables.find(id);
	if(find ==tables.end())
		return nullptr;
	return dynamic_cast<T*>(find->second);
}

#define DATATABLE_MGR (DataTableManager::Instance())
