#include "word_trainer.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string_view>

#include <fmt/core.h>
#include "showing.hpp"

template<typename T>
static void shuffleContainer(T& container) {
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(container.begin(), container.end(), rng);
}

const std::vector<std::string> getInputWords() {
    std::string line{};
    std::getline(std::cin, line);
    std::vector<std::string> inputWords{};
    std::stringstream out(line);
    std::string oneWord{};
    while (out >> oneWord) {
        inputWords.emplace_back(oneWord);
    }
    return inputWords;
}

void TrainerWords::start() {
    table_.read();

    //std::vector<TableReader::cIterator> allWord_test(table_.cbegin(), table_.cend());

    const std::vector<int> checkedColumns = table_.getIndexesCheckedColumns();
    const std::vector<int> promptColumns = table_.getIndexesPromptColumns();

    std::vector<const std::vector<std::string>*> allWord;
    for( auto it = table_.cbegin(), end = table_.cend(); it != end; it++ ) {
        allWord.push_back(&(*it));
    }

    shuffleContainer(allWord);

    std::random_device dev{};
    std::default_random_engine engine(dev());
    std::uniform_int_distribution <int> uniform_type(0, promptColumns.size()-1);

    const size_t max = table_.getMaxLength() + 1;
    const std::string space(max, ' ');
    int count = 0;
    int round = 1;
    while(count != allWord.size()) {
        count = 0;
        for(auto& words:allWord) {
            if(!words) {
                count++;
                continue;
            }
            const int randomFromList = promptColumns[uniform_type(engine)];
            fmt::print("{}\n", words->at(randomFromList));

            auto inputWords = getInputWords();

            bool correct = inputWords.size() == checkedColumns.size();
            if (correct) {
                for(auto i = 0; i < inputWords.size(); i++) {
                    correct &= inputWords[i] == words->at(checkedColumns[i]);
                }
            }

            if (correct) {
                fmt::print("Correct!\n");
                words = nullptr;
                count++;
            }
            else {
                fmt::print("Wrong!\n");
                
                fmt::print("Correct: ");
                for (int i = 1; i < words->size(); i++) {
                    const std::string& word = words->at(i);
                    fmt::print("{}", std::string_view(word + space).substr(0, max - (getCountLetters(word) - word.size() ) ));
                } 
                fmt::print("{}\n", words->front());

                fmt::print("Answer:  ");
                auto it = inputWords.begin();
                auto itP = checkedColumns.begin();
                for (int i = 1; i < words->size(); i++) {
                    if (i == *itP) {
                        itP++;
                        if (it != inputWords.end()){
                            fmt::print("{}", std::string_view(*it + space).substr(0, max));
                            it++;
                        }
                        else
                            fmt::print("{}", std::string_view(space).substr(0, max));
                    }
                    else
                        fmt::print("{}", std::string_view(space).substr(0, max));
                }
                
                fmt::print("\n");           
            }
        }
        fmt::print("Round {}. {} words passed\n", round, count);
        round++;
    }
    return;
}