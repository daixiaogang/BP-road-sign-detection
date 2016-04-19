//
// Created by maiikeru on 4/17/16.
//

#ifndef TRAIN_EXTRACTOR_H
#define TRAIN_EXTRACTOR_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <string>


class Extractor {

private:
    std::vector<float > getHog(std::string filename);


public:


    //bool walk_directory(std::string in_dir, std::string out_dir);

    Extractor();
    ~Extractor();


    bool walk_directory(const char *in_dir, const char *out_dir);
};


#endif //TRAIN_EXTRACTOR_H
