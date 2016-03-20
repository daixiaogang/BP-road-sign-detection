//
// Created by maiikeru on 1/18/16.
//

#include "extractor.h"

std::vector<float > getHog(cv::Mat src) {
    cv::Mat dst;

    // minimum dimensions are 40x40
    int rows = src.rows > 40 ? src.rows % 40 : src.rows - 40;
    int cols = src.cols > 40 ? src.cols % 40 : src.cols - 40;

    rows = rows < 20 ? -rows : 40 - rows;
    cols = cols < 20 ? -cols : 40 - cols;

    // image dimensions must be multiple of 40
    cv::resize(src, dst, cv::Size(src.cols + cols, src.rows + rows));

    // parameters used to change default HOG values according to image size
    // vector of fixed size 1568 is obtained
    int rowMagnitude = dst.rows / 40;
    int colMagnitude = dst.cols / 40;

    cv::HOGDescriptor hog
            (
                    cv::Size(dst.cols, dst.rows),
                    cv::Size(10 * colMagnitude, 10 * rowMagnitude),
                    cv::Size(5 * colMagnitude, 5 * rowMagnitude),
                    cv::Size(5 * colMagnitude, 5 * rowMagnitude),
                    8,
                    1,
                    -1,
                    cv::HOGDescriptor::L2Hys,
                    0.5,
                    false,
                    cv::HOGDescriptor::DEFAULT_NLEVELS
            );

    std::vector<float> descriptors;
    hog.compute(dst, descriptors, cv::Size(0,0));


    return descriptors;
}