//
// Created by maiikeru on 3/5/16.
//


#ifndef PREDIC_PROGRAMOPTION_H
#define PREDIC_PROGRAMOPTION_H

#include <iostream>
#include <vector>
#include "boost/program_options.hpp"
#include "FileOperation.h"

using namespace std;

namespace {
    const size_t ERROR_IN_COMMAND_LINE = 1;
    const size_t SUCCESS = 0;
    const size_t ERROR_UNHANDLED_EXCEPTION = 2;
} // namespace

class ProgramOption {
private:
    std::string input_path;
    std::string output;
    std::string annotation;
    std::string m_aboost;
    std::string m_svm;

    int camera;
    bool show, no_class, clasify, debug, cross;
    bool image_mode, video_mode, cam_mode;

public:


    ProgramOption(int argc, char **argv);

    virtual ~ProgramOption();

    void InitDefaultValues();


    int ParseArgument(int argc, char **argv);


    void ProcessOption(FileOperation *p_fopt);
};



#endif //PREDIC_PROGRAMOPTION_H
