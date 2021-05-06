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

}