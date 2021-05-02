//
//Created by Lauren Schwenke on 5/1/21.
//
#include <string>
#include <fstream>
#include <memory>

class spreadsheet_file {
public:
    std::fstream file;

    spreadsheet_file(const std::string &path) : _path(path)
    {
        file.open(path);
    }

    void write(const std::string &dataToWrite)
    {
        file << dataToWrite;
    }

private:
    std::string _path;
};

class Writer {
public:
    Writer(std::shared_ptr<spreadsheet_file> sf) : _sf(sf)
    {}

    void write_to_file(std::string message)
    {
        _sf->write(message);
    }

private:
    std::shared_ptr<spreadsheet_file> _sf;
};


