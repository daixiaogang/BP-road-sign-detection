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

    std::vector<string> list;

    int camera;
    bool show, no_class, clasify, debug, cross, output_mode;
    bool image_mode, video_mode, cam_mode;

public:


    ProgramOption(int argc, char **argv);

    virtual ~ProgramOption();

    void InitDefaultValues();


    int ParseArgument(int argc, char **argv, FileOperation *p_fopt);


    bool ProcessOption(FileOperation *p_fopt);


    std::vector<std::string> GetVectorOfFiles() {
        return list;
    }

    std::string GetPathModelAboost();
    std::string GetPathModelSVM();

    std::string GetOutputPath();


    int GetMode();



    int GetCameraRun();

    // udelat na bool
    int ToolMode();

    bool GetModeShow();


    bool GetModelClassif();

};



#endif //PREDIC_PROGRAMOPTION_H
