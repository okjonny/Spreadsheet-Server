#include "spreadsheet_file.h"

namespace ss
{
    /**
     * Creates the file to write to
     * @param path
     */
    spreadsheet_file::spreadsheet_file(const std::string &path) : _path("../files/" + path)
    {
    }

    /**
     * Writes to the file while on multithreading
     * @param dataToWrite
     */
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