#pragma once
#include <map>

class DataTable
{
public:
	enum class Ids
	{
		None = -1,
		String,
		Zombie,
	};
protected:
	Ids tableId = Ids::None;

	DataTable (const DataTable& other) = delete;
	bool operator==(const DataTable& other) const = delete;
public:
	static const int TotalIds = 1;

	DataTable(DataTable::Ids id) :tableId(id) {}
	virtual ~DataTable() { Release(); }

	virtual bool Load() = 0;
	virtual void Release() {};
};

