//
// Created by maiikeru on 3/5/16.
//

#include "ProgramOption.h"

ProgramOption::~ProgramOption() {

}


ProgramOption::ProgramOption(int argc, char **argv) {
    InitDefaultValues();

    FileOperation *fopt = new FileOperation();


    int ret = ParseArgument(argc, argv);
    if (ret != 0) {
        std::cerr << "Chyba" << std::endl;
    }

    ProcessOption(fopt);


    delete fopt;
}

void ProgramOption::InitDefaultValues() {

    m_aboost = "../../../models/models_Aboost_detector/";
    m_svm = "../../../models/models_/model_SVM_classifier";


    camera = 0;
    show = false, no_class = false, clasify = false, debug = false, cross = false;
    image_mode = false, video_mode = false, cam_mode = false;

}


int ProgramOption::ParseArgument(int argc, char **argv) {

    try {
        /** Define and parse the program options
         */
        namespace po = boost::program_options;
        po::options_description desc("Options");
        //po::variables_map vm;
        boost::program_options::variables_map vm;

        try {
            desc.add_options()
                    ("image,i", po::value<std::string>(&input_path),
                     "File or directory input images. If not chosen than default camera mode run.")
                    ("video,v", po::value<std::string>(&input_path),
                     "File or directory input videos. If not chosen than default camera mode run.")
                    ("camera,e", po::value<int>(&camera), "Selectin of the camera. Default 0.")
                    ("output,o", po::value<std::string>(&output), "Directory output result")
                    ("show,s", "Show current image or frame by video")
                    ("annotation,a", po::value<std::string>(&annotation), "File of output annotation")
                    ("classification,c", "Make only classification")
                    ("no_classification,n", "Make only detection")
                    ("model_detect,t", po::value<std::string>(&m_aboost), "File of model  detection")
                    ("model_classif,f", po::value<std::string>(&m_svm), "File of model  clasificatin")
                    ("help,h", "Print help messages")
                    ("cross_validation,r", "Cross-validation")
                    ("debug,g", "Debug mode");
            po::store(po::parse_command_line(argc, argv, desc), vm); // can throw

            /** --help option
             */
            if (vm.count("help")) {
                std::cout << "Basic Command Line Parameter App" << std::endl
                << desc << std::endl;
                cout << "help" << endl;
                return SUCCESS;
            }

            if (vm.count("show")) {
                show = true;
                cout << "show" << endl;
            }

            if (vm.count("classification") && vm.count("no_classification")) {
                cerr << "ERROR: " << "Can be only one parameter --classification|--no_classification" << endl;
                return ERROR_IN_COMMAND_LINE;
            }

            if (vm.count("classification")) {
                cout << "classif" << endl;
                clasify = true;
            }

            if (vm.count("no_classification")) {
                no_class = true;
                cout << "no clasiif" << endl;
            }

            if (!vm.count("image") && (!vm.count("video"))) {
                cam_mode = true;
                cout << "camera mode" << endl;
            }
            else if (vm.count("image") && !vm.count("video") && !vm.count("camera")) {
                image_mode = true;

                cout << "image mode" << endl;
            }
            else if (!vm.count("image") && vm.count("video") && !vm.count("camera")) {
                video_mode = true;
                cout << "video mode" << endl;
            }
            else {
                cerr << "ERROR: " << "Can be only one parameter --image|--video|--camera" << endl;
                return ERROR_IN_COMMAND_LINE;
            }

            if (vm.count("cross_validation")) {
                cross = true;
                cout << "cross_validaton" << endl;
            }

            if (vm.count("debug")) {
                debug = true;
                cout << "debug" << endl;
            }

            po::notify(vm);
        }
        catch (po::error &e) {
            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
            std::cerr << desc << std::endl;
            return ERROR_IN_COMMAND_LINE;
        }

        // application code here //

    }
    catch (std::exception &e) {
        std::cerr << "Unhandled Exception reached the top of main: "
        << e.what() << ", application will now exit" << std::endl;
        return ERROR_UNHANDLED_EXCEPTION;
    }


    return SUCCESS;
}


void ProgramOption::ProcessOption(FileOperation *p_fopt) {

    std::vector<std::string> list;

    if (image_mode || video_mode){
        if (p_fopt->LoadInputToVector(input_path))
            list = p_fopt->GetFilesVector();
        else
            cerr<<"chyba procesoption"<<endl;
    }



}
