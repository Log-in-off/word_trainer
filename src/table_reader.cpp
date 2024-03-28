#include "table_reader.hpp"

#include <OpenXLSX.hpp>

#include "showing.hpp"

void TableReader::Print() {
    const std::string space(30, ' ');
    for(const auto & row : table_ )
    {
        for(uint8_t i = 0; i < row.size(); i++)
        {
            //std::cout << std::string_view(row[i]+space).substr(0, offset[i]+1);
            std::cout << std::string_view(row[i]+" ") << "|";
        }
        std::cout << std::endl << "________________________________________________________________________\n";
    }
}

int TableReader::GetMaxLength() const{
    return maxLength_;
}

void TableReader::Read(const std::string_view fileName, const std::string_view sheet, const std::vector<char>& allColumns, 
                                                                                      const std::vector<char>& checkedColumns) {
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