//
// Created by jonny on 5/4/21.
//

#ifndef TEAMPOG_CS3505_WRITER_H
#define TEAMPOG_CS3505_WRITER_H

#include "spreadsheet_file.h"

namespace ss
{
    class writer {
    public:
        writer(std::shared_ptr<spreadsheet_file> sf);

        void write_to_file(const std::string &message);

    private:
        std::shared_ptr<spreadsheet_file> _sf;

    };
}


#endif //TEAMPOG_CS3505_WRITER_H
