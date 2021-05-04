//
// Created by jonny on 5/4/21.
//

#include "spreadsheet_file.h"


namespace ss
{
    spreadsheet_file::spreadsheet_file(const std::string &path) : _path("../files/" + path)
    {
    }

    void spreadsheet_file::write(const std::string &dataToWrite)
    {
        file.open(_path, std::ios_base::app);
        {
            std::lock_guard<std::mutex> lock(_writerMutex);
            file << dataToWrite;
        }
        file.close();
    }

//    std::vector<std::string> spreadsheet_file::read()
//    {
//        input_file.open(_path);
//        std::lock_guard<std::mutex> lock(_writerMutex);
//        std::vector<std::string> contents;
//        std::string line;
//
//        if (input_file.fail())
//            return contents;
//
//        while (!file.eof())
//        {
//            input_file >> line;
//            contents.push_back(line);
//        }
//        input_file.close();
//        return contents;
//    }

}