//
// Created by maiikeru on 3/16/16.
//


#include "FileOperation.h"

FileOperation::FileOperation() {

}


bool FileOperation::LoadInputToVector(std::string input) {

    path p(input);
    if (exists(p)) {
        if (is_directory(p)) {
            return LoadDir(input);
        }
        else if (is_regular_file(p)) {
            return LoadFile(input);
        }
        else
            std::cerr << "ERR: input is not directory or file. " << input << std::endl;
    }
    else {
        std::cerr << "ERR: input is not exist. " << input << std::endl;
    }


    return false;
}

bool FileOperation::LoadDir(std::string input) {

    boost::filesystem::path path = input;
    boost::filesystem::recursive_directory_iterator itr(path);
    while (itr != boost::filesystem::recursive_directory_iterator()) {

        if (!IsDirectory(itr->path().string())) {
            if (itr->path().extension() == ".ppm" || itr->path().extension() == ".jpeg" ||
                itr->path().extension() == ".png" || itr->path().extension() == ".avi" ||
                itr->path().extension() == ".mp4" || itr->path().extension() == ".3gp"){

                list.push_back(itr->path().string());
            }
        }
        ++itr;
    }

    return true;
}

bool FileOperation::LoadFile(std::string input) {

    std::string line;
    std::ifstream text(input, std::ios::in | std::ios::binary);

    if (text.is_open()) {
        while (text.good()) {
            getline(text, line);
            list.push_back(line);
        }
        text.close();
    }
    else {
        std::cerr << "Unable to open file" << input << std::endl;
        return false;
    }
    return true;
}

std::vector<std::string> FileOperation::GetFilesVector() {
    return list;
}

bool FileOperation::IsDirectory(std::string input) {

    path p(input);

    if (exists(p)) {
        if (is_directory(p))
            return true;
    }

    return false;
}

bool FileOperation::IsFileExist(std::string input) {
    path p(input);
    if (exists(p)) {
        if (is_regular_file(p))
            return true;
    }

    return false;
}
