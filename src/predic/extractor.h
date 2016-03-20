//
// Created by maiikeru on 1/18/16.
//

#ifndef SVM_EXTRACTOR_H
#define SVM_EXTRACTOR_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

std::vector<float > getHog(cv::Mat src);



#endif //SVM_EXTRACTOR_H
