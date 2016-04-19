//
// Created by maiikeru on 4/18/16.
//



#include "TrainWaldBoost.h"

TrainWaldBoost::TrainWaldBoost() {

}

TrainWaldBoost::~TrainWaldBoost() {

}

std::vector<cv::Mat> TrainWaldBoost::read_imgs(const std::string &path) {
    vector <String> filenames;
    glob(path, filenames);
    vector <Mat> imgs;
    for (size_t i = 0; i < filenames.size(); ++i) {
        imgs.push_back(imread(filenames[i], CV_LOAD_IMAGE_GRAYSCALE));
    }
    return imgs;
}

vector<Mat> TrainWaldBoost::sample_patches(const string &path, int n_rows, int n_cols, size_t n_patches) {
    vector <String> filenames;
    glob(path, filenames);
    vector <Mat> patches;
    size_t patch_count = 0;
    for (size_t i = 0; i < filenames.size(); ++i) {
        Mat img = imread(filenames[i], CV_LOAD_IMAGE_GRAYSCALE);
        for (int row = 0; row + n_rows < img.rows; row += n_rows) {
            for (int col = 0; col + n_cols < img.cols; col += n_cols) {
                patches.push_back(img(Rect(col, row, n_cols, n_rows)).clone());
                patch_count += 1;
                if (patch_count == n_patches) {
                    goto sampling_finished;
                }
            }
        }
    }
    sampling_finished:
    return patches;}

void TrainWaldBoost::train(const char *pos_dir, const char *neg_dir, const char *model_name) {
    string pos_samples_path = pos_dir;
    string neg_imgs_path = neg_dir;

    vector <Mat> pos_imgs = read_imgs(pos_samples_path);
    vector <Mat> neg_imgs = sample_patches(neg_imgs_path, 24, 24, pos_imgs.size() * 10);


    assert(pos_imgs.size());
    assert(neg_imgs.size());

    std::cout << "pos_imgs" << pos_imgs.size() << endl;
    std::cout << "neg_imgs" << neg_imgs.size() << endl;

    int n_features;
    Mat pos_data, neg_data;




    Ptr <xobjdetect::CvFeatureEvaluator> eval = xobjdetect::CvFeatureEvaluator::create();
    eval->init(xobjdetect::CvFeatureParams::create(), 1, Size(24, 24));

    n_features = eval->getNumFeatures();

    pos_data = Mat1b(n_features, (int) pos_imgs.size());
    neg_data = Mat1b(n_features, (int) neg_imgs.size());

    std::cout << "Extrakce priznaku" << endl;
    for (size_t k = 0; k < pos_imgs.size(); ++k) {
        eval->setImage(pos_imgs[k], +1, 0, boost_.get_feature_indices());
        for (int j = 0; j < n_features; ++j) {
            pos_data.at<uchar>(j, (int) k) = (uchar)(*eval)(j);
        }
    }

    for (size_t k = 0; k < neg_imgs.size(); ++k) {
        eval->setImage(neg_imgs[k], 0, 0, boost_.get_feature_indices());
        for (int j = 0; j < n_features; ++j) {
            neg_data.at<uchar>(j, (int) k) = (uchar)(*eval)(j);
        }
    }

    std::cout << "Train" << endl;
    const int stages[] = {64, 128, 256, 512, 1024};

    boost_.reset(stages[0]);
    boost_.fit(pos_data, neg_data);


    FileStorage fs(model_name, FileStorage::WRITE);
    fs << "waldboost";
    boost_.write(fs);
}

void TrainWaldBoost::predic(const char *model_name, const char *input, const char *output, const char *label) {
    vector<Rect> bboxes;
    vector<double> confidences;
    Mat img = imread(input, CV_LOAD_IMAGE_GRAYSCALE);


    //resize(img, img, Size(640, 480));

    FileStorage fs(model_name, FileStorage::READ);

    cout<<"read"<<endl;
    boost_.read(fs.getFirstTopLevelNode());


    Mat test_img = img.clone();
    bboxes.clear();
    confidences.clear();
    vector<float> scales;
    for (float scale = 0.2f; scale < 1.2f; scale *= 1.5f) {
        scales.push_back(scale);
        cout<<scale<<endl;
    }

    Ptr<cv::xobjdetect::CvFeatureParams> params = cv::xobjdetect::CvFeatureParams::create();
    Ptr<cv::xobjdetect::CvFeatureEvaluator> eval = cv::xobjdetect::CvFeatureEvaluator::create();
    eval->init(params, 1, Size(24, 24));
    boost_.detect(eval, img, scales, bboxes, confidences);
    assert(confidences.size() == bboxes.size());

    FILE *fhandle = fopen(label, "a");
    for (size_t i = 0; i < bboxes.size(); ++i) {
        Rect o = bboxes[i];
        fprintf(fhandle, "%s;%u;%u;%u;%u;%lf\n",
                label, o.x, o.y, o.width, o.height, confidences[i]);
    }
    for (size_t i = 0; i < bboxes.size(); ++i) {
        rectangle(img, bboxes[i], Scalar(255, 0, 0));
    }
    imwrite(output, img);
}
