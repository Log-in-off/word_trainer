#pragma once

#include <random>

#include "table_reader.hpp"

class TrainerWords final{
public:
    void start();
private:
    TableReader table_;
};



