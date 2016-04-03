//
// Created by maiikeru on 3/17/16.
//

#ifndef PREDIC_CLASSIFICATION_H
#define PREDIC_CLASSIFICATION_H
#include <iostream>
#include <opencv2/core/mat.hpp>
#include "lutTable.h"

using namespace std;

class Classification {
private:
    string model_file_name;

    vector<cv::Mat> images_icon;

public:

    bool LoadImagesIcon();

    vector<cv::Mat>GetImgIcon();

    bool CheckRoi(int i, int i1);

    Classification(string model_file);
};


#endif //PREDIC_CLASSIFICATION_H
