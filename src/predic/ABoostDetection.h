//
// Created by maiikeru on 3/17/16.
//

#ifndef PREDIC_ABOOSTDETECTION_H
#define PREDIC_ABOOSTDETECTION_H

#include <iostream>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;


class ABoostDetection {

private:

    string model_file_name;

    CascadeClassifier detector_cascade;

    double threshold = 0.0;


public:


    ABoostDetection(string model_file);

    bool LoadModel();

    vector<Rect> Detection(Mat frame, double fps);

    vector<Rect> DetectionCross(Mat frame, string roc_file, string msr_file, string file);


    void Draw_object(Mat mat, vector<Rect> vector);
    void Draw_object_debug(Mat mat, vector<Rect> vector);
};


#endif //PREDIC_ABOOSTDETECTION_H
