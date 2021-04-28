//
// Created by jonny on 4/27/21.
//

#ifndef TEAMPOG_CS3505_CELL_H
#define TEAMPOG_CS3505_CELL_H

#include "string"
#include "nlohmann/json.hpp"

namespace ss
{
    class cell {
        std::string messageType;
        std::string cellName;
        std::string contents;
        std::vector<std::string> cell_history;

    public:
        cell(std::string name, std::string cell_contents);

        void update_cell(std::string update);

        std::vector<std::string> get_cell_history();

        NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(cell, messageType, cellName, contents)
    };
}

#endif //TEAMPOG_CS3505_CELL_H
