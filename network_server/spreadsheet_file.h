//
// Created by jonny on 5/4/21.
//

#ifndef TEAMPOG_CS3505_SPREADSHEET_FILE_H
#define TEAMPOG_CS3505_SPREADSHEET_FILE_H

#include <string>
#include <memory>
#include <dirent.h>
#include <vector>
#include <fstream>
#include <mutex>

namespace ss
{
    class spreadsheet_file {
    public:
        std::ofstream file;
        std::ifstream input_file;

        spreadsheet_file(const std::string &path);

        void write(const std::string &dataToWrite);

//        std::vector<std::string> read();

    private:
        std::string _path;
        std::mutex _writerMutex;
    };
}

#endif //TEAMPOG_CS3505_SPREADSHEET_FILE_H
