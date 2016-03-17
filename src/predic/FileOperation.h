//
// Created by maiikeru on 3/16/16.
//

#ifndef PREDIC_FILEOPERATION_H
#define PREDIC_FILEOPERATION_H

#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>


using namespace boost::filesystem;

class FileOperation {

private:
    std::vector<std::string> list;

    bool LoadDir(std::string input);

    bool LoadFile(std::string input);


public:

    FileOperation();


    bool LoadInputToVector(std::string input);

    std::vector<std::string> GetFilesVector();

    bool IsDirectory(std::string input);
};


#endif //PREDIC_FILEOPERATION_H
