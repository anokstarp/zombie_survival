#pragma once
#include "DataTable.h"
class StringTable :
    public DataTable
{
protected:
    std::vector<std::unordered_map<std::string, std::string>> tables;
public:
    StringTable() : DataTable(StringTable::Ids::String), tables((int)Languages::Count) {}
    virtual ~StringTable() override { Release(); }

    bool Load() override;
    void Release() override;

    const std::string& Get(const std::string& id, Languages lang)const;
    const std::string& Get(const std::string& id)const 
    {
        return Get(id, Variables::CurrentLang);
    }
};

