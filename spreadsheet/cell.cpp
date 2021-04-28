//
// Created by jonny on 4/27/21.
//

#include "cell.h"

namespace spreadsheet_server
{
    cell::cell(std::string name, std::string cell_contents)
    {
        messageType = "cellUpdated";
        cellName = name;
        contents = cell_contents;
        cell_history.push_back(cell_contents);
    }

    void cell::update_cell(std::string update)
    {
        cell_history.push_back(update);
    }

    /*std::vector<std::string> cell::get_cell_history()
    {
        return cell_history;
    }*/

}
