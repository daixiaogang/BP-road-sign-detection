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
