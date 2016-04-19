//
// Created by maiikeru on 3/17/16.
//

#ifndef PREDIC_CLASSIFICATION_H
#define PREDIC_CLASSIFICATION_H
#include <iostream>
#include <opencv2/core/mat.hpp>
#include "lutTable.h"
#include "extractor.h"

namespace libsvm {
#include "Library/SVM/svm.h"
}
#include <opencv2/imgcodecs.hpp>


using namespace std;

class Classification {
private:
    string model_file_name;

    vector<cv::Mat> images_icon;


    int *model_label;

    vector<double > label_model;



    struct libsvm::svm_model *model;

    libsvm::svm_model *loadModelFromFile(bool *pBoolean);

public:

    libsvm::svm_parameter param;

    bool usedModel = false;


    bool LoadImagesIcon();

    vector<cv::Mat>GetImgIcon();

    bool CheckRoi(int i, int i1);


    bool loadModel();


    double detekuj_prob(vector<float> value);

    vector<float> extractHog(cv::Rect_<int> &faces, cv::Mat mat);

    void freeMem();


    Classification(string model_file);
    ~Classification();

    void predic(cv::Mat frame, cv::Mat original, vector<cv::Rect> sign);
};


#endif //PREDIC_CLASSIFICATION_H
