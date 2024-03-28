#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <tuple>
#include <string_view>
#include <OpenXLSX.hpp>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
using json = nlohmann::json;

int count_letters(const std::string& s) {
    const int ex0 = 0xffffffcc, ex1 = 0xffffffa9;
    int count = 0;
    for( auto ch:s) {
        if(((ch&0xC0) == 0x000000C0)  || (ch&0x00000080) != 0x80 )
            count++;
        if(int(ch) == ex1 ){
            count--;
        }
    }
    return  count;
}

void test () {
    std::string s = {"[hˈɪdn̩]"};
    std::cout << count_letters(s) << std::endl;
}

class ReaderJson final {
public:
    ReaderJson(std::string configName = "config.json");
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

ReaderJson::ReaderJson(std::string configName){
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

std::string ReaderJson::GetName() const {
    return name_;
}

std::string ReaderJson::GetSheet() const {
    return sheet_;
}

std::vector<char> ReaderJson::GetListAllColumns() const {
    return allColumns_;
}

std::vector<char> ReaderJson::GetListCheckedColumns() const {
    return checkedColumns_;
}

class ReaderTable final{
public:
    typedef std::vector<std::vector<std::string>>::const_iterator cIterator; 
    void Read(const std::string_view fileName, const std::string_view sheet, const std::vector<char>& allColumns, const std::vector<char>& checkedColumns);
    void Print();
    int GetMaxLength() const;
    std::vector<int> GetIndexesCheckedColumns() const;
    cIterator cbegin() const;
    cIterator cend() const ;

private:
    void MakeIndexesCheckedColumns(const std::vector<char>& columns, const std::vector<char>& checkedColumns);
    std::vector<std::vector<std::string>> table_{};
    std::vector<int> indexesCheckedColumns_{};
    int maxLength_{};
};

std::vector<int> ReaderTable::GetIndexesCheckedColumns() const {
    return indexesCheckedColumns_;
}

void ReaderTable::MakeIndexesCheckedColumns(const std::vector<char>& allColumns, const std::vector<char>& checkedColumns) {
    for (auto colum:checkedColumns) {
        if (allColumns.end() != std::find(allColumns.begin(), allColumns.end(), colum))
        {
            indexesCheckedColumns_.push_back(colum - allColumns.front());
        }
    }
}

ReaderTable::cIterator ReaderTable::cbegin() const {
    return table_.cbegin();
}

ReaderTable::cIterator ReaderTable::cend() const {
    return table_.cend();
}

class TrainerWords {
public:
    void start();
    template<typename T>
void shuffleContainer(T& container) {
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(container.begin(), container.end(), rng);
}
private:
    ReaderTable table_;
};

void TrainerWords::start() {
    ReaderJson json;
    
    const std::string name  = json.GetName();
    const std::string sheet = json.GetSheet();
    const std::vector<char> allColumns = json.GetListAllColumns();
    const std::vector<char> checkingColumns = json.GetListCheckedColumns(); 

    table_.Read(name, sheet, allColumns, checkingColumns);
    const std::vector<int> checkedColumns = table_.GetIndexesCheckedColumns();
    
    std::vector<const std::vector<std::string>*> allWord;
    for( auto it = table_.cbegin(), end = table_.cend(); it !=end; it++ ) {
        allWord.push_back(&(*it));
    }

    shuffleContainer(allWord);

    std::random_device dev;
    std::default_random_engine engine(dev());
    
    std::uniform_int_distribution <int> uniform_type(0, checkedColumns.size()-1);

    int count = 0;
    int round = 1;
    while(count != allWord.size())
    {
        count = 0;
        for(auto& words:allWord) {
            if(!words)
            {
                count++;
                continue;
            }
            const int randomFromList = checkedColumns[uniform_type(engine)];
            std::cout << words->at(randomFromList)<< std::endl;

            const size_t max = table_.GetMaxLength() + 1;
            const std::string space(max, ' ');
            std::string line{};
            std::getline(std::cin, line);
            std::vector<std::string> inputWords;
            std::stringstream out(line);
            std::string oneWord{};
            while (out >> oneWord) {
                inputWords.emplace_back(oneWord);
            }

            bool correct = inputWords.size() == checkedColumns.size();
            if (correct) {
                for(auto i = 0; i < inputWords.size(); i++) {
                    correct &= inputWords[i] == words->at(checkedColumns[i]);
                }
            }

            if (correct) {
                std::cout << "Correct!"<< std::endl;
                words = nullptr;
                count++;
            }
            else {
                std::cout << "Wrong!" << std::endl;
                
                std::cout << "Correct: ";
                for (int i = 1; i < words->size(); i++) {
                    const std::string& word = words->at(i);
                    //std::cout << std::string_view(word + space).substr(0, max - (count_letters(word) - word.size() ) );
                    fmt::print("{}", std::string_view(word + space).substr(0, max - (count_letters(word) - word.size() ) ));
                } 
                //std::cout << words->front() << std::endl;
                fmt::print("{}\n", words->front());

                std::cout << "Answer:  ";
                auto it = inputWords.begin();
                auto itP = checkedColumns.begin();
                for (int i = 1; i < words->size(); i++) {
                    if (i == *itP)
                    {
                        itP++;
                        if (it != inputWords.end()){
                            std::cout << std::string_view(*it + space).substr(0, max);
                            it++;
                        }
                        else
                            std::cout << std::string_view(space).substr(0, max);
                    }
                    else
                        std::cout << std::string_view(space).substr(0, max);
                }
                
                std::cout << std::endl;           
            }
        }
        std::cout << "Round  " <<  round << ". Passed word is " << count << std::endl;
        round++;
    }


    return;
}

void ReaderTable::Print() {
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

int ReaderTable::GetMaxLength() const{
    return maxLength_;
}

void ReaderTable::Read(const std::string_view fileName, const std::string_view sheet, const std::vector<char>& allColumns, 
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

int main(int argv, char ** args)
{
    TrainerWords trainer;
    trainer.start();
    return 0;
}     

