//
// Created by maiikeru on 4/14/16.
//

#ifndef TRAIN_PROGRAMOPTION_H
#define TRAIN_PROGRAMOPTION_H

#include <iostream>
#include <vector>
#include "boost/program_options.hpp"


class ProgramOption {

    virtual ~ProgramOption();

    void InitDefaultValues();

    int ParseArgument(int argc, char **argv);


public:
    ProgramOption(int argc, char **argv);
};


#endif //TRAIN_PROGRAMOPTION_H
