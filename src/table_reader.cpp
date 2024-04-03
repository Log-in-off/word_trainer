#include "table_reader.hpp"

#include <OpenXLSX.hpp>

#include "showing.hpp"
#include "json_reader.hpp"

std::vector<int> getIndexColumns(const std::vector<char>& columns) {
    std::vector<int> res{};
    for (auto column:columns) {
        res.push_back(column - 'A' + 1);
    }
    return res;
}

int TableReader::getMaxLength() const {
    return maxLength_;
}

void TableReader::read(std::string configName) {
    JsonReader json(configName);
    const std::string fileName  = json.getName();
    const std::string sheet = json.getSheet();
    const std::vector<char> allColumns = json.getListAllColumns();
    const std::vector<char> checkedColumns = json.getListCheckedColumns();
    const std::vector<char> translateColumns = json.getListTranslateColumns();
    const std::vector<char> promptColumns = json.getListPromptColumns();

    const std::vector<int> numbersOfColumns = getIndexColumns(allColumns);

    indexesCheckedColumns_ = makeIndexesColumns(allColumns, checkedColumns);
    indexesPromptColumns_ = makeIndexesColumns(allColumns, promptColumns);

    OpenXLSX::XLDocument doc;
    doc.open(fileName.data());
    auto wb = doc.workbook().worksheet(sheet.data());
    
    maxLength_ = 0;
    table_.clear();
    int row = 1;
    bool end_document = false;
    while(!end_document) {
        for(auto column:numbersOfColumns) {
            std::string field = wb.cell(row, column).value().get<std::string>();
            if (field.empty()) {
                if (column == *numbersOfColumns.begin()) {
                    end_document = true;
                    break;
                }
            }

            if (column == *numbersOfColumns.begin())
                table_.push_back({field});
            else {
                const auto countLetters = getCountLetters(field);
                maxLength_ = maxLength_ > countLetters ? maxLength_ : countLetters;
                table_.back().push_back(field);
            }
        }
        row++;
    }
}

std::vector<int> TableReader::getIndexesCheckedColumns() const {
    return indexesCheckedColumns_;
}

std::vector<int> TableReader::getIndexesPromptColumns() const {
    return indexesPromptColumns_;
}

std::vector<int> TableReader::makeIndexesColumns(const std::vector<char>& allColumns, const std::vector<char>& checkedColumns) {
    std::vector<int> res{};
    for (auto colum:checkedColumns) {
        if (allColumns.end() != std::find(allColumns.begin(), allColumns.end(), colum)) {
            res.push_back(colum - allColumns.front());
        }
    }
    return res;
}

TableReader::cIterator TableReader::cbegin() const {
    return table_.cbegin();
}

TableReader::cIterator TableReader::cend() const {
    return table_.cend();
}