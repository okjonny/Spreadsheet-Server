//
// Created by jonny on 5/4/21.
//

#include "writer.h"

namespace ss
{
    writer::writer(std::shared_ptr<spreadsheet_file> sf) : _sf(sf)
    {}

    void writer::write_to_file(const std::string &message)
    {
        _sf->write(message + "\n");
    }
}