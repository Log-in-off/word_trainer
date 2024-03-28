#pragma once

#include <vector>
#include <string>
#include <string_view>

class TableReader final{
public:
    typedef std::vector<std::vector<std::string>>::const_iterator cIterator; 
    void Read(const std::string_view fileName, const std::string_view sheet, const std::vector<char>& allColumns, const std::vector<char>& checkedColumns);
    void Print();
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
