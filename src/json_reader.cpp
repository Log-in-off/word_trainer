#include "json_reader.hpp"

#include <fstream>

JsonReader::JsonReader(std::string configName){
    std::ifstream f(configName);
    json data = json::parse(f);

    auto fileName = data.find("fileName");
    if (fileName != data.end()) {
        name_ = fileName.value();
    }

    auto fileSheet = data.find("sheet");
    if (fileSheet != data.end()) {
        sheet_ = fileSheet.value();
    }

    auto fileColumns = data.find("allColumns");
    if (fileColumns != data.end()) {
        for(auto ch : fileColumns.value()) {
            allColumns_.push_back(std::string(ch).at(0));
        }
    }

    auto fileCheckedColumns = data.find("checkedColumns");
    if (fileCheckedColumns != data.end()) {
        for(auto ch : fileCheckedColumns.value()) {
            checkedColumns_.push_back(std::string(ch).at(0));
        }
    }
}

std::string JsonReader::GetName() const {
    return name_;
}

std::string JsonReader::GetSheet() const {
    return sheet_;
}

std::vector<char> JsonReader::GetListAllColumns() const {
    return allColumns_;
}

std::vector<char> JsonReader::GetListCheckedColumns() const {
    return checkedColumns_;
}