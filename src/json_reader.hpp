#pragma once

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


class JsonReader final {
public:
    JsonReader(std::string configName = "config.json");
    std::string GetName() const;
    std::string GetSheet() const;
    std::vector<char> GetListAllColumns() const;
    std::vector<char> GetListCheckedColumns() const;
    std::vector<char> GetListPromptColumns() const;
    std::vector<char> GetListTranslateColumns() const;
private:
    void GetElement(json& data, std::string name, std::string& elem);
    void GetElements(json& data, std::string name, std::vector<char>& container);
    std::string name_{};
    std::string sheet_{};
    std::vector<char> allColumns_{};
    std::vector<char> checkedColumns_{};
    std::vector<char> promptColumns_{};
    std::vector<char> translateColumns_{};
};
