//
// Created by maiikeru on 3/17/16.
//

#include "ABoostDetection.h"

ABoostDetection::ABoostDetection(string model_file) {
    this->model_file_name = model_file;
}

bool ABoostDetection::LoadModel() {

    return detector_cascade.load(this->model_file_name);
}


vector<Rect> ABoostDetection::Detection(Mat frame, double fps) {
    std::vector<Rect> sign;
    Mat frame_gray;


    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    vector<int> reject_levels;
    vector<double> level_weights;


    this->detector_cascade.detectMultiScale(frame_gray, sign, 2, 2, 0 | CASCADE_SCALE_IMAGE);


    //this->detector_cascade.detectMultiScale(frame_gray, sign, reject_levels, level_weights, 1.1, 1, CASCADE_SCALE_IMAGE,
    //                              Size(), Size(), true);


    for (size_t i = 0; i < sign.size(); i++) {
        Point center(sign[i].x + sign[i].width / 2, sign[i].y + sign[i].height / 2);
        ellipse(frame, center, Size(sign[i].width / 2, sign[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
    }

    putText(frame, std::to_string(fps),
            Point(450, 50), 2, 1, Scalar(0, 255, 255));


    imshow( "window_name", frame );
    waitKey(1);

    return sign;
    }
