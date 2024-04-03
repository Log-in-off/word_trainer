#include "json_reader.hpp"

#include <fstream>

JsonReader::JsonReader(std::string configName) {
    std::ifstream f(configName);
    json data = json::parse(f);

    getElement(data, "fileName", name_);
    getElement(data, "sheet", sheet_);

    getElements(data, "allColumns", allColumns_);
    getElements(data, "checkedColumns", checkedColumns_);
    getElements(data, "translateColumns", translateColumns_);
    getElements(data, "promptColumns", promptColumns_);
}

void JsonReader::getElement(json& data, std::string name, std::string& elem) {
    auto tmp = data.find(name);
    if (tmp != data.end()) {
        elem = tmp.value();
    }
}

void JsonReader::getElements(json& data, std::string name, std::vector<char>& container) {
    auto array = data.find(name);
    if (array != data.end()) {
        for(auto ch : array.value()) {
            container.push_back(std::string(ch).at(0));
        }
    }
}

std::string JsonReader::getName() const {
    return name_;
}

std::string JsonReader::getSheet() const {
    return sheet_;
}

std::vector<char> JsonReader::getListAllColumns() const {
    return allColumns_;
}

std::vector<char> JsonReader::getListCheckedColumns() const {
    return checkedColumns_;
}

std::vector<char> JsonReader::getListPromptColumns() const {
    return promptColumns_;
}

std::vector<char> JsonReader::getListTranslateColumns() const {
    return translateColumns_;    
}