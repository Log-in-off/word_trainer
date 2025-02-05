#include "json_reader.hpp"

#include <fstream>
#include <vector>
#include <iostream>

unsigned GetValidChoose(const std::vector<std::string> &profiles) {

    //clean input
    std::cin.clear();
    std::cin.ignore(10000, '\n');

    std::cout << "Choose preferred profile." << std::endl;
    for (auto i = 0; i < profiles.size(); i++)
    {
        std::cout << i + 1 << ") " << profiles[i] << std::endl;
    }

    unsigned profile_number = 0;
    while (true)
    {
        std::cout << "Enter a number: ";
        if (!(std::cin >> profile_number)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Error! Enter a number.\n";
            continue;
        }

        if (profile_number <= profiles.size() && 0 != profile_number)
            break;
        std::cout << "Wrong profile number. Select the correct option." << std::endl;
    }

    return profile_number - 1 ;
}

JsonReader::JsonReader(std::string configName) {
    std::ifstream f(configName);
    json data = json::parse(f);
    std::vector<std::string> keys {};
    for (auto it = data.begin(); it != data.end(); ++it) {
        keys.push_back(it.key());
    }

    unsigned index = 0;
    std::string chosen_case {};
    if (0 == keys.size()) {
        abort();
    } else if (keys.size() > 1 ) {
        index = GetValidChoose(keys);
    }

    const std::string profile = keys[index];

    getElement(data,  profile, "fileName", name_);
    getElement(data,  profile, "sheet", sheet_);
    getElements(data, profile, "allColumns", allColumns_);
    getElements(data, profile, "checkedColumns", checkedColumns_);
    getElements(data, profile, "translateColumns", translateColumns_);
    getElements(data, profile, "promptColumns", promptColumns_);
}

void JsonReader::getElement(json& data, const std::string &profile, const std::string &name, std::string &elem) {
    auto tmp = data.at(profile).find(name);
    if (tmp != data.at(profile).end()) {
        elem = tmp.value();
    }
}

void JsonReader::getElements(json& data, const std::string &profile, const std::string &name, std::vector<char> &container) {
    auto array = data.at(profile).find(name);
    if (array != data.at(profile).end()) {
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