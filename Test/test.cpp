//
// Created by hibban on 4/26/21.
//
#include <regex>
#include <iostream>
#include <string>
#include "../spreadsheet/formula.h"
#include <algorithm>

int main()
{
    ss::formula f("(1  /2 +A_3 * 6 - Z99)");
    ss::formula f1("(1  /         2    + a3 * 6 - z99)");

    std::cout << f.to_string() << std::endl << f1.to_string() << std::endl;

    std::locale loc;
    std::string normalized = "a3";


    if(f == f1)
        std::cout << normalized << std::endl;
}
