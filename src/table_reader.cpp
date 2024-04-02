#include "table_reader.hpp"

#include <OpenXLSX.hpp>

#include "showing.hpp"
#include "json_reader.hpp"

int TableReader::GetMaxLength() const{
    return maxLength_;
}

void TableReader::Read(std::string configName) {
    JsonReader json(configName);
    const std::string fileName  = json.GetName();
    const std::string sheet = json.GetSheet();
    const std::vector<char> allColumns = json.GetListAllColumns();
    const std::vector<char> checkedColumns = json.GetListCheckedColumns();
    const std::vector<char> translateColumns = json.GetListTranslateColumns();
    const std::vector<char> promptColumns = json.GetListTranslateColumns();

    OpenXLSX::XLDocument doc;
    doc.open(fileName.data());
    auto wb = doc.workbook().worksheet(sheet.data());
    std::vector<int> numbersOfColumns;
    numbersOfColumns.reserve(allColumns.size());
    for (auto column:allColumns) {
        numbersOfColumns.push_back(column - 'A' + 1);
    }

    maxLength_ = 0;
    table_.clear();
    int row = 1;
    bool end_document = false;
    while(!end_document) {
        for(auto column:numbersOfColumns) {
            std::string field = wb.cell(row, column).value().get<std::string>();
            if (field.empty())
            {
                if (column == *numbersOfColumns.begin()) {
                    end_document = true;
                    break;
                }
            }

            if (column == *numbersOfColumns.begin())
                table_.push_back({field});
            else 
            {
                const auto countLetters = count_letters(field);
                maxLength_ = maxLength_ > countLetters ? maxLength_ : countLetters;
                table_.back().push_back(field);
            }
        }
        row++;
    }
    MakeIndexesCheckedColumns(allColumns, checkedColumns);
}

std::vector<int> TableReader::GetIndexesCheckedColumns() const {
    return indexesCheckedColumns_;
}

void TableReader::MakeIndexesCheckedColumns(const std::vector<char>& allColumns, const std::vector<char>& checkedColumns) {
    for (auto colum:checkedColumns) {
        if (allColumns.end() != std::find(allColumns.begin(), allColumns.end(), colum))
        {
            indexesCheckedColumns_.push_back(colum - allColumns.front());
        }
    }
}

TableReader::cIterator TableReader::cbegin() const {
    return table_.cbegin();
}

TableReader::cIterator TableReader::cend() const {
    return table_.cend();
}