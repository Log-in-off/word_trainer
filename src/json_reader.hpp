#pragma once

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


class JsonReader final {
public:
    JsonReader(std::string configName = "config.json");
    std::string getName() const;
    std::string getSheet() const;
    std::vector<char> getListAllColumns() const;
    std::vector<char> getListCheckedColumns() const;
    std::vector<char> getListPromptColumns() const;
    std::vector<char> getListTranslateColumns() const;
private:
    void getElement(json& data,  const std::string &profile, const std::string &name, std::string &elem);
    void getElements(json& data, const std::string &profile, const std::string &name, std::vector<char> &container);
    std::string name_{};
    std::string sheet_{};
    std::vector<char> allColumns_{};
    std::vector<char> checkedColumns_{};
    std::vector<char> promptColumns_{};
    std::vector<char> translateColumns_{};
};
