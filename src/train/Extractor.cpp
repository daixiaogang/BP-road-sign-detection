//
// Created by maiikeru on 4/17/16.
//

#include "Extractor.h"

std::vector<float> Extractor::getHog(std::string filename) {
    cv::Mat src = cv::imread(filename);
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
    hog.compute(dst, descriptors, cv::Size(0, 0));


    return descriptors;
}
/*
bool Extractor::walk_directory(std::string in_dir, std::string out_dir) {


    std::cout<<"AAAA\n"<<out_dir;

    boost::filesystem::path path = in_dir.c_str();
    boost::filesystem::recursive_directory_iterator itr(path);
    //boost::filesystem::path out = out_dir.c_str();
    boost::filesystem::path out = "/tmp/A/";
    boost::filesystem::create_directory(out);



    std::string dir;
    std::string tmp = "/tmp/A/";
    std::string directory = "";
    std::string output = "";
    boost::filesystem::path patchDir;
    while (itr != boost::filesystem::recursive_directory_iterator()) {
        //std::cout << itr->path().string() << std::endl;
        //std::cout << itr->path().filename().string() << std::endl;

        if (boost::filesystem::is_directory(itr.status())) {
            directory = "";
            //std::cout<< "direcory"<<itr->path().string() <<std::endl;
            //std::cout<< "direcory"<< itr->path().parent_path().string() <<std::endl;
            dir = itr->path().filename().string();
            directory += tmp;
            directory += dir;

            patchDir = directory;

            boost::filesystem::create_directory(patchDir);


        }
        else {

            output += directory;
            output += "/";

            if (itr->path().extension() == ".ppm" || itr->path().extension() == ".jpeg" ||
                itr->path().extension() == ".png") {
                std::vector<float> descriptors;
                descriptors = this->getHog(itr->path().string());

                output += itr->path().stem().string();
                output += ".txt";
                std::ofstream outfile(output);
                for (int i = 0; i < descriptors.size(); ++i) {
                    outfile << descriptors[i] << std::endl;
                }
                outfile.close();
            }

            //std::cout<< itr->path().parent_path() <<std::endl;
            //std::cout<< itr->path().filename() <<std::endl;
            //std::cout<<  itr->path().stem() <<std::endl;
            //std::cout<<  itr->path().extension() <<std::endl;

            output = "";
        }
        ++itr;
    }

    return true;
}
*/
Extractor::Extractor() {

}

Extractor::~Extractor() {

}


bool Extractor::walk_directory(const char *in_dir, const char *out_dir) {

    std::cout<<in_dir<<std::endl;
    std::cout<<out_dir<<std::endl;


    boost::filesystem::path path = in_dir;
    boost::filesystem::recursive_directory_iterator itr(path);
    //boost::filesystem::path out = out_dir.c_str();
    boost::filesystem::path out = "/tmp/A/";
    boost::filesystem::create_directory(out);


    std::string dir;
    std::string tmp = "/tmp/A/";
    std::string directory = "";
    std::string output = "";
    boost::filesystem::path patchDir;
    while (itr != boost::filesystem::recursive_directory_iterator()) {
        //std::cout << itr->path().string() << std::endl;
        //std::cout << itr->path().filename().string() << std::endl;

        if (boost::filesystem::is_directory(itr.status())) {
            directory = "";
            //std::cout<< "direcory"<<itr->path().string() <<std::endl;
            //std::cout<< "direcory"<< itr->path().parent_path().string() <<std::endl;
            dir = itr->path().filename().string();
            directory += tmp;
            directory += dir;

            patchDir = directory;

            boost::filesystem::create_directory(patchDir);


        }
        else {

            output += directory;
            output += "/";

            if (itr->path().extension() == ".ppm" || itr->path().extension() == ".jpeg" ||
                itr->path().extension() == ".png") {
                std::vector<float> descriptors;
                descriptors = this->getHog(itr->path().string());

                output += itr->path().stem().string();
                output += ".txt";
                std::ofstream outfile(output);
                for (int i = 0; i < descriptors.size(); ++i) {
                    outfile << descriptors[i] << std::endl;
                }
                outfile.close();
            }

            //std::cout<< itr->path().parent_path() <<std::endl;
            //std::cout<< itr->path().filename() <<std::endl;
            //std::cout<<  itr->path().stem() <<std::endl;
            //std::cout<<  itr->path().extension() <<std::endl;

            output = "";
        }
        ++itr;
    }

    return true;


    return false;
}
