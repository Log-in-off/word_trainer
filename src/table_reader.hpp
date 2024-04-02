#pragma once

#include <vector>
#include <string>
#include <string_view>

class TableReader final{
public:
    typedef std::vector<std::vector<std::string>>::const_iterator cIterator; 
    void Read(std::string configName = "config.json");
    int GetMaxLength() const;
    std::vector<int> GetIndexesCheckedColumns() const;
    cIterator cbegin() const;
    cIterator cend() const ;

private:
    void MakeIndexesCheckedColumns(const std::vector<char>& columns, const std::vector<char>& checkedColumns);
    std::vector<std::vector<std::string>> table_{};
    std::vector<int> indexesCheckedColumns_{};
    int maxLength_{};
};
