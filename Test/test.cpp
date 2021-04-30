//
// Created by hibban on 4/26/21.
//
#include <regex>
#include <iostream>
#include <string>
#include "../spreadsheet/formula.h"
#include <algorithm>
#include "../spreadsheet/spreadsheet.h"

int main()
{
   ss::spreadsheet ss;
    /*ss::formula f("(1  /2 +A3 * 6 - Z99)");
    ss::formula f1("(1  /         2    + a3 * 6 - z99)");

    std::cout << f.to_string() << std::endl << f1.to_string() << std::endl;

    std::locale loc;
    std::string normalized = "a3";


    if(f == f1)
        std::cout << normalized << std::endl;*/
//
//    ss.set_contents_of_cell("A1", "641658456");
//    std::cout << ss.get_cell_contents("A1") << std::endl;
//
//    ss.set_contents_of_cell("A1", "234");
//    std::cout << ss.get_cell_contents("A1") << std::endl;
//
//    ss.set_contents_of_cell("A1", "2");
//    std::cout << ss.get_cell_contents("A1") << std::endl;
//
//    ss.set_contents_of_cell("A1", "=A5");
//    std::cout << ss.get_cell_contents("A1") << std::endl;
//
//   ss.revert_cell_contents("A1");
//    std::cout << ss.get_cell_contents("A1") << std::endl;
//
//    ss.revert_cell_contents("A1");
//    std::cout << ss.get_cell_contents("A1") << std::endl;
//
//    ss.revert_cell_contents("A1");
//    std::cout << ss.get_cell_contents("A1") << std::endl;
//
//    ss.revert_cell_contents("A1");
//    std::cout << ss.get_cell_contents("A1") << std::endl;

    // No circular dependency
//    ss.set_contents_of_cell("A1", "=A2");
//    ss.set_contents_of_cell("A2", "=A3");
//    ss.set_contents_of_cell("A3", "=A4");

    // Normal circular dependency error:
//    ss.set_contents_of_cell("A1", "=A1");

    // Circular Dependency test
//    ss.set_contents_of_cell("A1", "=A2");
//    std::cout << "A1: " << ss.get_cell_contents("A1") << std::endl;
//    ss.set_contents_of_cell("A2", "=A1");
//    std::cout << "A2: " << ss.get_cell_contents("A2") << std::endl;

//    // Revert cell test
//    ss.set_contents_of_cell("A1", "=A2");
//    std::cout << "A1: " << ss.get_cell_contents("A1") << std::endl;
//
//    ss.set_contents_of_cell("A1", "Big Chungus");
//    std::cout << "A1: " << ss.get_cell_contents("A1") << std::endl;
//
//    ss.set_contents_of_cell("A2", "=A1");
//    std::cout << "A2: " << ss.get_cell_contents("A2") << std::endl;
//    ss.revert_cell_contents("A1");
//    ss.revert_cell_contents("A1");
//    ss.revert_cell_contents("A1");
//    ss.revert_cell_contents("A1");
//    ss.revert_cell_contents("A1");
//    std::cout << "A1: " << ss.get_cell_contents("A1") << std::endl;

//  Revert empty cell
//    ss.set_contents_of_cell("A1", "=A2");
//    std::cout << "A1: " << ss.get_cell_contents("A1") << std::endl;
//    ss.revert_cell_contents("A1");
//    std::cout << "A1: " << ss.get_cell_contents("A1") << std::endl;
//    ss.revert_cell_contents("A1");
//    std::cout << "A1: " << ss.get_cell_contents("A1") << std::endl;

// UNDO/REVERT TEST
    ss.set_contents_of_cell("A2", "Table");
    ss.set_contents_of_cell("A3", "=A2");
    ss.set_contents_of_cell("A2", "Text");
    ss.revert_cell_contents("A3");
    ss.set_contents_of_cell("A2", "Data");
    ss.undo();
    ss.undo();
    ss.undo();
    ss.revert_cell_contents("A2");
    ss.undo();
    ss.revert_cell_contents("A2");
    ss.undo();
    ss.revert_cell_contents("A2");
    ss.undo();
    ss.undo();
    ss.undo();

    return 0;
}
