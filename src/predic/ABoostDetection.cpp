//
// Created by maiikeru on 3/17/16.
//

#include <fstream>
#include "ABoostDetection.h"

ABoostDetection::ABoostDetection(string model_file) {
    this->model_file_name = model_file;
}

bool ABoostDetection::LoadModel() {

    return detector_cascade.load(this->model_file_name);
}


vector<Rect> ABoostDetection::Detection(Mat frame, double fps) {
    std::vector<Rect> sign;
    std::vector<Rect> object;
    Mat frame_gray;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    vector<int> reject_levels;
    vector<double> level_weights;
    vector<double> level_weights_object;


    double threshold = 1.0;

    //this->detector_cascade.detectMultiScale(frame_gray, sign, 2, 2, 0 | CASCADE_SCALE_IMAGE);


    //this->detector_cascade.detectMultiScale(frame_gray, sign, reject_levels, 1.3,3, 0);

    this->detector_cascade.detectMultiScale(frame_gray, sign, reject_levels, level_weights, 1.6, 2, CASCADE_SCALE_IMAGE,
                                            Size(), Size(), true);


    for (int j = 0; j < level_weights.size(); ++j) {
        if (level_weights[j] >= threshold) {
            object.push_back(sign.at(j));
            level_weights_object.push_back(level_weights.at(j));
        }
    }


    sign = object;
    level_weights = level_weights_object;

    for (Rect obj : sign)
        rectangle(frame, obj, Scalar(0, 255, 0), 2, 6, 0);

    /*
    for (size_t i = 0; i < sign.size(); i++) {
        putText(frame, std::to_string(level_weights[i]),
                Point(sign[i].x, sign[i].y), 1, 1, Scalar(0, 0, 255));

        cerr<<"level_weight:"<<level_weights[i]<<endl;
    }
*/
    //

    /*
    for (size_t i = 0; i < sign.size(); i++) {
        Point center(sign[i].x + sign[i].width / 2, sign[i].y + sign[i].height / 2);
        ellipse(frame, center, Size(sign[i].width / 2, sign[i].height / 2), 0, 0, 360, Scalar(0, 255, 0), 4, 8, 0);
    }
    */

    //putText(frame, std::to_string(fps), Point(frame.cols-200, 50), 2, 1, Scalar(0, 255, 255));


    //imshow( "window_name", frame );
    //waitKey(1);

    frame_gray.release();

    level_weights.clear();
    reject_levels.clear();

    return sign;
}

vector<Rect> ABoostDetection::DetectionCross(Mat frame, string roc_file, string msr_file) {

    // Set frame to grayscale
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    std::vector<Rect> sign;
    vector<int> reject_levels;
    vector<double> level_weights;

    // Detectin operation
    this->detector_cascade.detectMultiScale(frame_gray, sign, reject_levels, level_weights, 1.2, 2, CASCADE_SCALE_IMAGE,
                                            Size(), Size(), true);

    // Write result to files
    ofstream f_d_roc, f_d_msr;
    f_d_roc.open(roc_file, ios::app);
    f_d_msr.open(msr_file, ios::app);

    if (f_d_roc.good() && f_d_msr.good()) {
        if (level_weights.empty()) {
            f_d_roc << "0\n";
            f_d_msr << "0\n";
        }
        else {
            f_d_roc << level_weights[0] << "\n";
            if (level_weights[0] >= this->threshold)
                f_d_msr << "1\n";
        }
    }
    // free memory
    frame_gray.release();
    reject_levels.clear();
    level_weights.clear();

    // Close files
    f_d_roc.close();
    f_d_msr.close();

    return sign;
}

void ABoostDetection::Draw_object(Mat mat, vector<Rect> vector) {
    for (Rect obj : vector)
        rectangle(mat, obj, Scalar(0, 255, 0), 2, 6, 0);
}

void ABoostDetection::Draw_object_debug(Mat mat, vector <Rect> vector) {
    this->Draw_object(mat, vector);

    // TODO level weight

    /*
    for (size_t i = 0; i < sign.size(); i++) {
        putText(frame, std::to_string(level_weights[i]),
                Point(sign[i].x, sign[i].y), 1, 1, Scalar(0, 0, 255));

        cerr<<"level_weight:"<<level_weights[i]<<endl;
    }
*/


}
