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
private:
    std::string name_{};
    std::string sheet_{};
    std::vector<char> allColumns_{};
    std::vector<char> checkedColumns_{};
};
