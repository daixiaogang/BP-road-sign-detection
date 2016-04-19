//
// Created by maiikeru on 4/18/16.
//

#ifndef TRAIN_TRAINWALDBOOST_H
#define TRAIN_TRAINWALDBOOST_H
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <iostream>

#include <opencv2/xobjdetect.hpp>

#include "Library/xobject/precomp.hpp"



using namespace std;
using namespace cv;


class TrainWaldBoost {


private:
    static std::vector <cv::Mat> read_imgs(const std::string &path);

    static vector <Mat> sample_patches(const string &path,
                                       int n_rows,
                                       int n_cols,
                                       size_t n_patches);


public:
    TrainWaldBoost();
    ~TrainWaldBoost();


    cv::xobjdetect::WaldBoost boost_;

    void train(const char *pos_dir, const char *neg_dir, const char *model_name);

    void predic(const char *model_name, const char *input, const char *output, const char *label);
};


#endif //TRAIN_TRAINWALDBOOST_H
