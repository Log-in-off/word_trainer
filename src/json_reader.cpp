#include "json_reader.hpp"

#include <fstream>

JsonReader::JsonReader(std::string configName){
    std::ifstream f(configName);
    json data = json::parse(f);

    GetElement(data, "fileName", name_);
    GetElement(data, "sheet", sheet_);

    GetElements(data, "allColumns", allColumns_);
    GetElements(data, "checkedColumns", checkedColumns_);
    GetElements(data, "translateColumns", translateColumns_);
    GetElements(data, "promptColumns", promptColumns_);
}

void JsonReader::GetElement(json& data, std::string name, std::string& elem) {
    auto tmp = data.find(name);
    if (tmp != data.end()) {
        elem = tmp.value();
    }
}

void JsonReader::GetElements(json& data, std::string name, std::vector<char>& container) {
    auto array = data.find(name);
    if (array != data.end()) {
        for(auto ch : array.value()) {
            container.push_back(std::string(ch).at(0));
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

std::vector<char> JsonReader::GetListPromptColumns() const {
    return promptColumns_;
}

std::vector<char> JsonReader::GetListTranslateColumns() const {
    return translateColumns_;    
}