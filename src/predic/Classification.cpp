//
// Created by maiikeru on 3/17/16.
//

#include <opencv2/imgcodecs.hpp>
#include "Classification.h"

Classification::Classification(string model_file) {
    this->model_file_name = model_file;

}

bool Classification::LoadImagesIcon() {

    cout<<"AAAA"<<LUT_image_filename->size()<<endl;

    cout<<"BBB"<<LUT_image_filename->length()<<endl;


    for (int i = 0; i < LUT_image_filename->length(); ++i) {
        this->images_icon.push_back(cv::imread(LUT_image_filename[i]));
    }

    cout<<"BBB"<<this->images_icon.size()<<endl;

    return false;
}

vector<cv::Mat> Classification::GetImgIcon() {
    return this->images_icon;
}

bool Classification::CheckRoi(int i, int i1) {

    if (i - i1 <=0 )
        return false;

    return true;
}
