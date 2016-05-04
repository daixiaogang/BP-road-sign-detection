#include <iostream>
#include "ProgramOption.h"
#include "ABoostDetection.h"
#include "Classification.h"

using namespace std;
using namespace cv;

bool InitModels(ABoostDetection *p_adet, Classification *pClassification, ProgramOption *pOption) {

    if (!p_adet->LoadModel()) {
        cerr << "ERROR: " << "Loading  detector model of adaboost:" << endl;
        return false;
    }
    cout << "Model Adaboost is loaded" << endl;

    if (!pOption->GetModelClassif()) {
        if (!pClassification->loadModel()) {
            cerr << "ERROR: " << "Loading  classificator model of svm" << endl;
            return false;
        }
        cout << "Model SVM is loaded" << endl;
    }

    return true;
}


bool InitVideoCapture(VideoCapture *pCapture, int input_cam) {

    pCapture->open(input_cam);
    return pCapture->isOpened();
}

bool InitVideoCapture(VideoCapture *pCapture, string input_file) {

    pCapture->open(input_file);
    return pCapture->isOpened();
}


const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

string JoinFile(string out_dit, string file);

void CreateFile(string filename);

string GetFileName(string file);

int main(int argc, char **argv) {

    // vytvoreni konstruktoru a prace se vstupem
    ProgramOption *opt = new ProgramOption(argc, argv);

    if (opt->finish != 0) {
        delete opt;
        return opt->finish;
    }

    if (opt->GetModeHelp()) {
        delete opt;
        return EXIT_SUCCESS;
    }

    // nacteni a kontrola vstunich dat
    vector<string> list_input;
    list_input = opt->GetVectorOfFiles();

    // vytvoreni konruktoru ori aboost detektor
    ABoostDetection *adet = new ABoostDetection(opt->GetPathModelAboost());

    Classification *classif = new Classification(opt->GetPathModelSVM());

    //nacteni model
    if (!InitModels(adet, classif, opt)) {
        delete opt;
        delete adet;
        return EXIT_FAILURE;
    }

    vector<Mat> classif_icon = classif->GetImgIcon();

    time_t start, end;

    // fps calculated using number of frames / seconds
    double fps = 0;

    // frame counter
    int counter = 0;

    // floating point seconds elapsed since start
    double sec;

    std::string dbg_frame_dir = "frame/";
    std::string dbg_d_roc_file = "D_ROC.txt";
    std::string dbg_d_msr_file = "D_MSR.txt";

    std::string dbg_c_roc_file = "C_ROC.txt";
    std::string dbg_c_msr_file = "C_MSR.txt";


    std::string anot_file = "anotation.txt";


    VideoCapture capture;
    Mat frame;
    Mat original;
    Mat small_image;
    int cont = 84;
    string WindowName;

    string x1,x2,y1,y2;

    std::vector<string> output_anotation;
    if (opt->GetMode() == 1) {
        WindowName = "Camera";
        std::vector<double > predic_classes;

        string file_name;

        bool make_anotatin = false;

        double t, td, tc, te;
        if (!InitVideoCapture(&capture, opt->GetCameraRun())) {
            cerr << "RROOR load camera: " << opt->GetCameraRun() << endl;

            delete opt;
            delete adet;
            return EXIT_FAILURE;
        }

        // start the clock
        time(&start);

        while (capture.read(frame)) {
            if (frame.empty()) {
                printf(" --(!) No captured frame -- Break!");
                break;
            }

            Mat prev = frame.clone();


            t = (double) getTickCount();

            original = frame.clone();
            resize(frame, frame, Size(FRAME_WIDTH, FRAME_HEIGHT), INTER_CUBIC);

            std::vector<Rect> sign;

            sign = adet->Detection(frame, fps);


            if (!sign.empty() && !opt->GetModelClassif()) {
                predic_classes =   classif->predic(frame, original, sign);

                //adet->Draw_object(frame, sign);

                //classif->DrawClassif(frame);

            }
            else
                adet->Draw_object(frame, sign);


            if (opt->GetModeShow()) {

                namedWindow(WindowName, WINDOW_AUTOSIZE);
                imshow(WindowName, frame);
                waitKey(1);

                //imshow("window_name", frame);
                //waitKey(1);
            }


            if (make_anotatin){
                file_name = "Frame@";

                for (unsigned long j = 0; j < sign.size(); ++j) {
                    string anotace = file_name+to_string(counter).c_str()+";";
                    x1 = to_string(sign.at(j).x);
                    y1 = to_string(sign.at(j).y);
                    x2 = to_string(sign.at(j).x + sign.at(j).width);
                    y2 = to_string(sign.at(j).y + sign.at(j).height);

                    anotace+=x1+";"+y1+";"+x2+";"+y2;


                    if (!opt->GetModelClassif())
                        anotace+=";"+to_string((int)predic_classes.at(j));

                    anotace+="\n";
                    output_anotation.push_back(anotace);
                }
            }




            // see how much time has elapsed
            time(&end);
            // calculate current FPS
            ++counter;
            sec = difftime(end, start);

            fps = counter / sec;

            // overflow protection
            if (counter == (INT_MAX - 1000))
                counter = 0;


            t = ((double) getTickCount() - t) / getTickFrequency();

            cout << "QQ:" << t << ":" << 1 / t << ":" << td << ":" << tc << ":" << te << endl;

            fps = 1 / t;

            cout<<"FPS:"<<fps<<endl;

            //if (counter == 10this->object0)
            //    break;
        }

        if (make_anotatin) {
            // Create outputs for detector
            string anotating_file = JoinFile(opt->GetOutputPath(), anot_file);
            ofstream anot;
            anot.open(anotating_file, ios::out);

            if (anot.good()) {
                for (int k = 0; k < output_anotation.size(); ++k)
                    anot << output_anotation.at(k);
                anot.close();
            }
        }


        output_anotation.clear();
        frame.release();
        capture.release();




    } else if (opt->GetMode() == 2) {
        WindowName = "Video";
        std::vector<double > predic_classes;
        for (int i = 0; i < list_input.size(); i++) {
            cout << list_input.at(i) << ":" << list_input.size() << endl;
            if (!InitVideoCapture(&capture, list_input.at(i))) {
                cerr << "RROOR load file: " << list_input.at(i) << endl;

                delete opt;
                delete adet;
                return EXIT_FAILURE;
            }

            cont += 1;
            string pat = "/tmp/" + to_string(i) + ".avi";
            VideoWriter video(pat, CV_FOURCC('D', 'I', 'V', 'X'), 20, Size(640, 480), true);

            // start the clock
            counter = 0;
            fps = 0;
            double t, td, tc, te;

            while (capture.read(frame)) {
                Mat prev = frame.clone();

                t = (double) getTickCount();

                if (frame.empty()) {
                    printf(" --(!) No captured frame -- Break!");
                    break;
                }


                resize(frame, frame, Size(FRAME_WIDTH, FRAME_HEIGHT), INTER_CUBIC);
                original = frame.clone();

                std::vector<Rect> sign;
                std::vector<float> descriptors;

                sign = adet->Detection(frame, fps);

                string patr = "/tmp/cc/" + to_string(counter) + ".jpg";
                imwrite(patr, frame);
                adet->Draw_object(frame, sign);

                if (!sign.empty() && !opt->GetModelClassif()) {
                    predic_classes =   classif->predic(frame, original, sign);
                }


                if (opt->GetModeShow()) {

                    namedWindow(WindowName, WINDOW_AUTOSIZE);
                    imshow(WindowName, frame);
                    waitKey(1);

                }

                //video.write(frame);


                //sign.clear();
                descriptors.clear();
                frame.release();
                original.release();

                t = ((double) getTickCount() - t) / getTickFrequency();

                cout << "QQ:" << t << ":" << 1 / t << ":" << td << ":" << tc << ":" << te << endl;

                fps = 1 / t;
                counter++;




                pat = "/tmp/tt/" +to_string(i)+ "_"+ to_string(counter) + ".jpg";
                imwrite(pat, prev);
            }


            capture.release();
            video.release();


        }
    }
    else if (opt->GetMode() == 3) {
        WindowName = "Image";

        // Create outputs for detector
        string anotating_file = JoinFile(opt->GetOutputPath(), anot_file);

        ofstream anot;
        anot.open(anotating_file, ios::out);

        string file_name;
        string x1,x2,y1,y2;

        bool make_anotatin = true;

        std::vector<double > predic_classes;

        for (int i = 0; i < list_input.size()-1; ++i) {

            frame = imread(list_input.at(i));
            original = frame.clone();
            resize(frame, frame, Size(FRAME_WIDTH, FRAME_HEIGHT), INTER_CUBIC);

            std::vector<Rect> sign;
            sign = adet->Detection(frame, fps);

            adet->Draw_object(frame, sign);

            if (!sign.empty() && !opt->GetModelClassif()) {
              predic_classes =   classif->predic(frame, original, sign);
            }


            if (opt->GetModeShow()) {

                namedWindow(WindowName, WINDOW_AUTOSIZE);
                imshow(WindowName, frame);
                waitKey(1);
            }


            if (make_anotatin){
                file_name = GetFileName(list_input.at(i));

                for (unsigned long j = 0; j < sign.size(); ++j) {
                    string anotace = file_name+";";
                    x1 = to_string(sign.at(j).x);
                    y1 = to_string(sign.at(j).y);
                    x2 = to_string(sign.at(j).x + sign.at(j).width);
                    y2 = to_string(sign.at(j).y + sign.at(j).height);

                    anotace+=x1+";"+y1+";"+x2+";"+y2;


                    if (!opt->GetModelClassif())
                        anotace+=";"+to_string((int)predic_classes.at(j));

                    anotace+="\n";
                    output_anotation.push_back(anotace);
                }
            }

            string pat = "/tmp/bb/" + to_string(i) + ".jpg";
            imwrite(pat, frame);
        }

        if (make_anotatin) {
            // Create outputs for detector
            string anotating_file = JoinFile(opt->GetOutputPath(), anot_file);
            ofstream anot;
            anot.open(anotating_file, ios::out);

            if (anot.good()) {
                for (int k = 0; k < output_anotation.size(); ++k)
                    anot << output_anotation.at(k);
                anot.close();
            }
        }


    }
    else if (opt->GetMode() == 4) {

        bool make_anotatin = true;
        // Create outputs for detector
        string anotating_file = JoinFile(opt->GetOutputPath(), anot_file);

        ofstream anot;
        anot.open(anotating_file, ios::out);

        string file_name;
        string x1,x2,y1,y2;

        WindowName = "Cross-validation";
        std::vector<double > predic_classes;
        // Create output directory for frame
        string full_path_frame = JoinFile(opt->GetOutputPath(), dbg_frame_dir);

        if (opt->GetModeDebug()) if (!opt->CreateDir(full_path_frame))
            cerr << "Do not create degug directory" << endl;

        // Create outputs for detector
        string d_roc_file = JoinFile(opt->GetOutputPath(), dbg_d_roc_file);
        string d_msr_file = JoinFile(opt->GetOutputPath(), dbg_d_msr_file);

        CreateFile(d_roc_file);
        CreateFile(d_msr_file);

        // Create outputs for clasificator
        string c_roc_file = JoinFile(opt->GetOutputPath(), dbg_c_roc_file);
        string c_msr_file = JoinFile(opt->GetOutputPath(), dbg_c_msr_file);

        CreateFile(c_roc_file);
        CreateFile(c_msr_file);

        if (!opt->GetModelClassif())
            classif->WriteLabel(c_roc_file);

        for (int i = 0; i < list_input.size() - 1; i++) {
            // Load image

            try {
                frame = imread(list_input.at(i));
            }
            catch (runtime_error &ex) {
                fprintf(stderr, "Load image imread croosvalidation, frame empty: %s\n", ex.what());
                break;
            }


            // Clone image
            original = frame.clone();

            // Detection
            std::vector<Rect> sign;

            sign = adet->DetectionCross(frame, d_roc_file, d_msr_file, list_input.at(i));

            // Classification
            if (!opt->GetModelClassif()) {
               predic_classes =  classif->predicCross(original, sign, c_roc_file, c_msr_file, false);
            }


            // Show output
            if (opt->GetModeShow()) {
                adet->Draw_object(frame, sign);
                namedWindow(WindowName, WINDOW_AUTOSIZE);
                imshow(WindowName, frame);
                waitKey(1);
            }

            // Save frame
            if (opt->GetModeDebug()) {
                adet->Draw_object(frame, sign);
                boost::filesystem::path image = list_input.at(i);
                string pat;
                pat = full_path_frame + image.filename().string().c_str();
                imwrite(pat, frame);
            }

            if (make_anotatin){
                //file_name = GetFileName(list_input.at(i));
                file_name = list_input.at(i);

                for (unsigned long j = 0; j < sign.size(); ++j) {
                    string anotace = file_name+";";
                    x1 = to_string(sign.at(j).x);
                    y1 = to_string(sign.at(j).y);
                    x2 = to_string(sign.at(j).x + sign.at(j).width);
                    y2 = to_string(sign.at(j).y + sign.at(j).height);

                    anotace+=x1+";"+y1+";"+x2+";"+y2;


                    if (!opt->GetModelClassif())
                        anotace+=";"+to_string((int)predic_classes.at(j));

                    anotace+="\n";
                    output_anotation.push_back(anotace);
                }
            }



            string pat = "/tmp/bb/" + to_string(i) + ".jpg";
            imwrite(pat, frame);

            frame.release();
        }

        if (make_anotatin) {
            // Create outputs for detector
            string anotating_file = JoinFile(opt->GetOutputPath(), anot_file);
            ofstream anot;
            anot.open(anotating_file, ios::out);

            if (anot.good()) {
                for (int k = 0; k < output_anotation.size(); ++k)
                    anot << output_anotation.at(k);
                anot.close();
            }
        }

    }
    else {
        cerr << "Sometimg input is wrong" << endl;
    }


    delete opt;
    delete adet;

    return 0;
}

string GetFileName(string file) {
    boost::filesystem::path p = file;
    return  p.filename().string();
}

void CreateFile(string filename) {
    ofstream file;
    file.open(filename, ios::out);
    file.close();
}

string JoinFile(string out_dit, string file) {
    boost::filesystem::path full_path_frame;

    boost::filesystem::path dir = out_dit;
    boost::filesystem::path filename = file;
    full_path_frame = dir / filename;

    return full_path_frame.string();
}

