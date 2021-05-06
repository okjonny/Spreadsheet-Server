#include "writer.h"

namespace ss
{
    /**
     * Creates a shared_ptr<spreadsheet_file> to write to
     * @param sf - spreadsheet file
     */
    writer::writer(std::shared_ptr<spreadsheet_file> sf) : _sf(sf)
    {}

    /**
     * Writes message to the file
     * @param message - to send to file
     */
    void writer::write_to_file(const std::string &message)
    {
        _sf->write(message + "\n");
    }
}