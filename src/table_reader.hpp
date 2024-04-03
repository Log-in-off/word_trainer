#pragma once

#include <vector>
#include <string>
#include <string_view>

class TableReader final{
public:
    typedef std::vector<std::vector<std::string>>::const_iterator cIterator; 
    void read(std::string configName = "config.json");
    int getMaxLength() const;
    std::vector<int> getIndexesCheckedColumns() const;
    std::vector<int> getIndexesPromptColumns() const;
    cIterator cbegin() const;
    cIterator cend() const ;

private:
    std::vector<int> makeIndexesColumns(const std::vector<char>& columns, const std::vector<char>& checkedColumns);
    std::vector<std::vector<std::string>> table_{};
    std::vector<int> indexesCheckedColumns_{};
    std::vector<int> indexesPromptColumns_{};
    int maxLength_{};
};
