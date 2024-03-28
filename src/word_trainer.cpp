#include "word_trainer.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <string_view>
#include <fmt/core.h>

#include "json_reader.hpp"
#include "showing.hpp"

template<typename T>
static void shuffleContainer(T& container) {
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(container.begin(), container.end(), rng);
}

void TrainerWords::start() {
    {   
        JsonReader json;
        const std::string name  = json.GetName();
        const std::string sheet = json.GetSheet();
        const std::vector<char> allColumns = json.GetListAllColumns();
        const std::vector<char> checkingColumns = json.GetListCheckedColumns(); 

        table_.Read(name, sheet, allColumns, checkingColumns);
    }

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